#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int system(char* cmdstring){
    pid_t pid;
    int status;
    if (cmdstring == NULL) return 1;
    if((pid = fork())< 0) status = -1;
    else if (pid == 0){
        execl("/bin/sh","sh","-c",cmdstring,(char*)0); // if the execl success, then the process will be finish, implying that _exit(127) will not be executed.
        _exit(127);
    }else{
        while(waitpid(pid,&status,0) < 0){
            if(errno != EINTR){
                status = -1;
                break;
            }
        }
    }
    return status;
}


int main(){
    system("ls /");
}