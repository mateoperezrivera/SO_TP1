#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H
//Se debe linkeditar con  -lpthread -lrt
#include <semaphore.h>
#include <stdbool.h>


#define SEM_ESCRITURA "/mySemaphore"
#define BLOCK_SIZE 4096
#define MAX_SIZE 1024
#define FILENAME "app.c"

sem_t * joinSemaphore();
void leaveSemaphore(sem);
void destroySemaphore(sem);
char * joinMemoryBlock( char* filename, int size);
bool leaveMemoryBlock(int id);

#endif