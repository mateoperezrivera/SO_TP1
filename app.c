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
#define ERROR -1
static char slaveString[]="\tSlave ID: ";

typedef struct fileDescriptors{
    int writeFd[SLAVE_NUM][2];
    int readFd[SLAVE_NUM][2];
    //Esto lo tenemos q implementar para cuando agregamos la primer vez, darle tareas solo cuando termine
    int tasksLeft[SLAVE_NUM];
}fileDesc;

void createSlaves(fileDesc* fDescPtr);




int main(int argc, char const *argv[])
{
    printf("solo arranca");
    if (argc > 1)
    {
        
        printf("Arranco por ");
        // creo 5 esclavos vacio
        fileDesc* fDescPtr;
        createSlaves(fDescPtr);
        /*printf("Creo los esclavos");

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
        
        //Creo mi fd_set para usar en el select
        fd_set* miSet=NULL;
        for(int i=0; i<SLAVE_NUM;i++){
            FD_SET(fDescPtr->readFd[i][0],miSet);               //Este es el fd al que llega la salida de los escalvos
        }

        char solution[MAX_SIZE];
        int j=0;
        for(int i=0;j<argc/10;j++){
            write(fDescPtr->writeFd[i++][1],argv[j], strlen(argv[j]));
            fDescPtr->tasksLeft[i]++;
            if (i>=SLAVE_NUM)
                i=0;
        }
        
        while(j<argc){        //hay que ver que logica usamos para estar viendo los resultados
            if(select(SLAVE_NUM,miSet,NULL,NULL,NULL)==-1)
                //ERROR
                return -1;

            
            // Aca deberia de estar la parte de lectura de los escalvos y entregarles mas trabajo

            //Una vez salta el select vamos probando uno por uno hasta ver cual tenemos lectura
            for(int i=0; i<SLAVE_NUM;i++){
                if (FD_ISSET(readFd[i][0],miSet)!=0){//Hay escritura
                    int bytesRead=read(STDIN_FILENO,solution,MAX_SIZE);
                    fDescPtr->tasksLeft[i]--;
                    int k=0;
                    for(;k<STRING_SIZE;k++){
                        solution[bytesRead+k]=slaveString[k];
                    }
                    bytesRead+=k;
                    solution[bytesRead++]='0'+i;        //Le agrego el numero de esclavo
                    solution[bytesRead++]='\n';
                    solution[bytesRead++]='\0';

                    printf("%s",solution);//Printf solo de prueba
         //         sharedMemBlock += sprintf(sharedMemBlock,"%s",solution)+1;
         //         if(fDescPtr->tasksLeft[i]==0){
         //           write(fDescPtr->writeFd[i][1],argv[j],strlen(argv[j]));
         //           fDescPtr->tasksLeft[i]++;
         //         }
         //         sem_post(sem);
                    j++;
                    
                }
            }
            
        }*/
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

    for (int i = 0; i < 1; i++)
    {
        if (pipe(fDescPtr->readFd[i]) < 0)
        {
            printf("Father Pipe Error");
        }
        
        if (pipe(fDescPtr->writeFd[i]) < 0)
        {
            printf("Son Pipe Error");
        }
        fDescPtr->tasksLeft[i]=0;
        printf("Hay fd");
        
        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "fork() failed!\n"); // Analizar q hacer en este caso
        }
        else if (pid == 0)
        {
            close(readFd[i][0]);            // el read del esclaco i
            close(writeFd[i][1]);
            dup2(readFd[i][1], STDOUT_FILENO);
            dup2(writeFd[i][0], STDIN_FILENO);
            close(readFd[i][1]);
            close(writeFd[i][0]);
            char *argv_for_program[] = { "esclavos.c", NULL };
            execv("esclavos.c", argv_for_program);
        }
        else
        {
            close(readFd[i][1]);
            close(writeFd[i][0]);
        }
        */
    }
    
    return;
}
