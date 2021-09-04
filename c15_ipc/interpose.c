#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>

#define BUFSIZE 4096
int main(){
    FILE* f1 = popen("./tolower","w");
    if(f1 == NULL){
        printf("popen failed\n");
        exit(1);
    }
    char line[BUFSIZ];
    while (1){
        if(fputs("prompt>", stdout) == EOF){
            printf("fputs failed\n");
            exit(1);
        }
        fflush(stdout);
        if(fgets(line, BUFSIZ, stdin) == NULL){
            break;
        }
        if(fputs(line, f1) == EOF){
            printf("fputs failed\n");
            exit(1);
        }
    }
    pclose(f1);
}