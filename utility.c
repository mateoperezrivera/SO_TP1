#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>

sem_t * joinSemaphore(){
    sem_t* semaphore=sem_open(SEM_ESCRITURA, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (semaphore == SEM_FAILED){
        perror("sem_open/producer");
        exit(EXIT_FAILURE);
    } 
    return semaphore;
}

int leaveSemaphore(sem_t * sem){
   return sem_close(sem);
}

int terminateSemaphore(sem_t * sem){
   return sem_unlink(SEM_ESCRITURA);
}


//Toma el filename y el tamanio y devuelve el id del bloque
static int getSharedBlock(){
    //obtengo el id o lo creo en caso de q no exista
    return shmget(IPC_PRIVATE, BLOCK_SIZE, 0644 | IPC_CREAT);
}

char * createMemoryBlock(int *id){
    *id = getSharedBlock();
    if(*id==ERROR){
        return NULL;
    }

    //mapeo el bloque al id devolviendome un puntero al mismo
    char * result=shmat(*id, NULL, 0);
    if(result == (char *)ERROR){
        return NULL;
    }

    return result;
}

char * joinMemoryBlock(int id){
    //mapeo el bloque al id devolviendome un puntero al mismo

    char * result=shmat(id, NULL, 0);
    if(result == (char *)ERROR){
        return NULL;
    }

    return result;
}

//Me voy del bloque
bool leaveMemoryBlock(char * block){
    return (shmdt(block) != ERROR);
}

//destruyo el bloque
bool destroyMemoryBlock(int id){
    return (shmctl(id, IPC_RMID, NULL) != ERROR);
}



