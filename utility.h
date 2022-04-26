#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H
//Se debe linkeditar con  -lpthread -lrt
#include <semaphore.h>
#include <stdbool.h>


#define SEM_ESCRITURA "/mySemaphore"
#define BLOCK_SIZE 4096
#define MAX_SIZE 1024
#define ERROR -1
#define FILENAME "app.c"
#define PIPENAME "./pipeEsclavo"

sem_t * joinSemaphore();
int leaveSemaphore(sem_t * sem);
int terminateSemaphore(sem_t * sem);
char * createMemoryBlock(int * id);
char * joinMemoryBlock(int id);
bool leaveMemoryBlock(char * block);
bool destroyMemoryBlock(int id);
#endif