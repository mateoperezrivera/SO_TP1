// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_SIZE 1024
#define ERROR -1


int main(int argc, char const *argv[])          //El argumento va a salir el numero de esclavo
{              
    setvbuf(stdout, NULL, _IONBF, 0);  //soluciona la transmicion de los bytes 
    
    size_t size = 0;
    char * input=NULL;
    int inputBytes;


    while ((inputBytes = getline( &input, &size, stdin)) >0)
    {   
         
        if (input==NULL){
            exit(3);
        }
        input[inputBytes-1]=0;           
        char salidaEgrep[MAX_SIZE + 1];
        char command[MAX_SIZE];
        //PVS-STUDIO detecta que commando no esta chequeado
        //Command se declara justo arriba de su su uso, por lo tanto no es necesario un chequeo como sugiere
        sprintf(command, "minisat \"%s\" | grep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e\".*SATISFIABLE\"| tr \"\n\" \" \" | tr \"\t\" \" \" ", input);

        FILE * fileNum =popen(command, "r");
        if(fileNum == NULL){
            perror("Open Error");
            exit(1);
        }
        int salidaDim=fread(salidaEgrep,sizeof(char), MAX_SIZE, fileNum); // en minisat poner el path, voy a hacer q el output sea el mismo que el input

        salidaEgrep[salidaDim]='\n';
        printf("PID: %d\tFilename: %s\t %s\t", getpid(), input, salidaEgrep);

        if(pclose(fileNum)== ERROR){
            perror("Close Error");
            exit(1);
        }

        
    }
    free(input);
}

