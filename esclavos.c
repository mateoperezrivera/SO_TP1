#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define INPUT_SIZE 512

// The popen() function shall execute the command specified by the string command.
// It shall create a pipe between the calling program and the executed command, and
// shall return a pointer to a stream that can be used to either read from or write to the pipe.

int main()
{
    char input[INPUT_SIZE];
    char output[INPUT_SIZE];
    // minisat [options] [INPUT-FILE [RESULT-OUTPUT-FILE]]
    char *inputProcesado; // Un path de respuesta para darle a minisat
    int inputBytes;
    pid_t ret;
    dup2(0,6);      //Me guardo en el fd 6 la IN del padre para recuperarla despues?????         
    while ((inputBytes = read(STDIN_FILENO, input, INPUT_SIZE)) > 0)
    {
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
                FILE* file=fopen(output,"w");
                //Mi idea es guardar en el  buffer al q apunta el file el resulado del egrep
                read(STDIN_FILENO, file, file->_bufsiz);      //Recibir por el R del pipe, el contenido de output procesado por egrep
                //Esto supuestamente me dejaria en output, la salida del egrep
                close(pipe1[0]);
                fclose(pipe1[1]);
                fclose(output);
                dup2(6,0);               //Recupero el fd con la salida del pipe del padre
                printf("%s",output);//Printeo a salida estandard
            }
        
        
    }
}

