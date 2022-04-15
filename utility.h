#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H
//Se debe linkeditar con  -lpthread -lrt
#include <semaphore.h>
#include <stdbool.h>


#define SEM_ESCRITURA "/mySemaphore"
#define BLOCK_SIZE 4096
#define MAX_SIZE 1024
#define FILENAME "app.c"

int getSharedBlock(char* filename, int size);
sem_t * joinSemaphore();
int leaveSemaphore(sem_t * sem);
int terminateSemaphore(sem_t * sem);
char * joinMemoryBlock( char* filename, int size);
bool leaveMemoryBlock(char * id);
bool destroyMemoryBlock(int id);
#endif