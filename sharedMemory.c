#include "sharedMemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ERROR (-1)

//Toma el filename y el tamanio y devuelve el id del bloque
static int getSharedBlock(char* filename, int size){
    key_t key;

    key = ftok(filename,0);
    if(key == ERROR){
        return ERROR;
    }

    //obtengo el id o lo creo en caso de q no exista
    return shmget(key, size, 0644 | IPC_CREAT);
}

int joinMemoryBlock( char* filename, int size){
    int id = getSharedBlock(filename, size);

    if(id==ERROR){
        return null;
    }

    //mapeo el bloque al id devolviendome un puntero al mismo
    char * result=shmat(id, NULL, 0);
    if(result == (char *)ERROR){
        return NULL
    }

    return result;
}


//Me voy del bloque
bool leaveMemoryBlock(char * block){
    return (shmdt(block) != ERROR)
}

//destruyo el bloque
bool destroyMemoryBlock(char * filename){
    int id = getSharedBlock(filename,0);

    if(id== ERROR){
        return NULL:
    }

    return (shmctl(id, IPC_RMID, NULL) != ERROR);
}