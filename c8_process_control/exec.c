#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
char* envi_init[] = {"USER=unknown","PATH=tmp",NULL}; //the envi list most ended with a NULL

int main(){
    pid_t pid;
    int wstatus;
    if((pid = fork()) < 0){
        printf("fork failure!\n");
        exit(1);
    }else if (pid == 0){
        if(execle("./myecho","myecho","99","op",(char*)(0),envi_init) < 0){
            printf("execle error!\n");
            exit(1);
        }
    }
    if(wait(&wstatus) != pid){
        printf("wait error!\n");
        exit(1);
    }
    exit(0);
}