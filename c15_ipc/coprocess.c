#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

#define LINESIZE 512

static  void sig_pipe(int);

void sig_pipe(int signo){
    printf("SIGPIPE caught\n");
    exit(1);
}

void perr(char* str){
    printf("%s\n",str);
    exit(1);
}

int main(){
    int n, fd1[2], fd2[2];
    pid_t pid;
    char line[LINESIZE];
    
    if (signal(SIGPIPE, sig_pipe) == SIG_ERR){
        perr("signal error");
    }

    if(pipe(fd1) < 0 || pipe(fd2) < 0){
        perr("pipe error");
    }

    if((pid = fork()) < 0){
        perr("fork error");
    } else if (pid > 0){
        close(fd1[0]);
        close(fd2[1]);
        while(fgets(line, LINESIZE, stdin) != NULL){
            n = strlen(line);
            if(write(fd1[1],line,n) != n){
                perr("write error");
            }
            if((n = read(fd2[0],line, LINESIZE)) < 0){
                perr("read error");
            }

            line[n] = '\0';
            if(fputs(line, stdout) == EOF){
                perr("fputs error");
            }
        }
        if(ferror(stdin)){
            perr("fgets error on stdin\n");
        }
        exit(0);
    } else{
        close(fd1[1]);
        close(fd2[0]);
        if (fd1[0] != STDIN_FILENO){
            if(dup2(fd1[0],STDIN_FILENO) != STDIN_FILENO){
                perr("dup2 error");
            }
            close(fd1[0]);
        }
        if(fd2[1] != STDOUT_FILENO){
            if(dup2(fd2[1],STDOUT_FILENO) != STDOUT_FILENO){
                perr("dup2 error");
            }
            close(fd2[1]);
        }
        if(execl("./add2","add2",(char*)0) < 0){
            perr("execl error");
        }

    }
    exit(0);
}