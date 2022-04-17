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
#define STRING_SIZE 11
static char slaveString[]="\tSlave ID: ";

typedef struct fileDescriptors{
    int writeFd[SLAVE_NUM][2];
    int readFd[SLAVE_NUM][2];

    int highFd;
    //Esto lo tenemos q implementar para cuando agregamos la primer vez, darle tareas solo cuando termine
    int tasksLeft[SLAVE_NUM];
}fileDesc;

void createSlaves(fileDesc* fDescPtr);


int main(int argc, char const *argv[])
{
    argc--;
    if (argc >= 1)
    {
        
        // creo 5 esclavos vacio
        fileDesc fDescPtr;
        createSlaves(&fDescPtr);
        
        // trato de obtener el bloque o lo creo en caso de q no exista
        //char *sharedMemBlock = joinMemoryBlock(FILENAME, BLOCK_SIZE);
        //char *sharedMemBlockSeguro=sharedMemBlock;
        // if (sharedMemBlock == NULL)
        //     {
        //         // ERROR
        //         return ERROR;
        //     }

        // Trato de obtener el semaforo o lo creo en caso de que no exista
       // sem_t *sem = joinSemaphore();
        

        int j=0;
        int resueltos=0;
        for(int i=0;j<1+argc/10;j++){
            if(write(fDescPtr.writeFd[i][1],argv[j], strlen(argv[j])) == ERROR || write(fDescPtr.writeFd[i++][1],"\n", 2)== ERROR){
                printf("Write Error\n");
            }            

            fDescPtr.tasksLeft[i]++;
            if (i>=SLAVE_NUM){
                i=0;
            }
               
        }
        
        //Creo mi fd_set para usar en el select
        fd_set miSet;        
        char solution[MAX_SIZE];
        
        while(resueltos<argc){        //hay que ver que logica usamos para estar viendo los resultados
            for(int i=0; i<SLAVE_NUM;i++){
                FD_ZERO(&miSet);
                FD_SET(fDescPtr.readFd[i][0],&miSet);              //Este es el fd al que llega la salida de los escalvos
            
            }

            int res;
            if((res=select(fDescPtr.highFd + 1, &miSet, NULL ,NULL,NULL)) == ERROR){
                return ERROR;
            }
            
            // Aca deberia de estar la parte de lectura de los escalvos y entregarles mas trabajo

            //Una vez salta el select vamos probando uno por uno hasta ver cual tenemos lectura
            for(int i=0; res>0 && i<SLAVE_NUM;i++){     
                
                if (FD_ISSET(fDescPtr.readFd[i][0],&miSet)){ //Hay escritura
                    resueltos++;
                    printf("%d \n",i);
                    int bytesRead=read(fDescPtr.readFd[i][0],solution,MAX_SIZE);
                    printf("%s",solution); //Printf solo de prueba
                   /*fDescPtr.tasksLeft[i]--;
                    int k=0;
                    for(;k<STRING_SIZE;k++){
                        solution[bytesRead+k]=slaveString[k];
                    }
                    bytesRead+=k;
                    solution[bytesRead++]='0'+i;        //Le agrego el numero de esclavo
                    solution[bytesRead++]='\n';
                    solution[bytesRead++]='\0';

                    printf("%s",solution); //Printf solo de prueba
         //         sharedMemBlock += sprintf(sharedMemBlock,"%s",solution)+1;
                    }
                    if(fDescPtr.tasksLeft[i]==0 && j<argc ){
                        write(fDescPtr.writeFd[i][1],argv[j],strlen(argv[j]));
                        fDescPtr.tasksLeft[i]++;
                        //sem_post(sem);
                        j++;
                    }*/
                }
                                    
                
            }
            
        }
        
        //sharedMemBlock += sprintf(sharedMemBlock,"%c",EOF);
        // Termine y salgo del bloque de memoria
        // leaveSemaphore(sem);
        // leaveMemoryBlock(sharedMemBlockSeguro);
        return 1;
     
    }

    return 0;
}

// Creo esclavos y modifico los punteros ingresados
void createSlaves(fileDesc* fDescPtr)
{
    pid_t pid;
    fDescPtr->highFd=0;
    for (int i = 0; i < SLAVE_NUM; i++)
    {
        if (pipe(fDescPtr->readFd[i]) < 0 || pipe(fDescPtr->writeFd[i]) < 0)
        {
            printf("Pipe Error\n");
        }
        
        fDescPtr->tasksLeft[i]=0;
        
        if(fDescPtr->highFd<fDescPtr->readFd[i][0]) {
            fDescPtr->highFd=fDescPtr->readFd[i][0];
        }

        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "fork() failed!\n"); // Analizar q hacer en este caso
        }
        else if (pid == 0)
        {
            close(fDescPtr->readFd[i][0]);            // el read del esclavo i
            close(fDescPtr->writeFd[i][1]);
            dup2(fDescPtr->readFd[i][1], STDOUT_FILENO);
            dup2(fDescPtr->writeFd[i][0], STDIN_FILENO);
            close(fDescPtr->readFd[i][1]);
            close(fDescPtr->writeFd[i][0]);

            char *argv_for_program[] = { "esclavos.c", NULL };
            execv("esclavos.c", argv_for_program);
        }
        else
        {
            close(fDescPtr->readFd[i][1]);
            close(fDescPtr->writeFd[i][0]);
        }
        
    }
    
    return;
}
