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
    size_t size = MAX_SIZE;
    char * fullInput;
    char salidaEgrep[MAX_SIZE];
    // minisat [options] [INPUT-FILE [RESULT-OUTPUT-FILE]]
    int inputBytes;
    

    int completePathIndex;
    while ((inputBytes = getline( &fullInput, &size, STDIN_FILENO)) >0)
    {   
        completePathIndex=0;
        while (completePathIndex!=inputBytes){
            char input[MAX_SIZE];       
            getPath(input,fullInput,&completePathIndex);
            FILE * fileNum =popen("./minisat input | egrep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\" | tr \"\n\" \"\t\" | tr \" \" \"\t\" | tr -d \"\t\"", "r");
            int cantidadLeida=read(fileno(fileNum), salidaEgrep ,MAX_SIZE); // en minisat poner el path, voy a hacer q el output sea el mismo que el input
            
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

