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
void createSlaves(int **writeFd, int **readFd);




int main(int argc, char const *argv[])
{

    if (argc > 1)
    {


        // creo 5 esclavos vacios
        int *writeFd[SLAVE_NUM];
        int *readFd[SLAVE_NUM];
        createSlaves(writeFd, readFd);

        // trato de obtener el bloque o lo creo en caso de q no exista
        char *sharedMemBlock = joinMemoryBlock(FILENAME, BLOCK_SIZE);
        char *sharedMemBlockSeguro=sharedMemBlock;
        if (sharedMemBlock == NULL)
            {
                // ERROR
                return ERROR;
            }
        // Trato de obtener el semaforo o lo creo en caso de que no exista
        sem_t *sem = joinSemaphore();
        //Creo mi fd_set para usar en el select
        fd_set* miSet=NULL;
        for(int i=0; i<SLAVE_NUM;i++){
            FD_SET(readFd[i][0],miSet);               //Este es el fd al que llega la salida de los escalvos
        }

        char solution[MAX_SIZE];
        int j=0;
        for(int i=0;j<argc/10;j++){
            write(writeFd[i++][1],argv[j], strlen(argv[j]));
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
                    int k=0;
                    for(;k<STRING_SIZE;k++){
                        solution[bytesRead+k]=slaveString[k];
                    }
                    solution[bytesRead+k]='0'+i;        //Le agrego el numero de esclavo
                    solution[bytesRead+k+1]='\n';
                    sharedMemBlock += sprintf(sharedMemBlock,"%s",solution)+1;
                    write(writeFd[i][1],argv[j],strlen(argv[j]));
                    sem_post(sem);
                    j++;
                    
                }
            }
        }
        sharedMemBlock += sprintf(sharedMemBlock,"%c",EOF);
        // Termine y salgo del bloque de memoria
        leaveSemaphore(sem);
        leaveMemoryBlock(sharedMemBlockSeguro);
        return 1;
        
    }

    return 0;
}

// Creo esclavos y modifico los punteros ingresados
void createSlaves(int **writeFd, int **readFd)
{
    pid_t pid;

    for (int i = 0; i < SLAVE_NUM; i++)
    {
        if (pipe(writeFd[i]) < 0)
        {
            // PIPE ERROR
        }
        if (pipe(readFd[i]) < 0)
        {
            // PIPE ERROR
        }

        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "fork() failed!\n"); // Analizar q hacer en este caso
        }
        else if (pid == 0)
        {

            close(readFd[i][0]);
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
    }
    return;
}
