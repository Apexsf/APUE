#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>



char* buf = NULL;

int main(){
    buf = malloc(4096);
    pid_t pid;
 
    if((pid = fork()) < 0) exit(1);
    else if(pid == 0){
        buf[0] = 'd';
        buf[1] = 'l';
        buf[2] = '\0';
        printf("%s\n", buf);
    }
    if(waitpid(pid, NULL,0) < 0) exit(1);
    printf("%s\n", buf);

    if((pid = fork()) < 0) exit(1);
    else if(pid == 0){
        execl("/bin/sh","sh","-c", "ls -l", (char*)0);
        printf("fadsfdas\n");
        exit(127);
    }
    if(waitpid(pid, NULL,0) < 0) exit(1);
    printf("%s\n", buf);

    return;

}