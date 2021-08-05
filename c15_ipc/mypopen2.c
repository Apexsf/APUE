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


static pid_t* childpid = NULL;
int maxfd;

void perr(char* str){
    printf("%s\n",str);
    exit(1);
}

FILE* mypopen(char* cmdstring, char* type){
    if((type[0] != 'r' && type[0] != 'w') || (type[1] != '\0')){
        return NULL;
    }

    if (childpid == NULL){
        maxfd = sysconf(_SC_OPEN_MAX);
        childpid = calloc(maxfd, sizeof(pid_t));
    }

    int pfd[2];
    if(pipe(pfd) < 0) return NULL;

    if(pfd[0] > maxfd || pfd[1] > maxfd) return NULL;

    pid_t pid;
    if((pid = fork()) < 0){
        return NULL;
    } else if (pid == 0){
        if(type[0] == 'r'){
            close(pfd[0]);
            if(STDOUT_FILENO != pfd[1]){
                dup2(pfd[1], STDOUT_FILENO);
                close(pfd[1]);
            }
        } else {
            close(pfd[1]);
            if(STDIN_FILENO != pfd[0]){
                dup2(pfd[0], STDIN_FILENO);
                close(pfd[0]);
            }
        }

        for(int i = 0; i < maxfd; i++){
            if (childpid[i] > 0){
                close(i);
            }
        }

        execl("/bin/sh", "sh", "-c", cmdstring, (char*)0);
        _exit(127);
    }

    FILE* fp;

    if(type[0] == 'r'){
        close(pfd[1]);
        childpid[pfd[0]] = pid;
        if((fp = fdopen(pfd[0],"r"))  == NULL) return NULL;
    } else {
        close(pfd[0]);
        childpid[pfd[1]] = pid;
        if ((fp = fdopen(pfd[1], "w")) == NULL) return NULL;
    }

    return fp;
}

int myplose(FILE* fp){
    if(childpid == NULL){
        errno = EINVAL;
        return -1;
    }
    
    int fd = fileno(fp);
    if(fd > maxfd){
        errno = EINVAL;
        return -1;
    }

    pid_t pid = childpid[fd];
    if(pid == 0){   
        errno = EINVAL;
        return -1;
    }

    childpid[fd] = 0;

    if(fclose(fp) == EOF) return -1;

    int stat;
    while(waitpid(pid, &stat, 0) < 0){
        if(errno != EINTR) return -1; // if interrupt by a signal, we call waitpid again.
    }

    return stat;



}

int main(){
    char line[4096];
    FILE* fin = fopen("./mypopen2.c","r");
    FILE* fout = mypopen("more", "w");
    while(fgets(line, 4096, fin) != NULL){
        fputs(line, fout);
    }

    myplose(fout);

}