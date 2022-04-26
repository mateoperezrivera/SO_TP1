#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utility.h"

int main(int argc, char const *argv[]){
    setvbuf(stdout, NULL, _IONBF, 0);  //soluciona la transmicion de los bytes 
    
    char input[MAX_SIZE]={0};
    int inputBytes;
    int fd = open(PIPENAME, O_RDONLY);
    int hayElementos=1
    while(hayElementos){
    //read from pipename
        read(fd, input, MAX_SIZE);

        char * answer = strtok(input, "\n");
        if(answer != NULL){
            input += sprintf(input,"%s\n",solution)+1;
            if(strcasestr("@"))
                hayElementos=0;
            answer = strtok(NULL, "\n");

        }    
    }

    printf("%s\n", input);

    close(fd);
}