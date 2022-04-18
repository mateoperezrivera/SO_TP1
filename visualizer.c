// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>

int printElements(char* shm, int * hayElem);

int main(int argc, char const *argv[])
{   setvbuf(stdout, NULL, _IONBF, 0);
    int id;

    if (argc>1){
        id=atoi(argv[1]);
    }else{
        scanf("%d",&id);
    }

    sem_t *sem = joinSemaphore();
    ;
    char * sharedMemBlock = joinMemoryBlock(id);
    char *initialSharedMemBlock=sharedMemBlock;
    if (sharedMemBlock == NULL){
        return ERROR;
    }
    
 
    int hayElementos=1;
    while(hayElementos){
        sem_wait(sem);
        sharedMemBlock+= printElements(sharedMemBlock, &hayElementos);        
    }    

    //Termine
    leaveSemaphore(sem);
    terminateSemaphore(sem);
    leaveMemoryBlock(initialSharedMemBlock);
    destroyMemoryBlock(id);
    return 0;
}

int printElements(char* shm, int * hayElem){
    char result[MAX_SIZE+1];
    int index=0;
    while(shm[index]!='\n' && shm[index]!='@' ){
        result[index]=shm[index];
        index++;
    }
    if(shm[index]=='@'){
        *hayElem=0;
        return 1;
    }
    result[index++]=0;
    printf("%s\n",result);
    return index+1;
}