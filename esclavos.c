#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LIMITER '|'
#define MAX_SIZE 1024


void getPath(char* newPath,char* completePath,int* index){
    int newPathIndex=0;
    while(completePath[*index]!=LIMITER && completePath[*index]!=EOF){
        newPath[newPathIndex]=completePath[*index];
        newPathIndex++;
        (*index)++;        
    }
    newPath[newPathIndex]=EOF;
    return; 
}

// The popen() function shall execute the command specified by the string command.
// It shall create a pipe between the calling program and the executed command, and
// shall return a pointer to a stream that can be used to either read from or write to the pipe.
// Vamos a hacer que puedan recibir multiples paths, separados por un pipe
int main(void)          //El argumento va a salir el numero de esclavo
{              
    char fullInput[MAX_SIZE];
    char output[MAX_SIZE];
    char salidaEgrep[MAX_SIZE];
    // minisat [options] [INPUT-FILE [RESULT-OUTPUT-FILE]]
    char *inputProcesado; // Un path de respuesta para darle a minisat
    int inputBytes;
    pid_t ret;
    
    dup2(0,6);      //Me guardo en el fd 6 la IN del padre para recuperarla despues?????       
    FILE* fptr;    //Puntero donde voy a guardar el nuevo archivo a abrir  
    int completePathIndex;
    while ((inputBytes = getline( &fullInput,MAX_SIZE, STDIN_FILENO)) >0)
    {   
        completePathIndex=0;
        while (completePathIndex!=inputBytes){
            char input[MAX_SIZE];       
            getPath(input,fullInput,&completePathIndex);

            int cantidadLeida=read(popen("./minisat input | egrep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\" | tr \"\n\" \"\t\" | tr \" \" \"\t\" | tr -d \"\t\"", "r"), salidaEgrep ,MAX_SIZE); // en minisat poner el path, voy a hacer q el output sea el mismo que el input
            
            salidaEgrep[cantidadLeida++]='\t';
        
            int l=0;
            //Agrego el nombre del archivo procesado
            while(input[l]!=0){
                salidaEgrep[cantidadLeida+l]=input[l];
                l++;
            }
    
            //Agrego un enter al final para marcar el final del output, no estoy seguro si es eso o '\0'
            salidaEgrep[cantidadLeida+l]='\n';          

            printf("%s",salidaEgrep);//Printeo a salida estandard todo el resultado
        }
                
    }

}

