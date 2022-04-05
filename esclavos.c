#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
    char * input;
    char* inputProcesado;           //Un path de respuesta para darle a minisat
    while(input = read() > 0){
        if (fork()==0){
            exec(./minisat input inputProcesado);
        }else{
            inputProcesado = wait()
        }

    }
}