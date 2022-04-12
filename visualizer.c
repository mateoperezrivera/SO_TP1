#include <stdio.h>
#include "utility.h"

// _IONBF
// No buffering − No buffer is used. Each I/O operation is written as soon as possible. The buffer and size parameters are ignored.

int main(int argc, char const *argv[])
{   
    char * sharedMemBlock = joinMemoryBlock(FILENAME, BLOCK_SIZE);
    if (sharedMemBlock == NULL){
        //ERROR
        return -1;
    }

    //ACA VA LO Q SEA Q TENGA Q HACER LA VISTA

    leaveMemoryBlock(sharedMemBlock);
    return 0;
}