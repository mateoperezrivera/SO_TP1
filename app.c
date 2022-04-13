#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "utility.h"
#define SIZE 256
#define INPUT_SIZE 512
#define SLAVE_NUM 5

int main(int argc, char const *argv[])
{

    if (argc > 1)
    {

        // creo 5 esclavos vacios
        int *writeFd[SLAVE_NUM];
        int *readFd[SLAVE_NUM];
        createSlaves(writeFd, readFd);

        int j = 0;
        for (int i = 1; i < argc; i++)
        {

            // trato de obtener el bloque o lo creo en caso de q no exista
            char *sharedMemBlock = joinMemoryBlock(FILENAME, BLOCK_SIZE);
            // Trato de obtener el semaforo o lo creo en caso de que no exista
            sem_t *sem = joinSemaphore();
            if (sharedMemBlock == NULL)
            {
                // ERROR
                return -1;
            }

            sem_wait(sem);
            // Aca deberia de estar la parte de lectura de los escalvos y entregarles mas trabajo
            sem_post(sem);
            // Cuando hagamos vista tenemos q usar el pipe de lectura

            // Termine y salgo del bloque de memoria
            leaveMemoryBlock(sharedMemBlock);
            return 1;
        }
    }
    // No se bien donde destruirlo
    // destroyMemoryBlock(FILENAME);
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
            execv("esclavos.c", NULL);
        }
        else
        {
            close(readFd[i][1]);
            close(writeFd[i][0]);
        }
    }
    return;
}
