#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "sharedMemory.h"
#define SIZE 256
#define INPUT_SIZE 512
#define SLAVE_NUM 5

//./app ./path1 ./path2
void slave(char* argumento,int numSlave);

int main(int argc, char const *argv[])
{

    if (argc > 1)
    { // Tipo si por lo menos hay uno
        FILE *file;

        //creo 5 esclavos vacios
        int * pipeFd[SLAVE_NUM][2];
        for(int i=0 ; i<SLAVE_NUM ; i++){
            int * slaveFd = createSlave();            
            pipeFd[i][0] = slaveFd[0];
            pipeFd[i][1] = slaveFd[1];
        }

        int j=0;
        for (int i = 1; i < argc; i++)
        {
            file = fopen(argv[i], "r");
            if (file)
            {
                //trato de obtener el bloque o lo creo en caso de q no exista
                char * sharedMemBlock = joinMemoryBlock(FILENAME, BLOCK_SIZE);
                if (sharedMemBlock == NULL){
                    //ERROR
                    return -1;
                }

                if(j==SLAVE_NUM){
                    j=0;
                }
                
                slave(argv[i],pipeFd[j++]);
                read(STDIN_FILENO, buffer1[i], INPUT_SIZE); //guardo en buffer por orden de llegada                    

                //Termine y salgo del bloque de memoria
                leaveMemoryBlock(sharedMemBlock);
                return 1;

            }
        }
    }
    //No se bien donde destruirlo
    //destroyMemoryBlock(FILENAME);
    return 0;
}

//Creo el esclavo y retorno su fd
int * createSlave(){
    pid_t pid;
               
    pid = fork();
    if(pid < 0){
        fprintf(stderr, "fork() failed!\n"); // Analizar q hacer en este caso
    }else if (pid == 0){
        //Me quedo esperando para correr el eclavos.c
        execv("esclavos",NULL);        
    }else{
        //creo el pipe q me va a comunicar con mi hijo
        int pipe1[2], readbytes;
       
        pipe(pipe1);
        if (pipe(pipe1) < 0){
            // PIPE ERROR
        }
    }

    return pipe1; //devuelvo el fd
}

void slave(char* argumento,int * fd){
    
        fd[1] = argumento;
        pipe(fd);
        if (pipe(fd) < 0){
            // PIPE ERROR
        }

}
