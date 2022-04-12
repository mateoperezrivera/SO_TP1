#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H
//Se debe linkeditar con  -lpthread -lrt
#include <semaphore.h>
#include <stdbool.h>


#define SEM_ESCRITURA "/mySemaphore"
#define BLOCK_SIZE 4096
#define FILENAME "app.c"

sem_t * joinSemaphore();
int joinMemoryBlock( char* filename, int size);
bool leaveMemoryBlock(char * block);
bool destroyMemoryBlock(char * filename);

#endif