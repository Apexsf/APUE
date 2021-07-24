#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int glob = 6;
char buf[] = "write\n";

int main(){
    int var;
    pid_t pid;
    var = 88;
    if(write(STDOUT_FILENO, buf, sizeof(buf))-1 != sizeof(buf)-1){
        exit(1);
    }
    printf("before fork\n");
    if ((pid = fork()) < 0){
        exit(1);
    } else if (pid == 0){  // child process 
        glob++;
        var++;
    } else{ // parent process
        sleep(2);
    }
    printf("pid = %ld, glob = %d, var = %d\n",(long)getpid(), glob, var);
    exit(0);
}