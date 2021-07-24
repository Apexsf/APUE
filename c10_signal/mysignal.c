#include <signal.h>
#include <stdio.h>
#include <unistd.h>

typedef void Sigfunc (int);


Sigfunc* mysignal(int signo, Sigfunc* func){
    struct sigaction act, old_act;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if(signo == SIGALRM){
    #ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
    #endif
    } else{
        act.sa_flags |= SA_RESTART;
    }
    if (sigaction(signo, &act, &old_act) < 0){
        return SIG_ERR;
    }
    return act.sa_handler;
}


void foo_handler(int signo){
    if(signo == SIGALRM){
        printf("receive SIGALRM\n");
    }
}

int main(){
    if (mysignal(SIGALRM, foo_handler) == SIG_ERR){
        printf("set signal error!\n");
        exit(1);
    }
    alarm(1);
    sleep(2);
    return 0;
}
