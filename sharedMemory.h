#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h>

#define BLOCK_SIZE 4096
#define FILENAME "app.c"

 
int joinMemoryBlock( char* filename, int size);
bool leaveMemoryBlock(char * block);
bool destroyMemoryBlock(char * filename);

#endif