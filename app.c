#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <string.h>
#include <fcntl.h>
#include "utility.h"
#include <unistd.h>

#define SLAVE_NUM 5
#define INIT_TASK 3
#define STRING_SIZE 11


typedef struct fileDescriptors{
    
    int appToSlave[SLAVE_NUM][2];
    int slaveToApp[SLAVE_NUM][2];
    int highFd;
    //Esto lo tenemos q implementar para cuando agregamos la primer vez, darle tareas solo cuando termine
    int tasksLeft[SLAVE_NUM];
    int slavesCreated;
}fileDesc;

void createSlaves(fileDesc* fDescPtr, int slaveNum);


int main(int argc, char const *argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);
    int tasksAmount = argc -1;
    if (tasksAmount > 0)
    {
        
        // creo 5 esclavos vacio
        fileDesc fDescPtr;
        
        if(tasksAmount>=SLAVE_NUM){
            createSlaves(&fDescPtr, SLAVE_NUM);
        }else{
            createSlaves(&fDescPtr, tasksAmount);
        }
        
        // trato de obtener el bloque o lo creo en caso de q no exista
        int id;
        char *sharedMemBlock = createMemoryBlock(&id);
        
        if (sharedMemBlock == NULL)
        {
            return ERROR;
        }

        printf("%d\n", id);
        
        char *initialSharedMemoryPosition=sharedMemBlock;
        sem_t* sem=joinSemaphore();


        int j=1;
        int resueltos=0;
        for(int i=0 ; j<=tasksAmount && j<=INIT_TASK*fDescPtr.slavesCreated ; j++){
            if(write(fDescPtr.appToSlave[i][1],argv[j], strlen(argv[j])) == ERROR || write(fDescPtr.appToSlave[i][1],"\n", 1)==ERROR) {
                perror("Write Error");
            }
    
            fDescPtr.tasksLeft[i]++;
            i++;
            if(i==fDescPtr.slavesCreated){
                i=0;
            }
        }
        
       
        //Creo mi fd_set para usar en el select
        fd_set miSet;        
        char solution[MAX_SIZE] = {0};
        
        while(resueltos<tasksAmount){        //hay que ver que logica usamos para estar viendo los resultados
            FD_ZERO(&miSet);
            for(int i=0; i<fDescPtr.slavesCreated;i++){
                FD_SET(fDescPtr.slaveToApp[i][0],&miSet);              //Este es el fd al que llega la salida de los escalvos
            }

            int res;
            if((res=select(fDescPtr.highFd + 1, &miSet, NULL ,NULL,NULL)) == ERROR){
                return ERROR;
            }
            
            // Aca deberia de estar la parte de lectura de los escalvos y entregarles mas trabajo

            //Una vez salta el select vamos probando uno por uno hasta ver cual tenemos lectura
            for(int i=0; res>0 && i<fDescPtr.slavesCreated;i++){     
                
                if (FD_ISSET(fDescPtr.slaveToApp[i][0],&miSet)){ //Hay escritura
                    resueltos++;
                    read(fDescPtr.slaveToApp[i][0],solution,MAX_SIZE);
                    fDescPtr.tasksLeft[i]--;
                    sharedMemBlock += sprintf(sharedMemBlock,"%s\n",solution)+1;
                    sem_post(sem);
                    if(sharedMemBlock-initialSharedMemoryPosition>=BLOCK_SIZE){
                        sharedMemBlock += sprintf(sharedMemBlock,"%c",'@');
                        sem_post(sem);
                        exit(3);                //Se paso de mi memoria maxima de trabajo
                    }
                }
                
                //Agrego los tasks faltantes
                if(fDescPtr.tasksLeft[i]==0 && j<=tasksAmount ){
                    write(fDescPtr.appToSlave[i][1],argv[j],strlen(argv[j]));
                    write(fDescPtr.appToSlave[i][1],"\n", 1);
                    fDescPtr.tasksLeft[i]++;
                    j++;
                }
                              
            }
            
        }
        sharedMemBlock += sprintf(sharedMemBlock,"%c",'@');
        sem_post(sem);

        for(int i=0; i<fDescPtr.slavesCreated;i++){
            close(fDescPtr.slaveToApp[i][0]);
            close(fDescPtr.appToSlave[i][1]);
        }
        // Termine y salgo del bloque de memoria
        leaveSemaphore(sem);
        leaveMemoryBlock(initialSharedMemoryPosition);
        return 1;
     
    }

    return 0;
}

// Creo esclavos y modifico los punteros ingresados
void createSlaves(fileDesc* fDescPtr, int slaveNum)
{
    pid_t pid;
    fDescPtr->highFd=0;
    for (int i=0; i < slaveNum ; i++)
    {

        if (pipe(fDescPtr->slaveToApp[i]) < 0 || pipe(fDescPtr->appToSlave[i]) < 0)
        {
            perror("Pipe Error");
        }


        fDescPtr->tasksLeft[i]=0;
        
        if(fDescPtr->highFd<fDescPtr->slaveToApp[i][0]) {
            fDescPtr->highFd=fDescPtr->slaveToApp[i][0];
        }

        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "fork() failed!\n"); // Analizar q hacer en este caso
        }
        else if (pid == 0)
        {
            close(fDescPtr->slaveToApp[i][0]);             // cierro el read del esclavo
            close(fDescPtr->appToSlave[i][1]);             //cierro el write del app
            dup2(fDescPtr->slaveToApp[i][1],STDOUT_FILENO); //redirijo el write
            dup2(fDescPtr->appToSlave[i][0], STDIN_FILENO); //redirijo el read
            close(fDescPtr->slaveToApp[i][1]);
            close(fDescPtr->appToSlave[i][0]);
            for(int m=0;m<i;m++){
                close(fDescPtr->slaveToApp[m][0]);            // cierro el read del esclavo
                close(fDescPtr->appToSlave[m][1]);             //cierro el write del app
            }

            char *argv_for_program[] = { "esclavos.exe", NULL };
            int ex = execv("esclavos.exe", argv_for_program);
            perror("Fallo el execv");
            exit(ex);
        }
        else
        {
            close(fDescPtr->slaveToApp[i][1]); 
            close(fDescPtr->appToSlave[i][0]);
        }
        
    }
    fDescPtr->slavesCreated=slaveNum;
    
    return;
}
