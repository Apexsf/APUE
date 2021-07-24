#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void sig_user(int signo){
    if (signo == SIGUSR1){
        printf("received SIGUSR1\n");
    }else if (signo == SIGUSR2){
        printf("received SIGUSR2\n");
    }
    else{
        printf("receive signal %d\n",signo);
    }
}

int main(){
    if(signal(SIGUSR1,sig_user) == SIG_ERR){
        printf("can't catch SIGUSR1\n");
    } 
    if(signal(SIGUSR2,sig_user) == SIG_ERR){
        printf("can't catch SIGUSR2\n");
    }
    while (1){
        pause();
    }
    
}