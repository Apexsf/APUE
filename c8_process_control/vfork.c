#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
int glob = 6;
int main(){
    int var = 88;
    pid_t pid;
    if ((pid = vfork() < 0)){
        printf("vfork failure!");
        exit(0);
    } else if (pid == 0){
        var++;
        glob++;
       _exit(127);
    }
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), glob,var);
    exit(0);
}