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

int main(int argc, char const *argv[])
{

    if (argc > 1)
    { // Tipo si por lo menos hay uno
        FILE *file;

        //creo 5 esclavos vacios
        int pipeFd[SLAVE_NUM][2][2];
        for(int i=0 ; i<SLAVE_NUM ; i++){
            // int ** slaveFd = createSlave();            
            pipeFd[i]=createSlave();
            // for(int j=0; j<2 ; j++){
            //     for(int k=0 ; k<2 ;k++){
            //         pipeFd[i][j][k] = slaveFd[j][k];
            //     }
            // }
            
        }

        //[ [ [pipe1.0,[r,w]], [pipe1.1,[r,w]] ] ]

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

                //si ya use todos vuelvo a empezar
                if(j==SLAVE_NUM){
                    j=0;
                }

                //paso por el pipe de escritura los argumentos                
                write(pipeFd[j++][1] , argv[i], strlen(argv[i]));

                //Cuando hagamos vista tenemos q usar el pipe de lectura

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
int ** createSlave(){
    pid_t pid;

    int pipes[2][2];
    int pipe1[2];
    int pipe2[2];

    //[[pipe1, [r,w]],[pipe2, [r,w]]]

    pid = fork();
    if(pid < 0){
        fprintf(stderr, "fork() failed!\n"); // Analizar q hacer en este caso
    }else if (pid == 0){
        //Me quedo esperando para correr el eclavos.c
        close(pipe1[0]);
        close(pipe2[1]);
        dup2(pipe1[1],STDOUT_FILENO);
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe1[1]);
        close(pipe2[0]);
        execv("esclavos.c",NULL);        
    }else{
        //creo el pipe q me va a comunicar con mi hijo
        // pipe(pipe1);
        if (pipe(pipe1) < 0){
            // PIPE ERROR
        }
        close(pipe1[1]);
        close(pipe2[0]);
    }

    pipes[0] = pipe1; //leyendo
    pipes[1] = pipe2; //escribiendo

    return pipes; //devuelvo el fd
}

