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
    // minisat [options] [INPUT-FILE [RESULT-OUTPUT-FILE]]
    char *inputProcesado; // Un path de respuesta para darle a minisat
    int inputBytes;
    pid_t ret;

    while ((inputBytes = read(STDIN_FILENO, input, INPUT_SIZE)) > 0)
    {
        ret = fork();
        if (ret < 0)
        {
            fprintf(stderr, "fork() failed!\n"); // Analizar q hacer en este caso
        }
        else if (ret == 0)
        {
            execv(./ minisat input input, 2); // en minisat poner el path, voy a hacer q el output sea el mismo que el input
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

            pipe(pipe1);
            if (pipe(pipe1) < 0)
            {
                // PIPE ERROR
            }

            ret = fork();
            f(ret < 0)
            {
                fprintf(stderr, "fork() failed!\n"); // Analizar q hacer en este caso
            }
            else if (ret == 0)
            {
                execv(./ egrep - o - e "Number of.*[0-9]\+" - e "CPU time.*" - e ".*SATISFIABLE" input, 8); // en egrep poner el path, voy a hacer q el output sea el mismo que el input
            }
            else
        }
    }
}