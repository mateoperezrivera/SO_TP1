#include "utility.h"
#include <stdio.h>
#define ERROR -1

// _IONBF
// No buffering − No buffer is used. Each I/O operation is written as soon as possible. The buffer and size parameters are ignored.

void printElements(char** shm, int * hayElem);

int main(int argc, char const *argv[])
{   
    sem_t *sem = joinSemaphore();
    char * sharedMemBlock = joinMemoryBlock(FILENAME, BLOCK_SIZE);
    char *sharedMemBlockSeguro=sharedMemBlock;

    if (sharedMemBlock == NULL){
        return ERROR;
    }
    
    int idBlock = getSharedBlock(FILENAME, BLOCK_SIZE); //talvez aca en vez de pasarle el filenam y size podriamos pasar sharedMemBlock
    if(idBlock==0){
        return ERROR;
    }
 
    int hayElementos=1;
    while(hayElementos){
        sem_wait(sem);
        printElements(&sharedMemBlock, &hayElementos);
        
    }    

    //Termine
    leaveSemaphore(sem);
    terminateSemaphore(sem);
    leaveMemoryBlock(sharedMemBlockSeguro);
    destroyMemoryBlock(idBlock);
    return 0;
}

void printElements(char** shm, int * hayElem){
    char result[MAX_SIZE];
    int index=0;
    while(**shm!='\n' && **shm!=EOF){
        result[index++]=*(*shm++);
    }
    if(**shm==EOF){
        *hayElem=0;
    }
    printf("%s",result);
}