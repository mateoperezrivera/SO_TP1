#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sharedMemory.h"
#define MAX_SIZE 512
static char resuelto[] = "resuelto.txt";
static char esclavo[] = "Soy el escalvo ";

// The popen() function shall execute the command specified by the string command.
// It shall create a pipe between the calling program and the executed command, and
// shall return a pointer to a stream that can be used to either read from or write to the pipe.

int main(int argc, char const *argv[])          //El argumento va a salir el numero de esclavo
{              
    char nroEsclavo= *argv[1];
    char input[MAX_SIZE];
    char output[MAX_SIZE];
    char salidaEgrep[MAX_SIZE];
    // minisat [options] [INPUT-FILE [RESULT-OUTPUT-FILE]]
    char *inputProcesado; // Un path de respuesta para darle a minisat
    int inputBytes;
    pid_t ret;
    dup2(0,6);      //Me guardo en el fd 6 la IN del padre para recuperarla despues?????       
    FILE* fptr;    //Puntero donde voy a guardar el nuevo archivo a abrir  
    while ((inputBytes = read(STDIN_FILENO, input, MAX_SIZE)) > 0)
    {
        strcpy(output,input);
        int i=0;
        for(;i<MAX_SIZE && output[i]!=0;i++);
        int j=0;
        while(resuelto[j]!=0){    //Cambio el path para generar el archivo resultado
            output[i+j]=resuelto[j];
            j++;
        }

        fptr=fopen(output,"w");
        fclose(output);
        ret = fork();
        if (ret < 0)
        {
            fprintf(stderr, "fork() failed!\n"); // Analizar q hacer en este caso
        }
        else if (ret == 0)
        {
            execv("./minisat input output", 2); // en minisat poner el path, voy a hacer q el output sea el mismo que el input
        }
        else
        {
            if (wait() == 0)
            { // Aca va el valor de retorno si miniset falla
              // hacer algo en caso fallo
            }

            // Creo el pipe de hijo a padre ya que egrep retorna por salida estandard y necesito esa respúesta
            pid_t pid;
            int pipe1[2];

            pipe(pipe1);                //Agrega fd 3 y 4
            if (pipe(pipe1) < 0)
            {
                // PIPE ERROR
            }


            ret = fork();
            if(ret < 0)
            {
                fprintf(stderr, "fork() failed!\n"); // Analizar q hacer en este caso
            }
            else if (ret == 0)
            {
                close(1);       //Cierro la OUT_STD del hijo
                dup(3);          //Copia el fd 3 en el fd mas bajo habilitado
                close(3);
                close(4);
                dup2(1,2);
                execv("/egrep - o - e \"Number of.*[0-9]\+\" - e \"CPU time.*\" - e \".*SATISFIABLE\" output", 8); // en egrep poner el path, voy a hacer q el output sea el mismo que el input
            }
            else{
                close(0);        //Cierro la entrada standard
                dup(4);          //Copio el Read del pipe al STD_IN
                close(4);
                close(3);
                //Guardo en salidaEgrep, el resultado de egrep
                int cantidadLeida = read(STDIN_FILENO, salidaEgrep ,MAX_SIZE);      
                salidaEgrep[cantidadLeida++]='\n';
                int k=0;
                while(esclavo[k]!=0){
                    salidaEgrep[cantidadLeida+k]=esclavo[k++];
                }
                //Agrego a la salida de egrep, la identificacion del esclavo
                salidaEgrep[cantidadLeida+k]=nroEsclavo,k++;
                salidaEgrep[cantidadLeida+k]='\n',k++; 
                //Agrego a la salida de egrep, 
                salidaEgrep[k++]='\n';   //
                k+=cantidadLeida;
                int l=0;
                //Agrego el nombre del archivo procesado
                while(input[l]!=0){
                    salidaEgrep[k+l]=input[l];
                    l++;
                }
                salidaEgrep[k+l]=0;

                close(pipe1[0]);
                fclose(pipe1[1]);
                fclose(output);
                dup2(6,0);               //Recupero el fd con la salida del pipe del padre
                printf("%s",salidaEgrep);//Printeo a salida estandard todo el resultado
            }
        
        
    }

}

