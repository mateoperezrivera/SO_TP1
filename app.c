#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#define SIZE 256
#define INPUT_SIZE 512

//./app ./path1 ./path2

int main(int argc, char const *argv[])
{

    if (argc > 1)
    { // Tipo si por lo menos hay uno
        FILE *file;
        
        for (int i = 1; i < argc; i++)
        {
            file = fopen(argv[i], "r");
            if (file)
            {
                // crear x esclavos y hacer dos pipe para cada uno, uno de idaa y otro de vuelta
                // LA CONSIGNA DICE QUE L BUFFER USA SHARED MEMORY Y ES UNO
                // LA CONSIGNA DICE QUE SE HABLA A LOS ESCLAVOS POR PIPES
                // primero intentemos que funcione con 1 esclavo
                // Despues hacemos un vector de pipes para controlar todos los esclavos
                pid_t pid;
               

                pid = fork();
                if(pid < 0)
                {
                    fprintf(stderr, "fork() failed!\n"); // Analizar q hacer en este caso
                }
                else if (pid == 0)
                {
                    //si esta libre le tengo q pasar otro archivo
                    
                }else{
                    //creo el pipe q me va a comunicar con mi hijo
                    int pipe1[2], readbytes;
                    char buffer1[SIZE];
                    pipe1[1] = argv[i];
                    pipe(pipe1);
                    if (pipe(pipe1) < 0)
                    {
                        // PIPE ERROR
                    }

                // DEBE recibir el resultado del procesamiento de cada archivo y DEBE agregarlo a un
                // buffer POR ORDEN DE LLEGADA.
                    read(STDIN_FILENO, buffer1[i], INPUT_SIZE); //guardo en buffer por orden de llegada
                }


                int pipe2[2], readbytes;

                pipe(pipe2);
                if (pipe(pipe2) < 0)
                {
                    // PIPE ERROR
                }
                if (fork() == 0)
                {
                    // hijo
                }

                // mira encontre esto de pipes q maybe nos sirve
                //  The popen() function shall execute the command specified by the string command.
                // It shall create a pipe between the calling program and the executed command, and
                // shall return a pointer to a stream that can be used to either read from or write to the pipe.
                // te hace el pipe doble de una no? eso entendi creo q si osea te lo hace entre donde esta y el comando no se si es justo lo q necesitamos aca

                return 1;
            }
            return 0;
        }
    }
}
