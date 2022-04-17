#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_SIZE 1024
#define ERROR -1
#define TAB '\t'



// The popen() function shall execute the command specified by the string command.
// It shall create a pipe between the calling program and the executed command, and
// shall return a pointer to a stream that can be used to either read from or write to the pipe.
// Vamos a hacer que puedan recibir multiples paths, separados por un pipe
int main(void)          //El argumento va a salir el numero de esclavo
{              
    size_t size = MAX_SIZE;
    char * input;
    char salidaEgrep[MAX_SIZE];
    int inputBytes;
    

    while ((inputBytes = getline( &input, &size, stdin)) >0)
    {   
        input[inputBytes-1]=0;
        char command[MAX_SIZE];
        sprintf(command, "minisat %s | grep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e\".*SATISFIABLE\" | tr \"\n\" \"\t\"", input);
       
        FILE * fileNum;
        if((fileNum=popen(command, "r")) == NULL){
            return ERROR;
        }
        int salidaDim=fread(salidaEgrep,sizeof(char), MAX_SIZE, fileNum); // en minisat poner el path, voy a hacer q el output sea el mismo que el input
              
        salidaEgrep[salidaDim]=0;

        printf("PID: %d\tFilename: %s\t %s", getpid(), input, salidaEgrep);

        if(pclose(fileNum)== ERROR){
            return ERROR;
        }
        
    }
    
}

