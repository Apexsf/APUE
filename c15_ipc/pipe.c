#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(){
    int n;
    int fd[2];
    pid_t pid;
    char line[128];

    if(pipe(fd) < 0) exit(1);
    if((pid  = fork()) < 0) exit(1);
    else if(pid > 0){
        close(fd[0]);
        write(fd[1],"hello world\n", 12);
    } else{
        close(fd[1]);
        n = read(fd[0], line, 128);
        write(STDOUT_FILENO, line, n);
    }
    exit(0);
}