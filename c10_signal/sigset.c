#include <signal.h>
#include <stdio.h>
#include <unistd.h>


#define SIGBAD(signo) ((signo) <= 0 || (signo) >= NSIG)
static void sig_quit(int);


int main(){
    sigset_t newmask, oldmask, pendmask;
    if(signal(SIGQUIT, sig_quit) == SIG_ERR){
        printf("set SIGQUI error!\n");
        exit(1);
    }
    sigemptyset(&newmask);
    sigemptyset(&oldmask);
    sigemptyset(&pendmask);

    sigaddset(&newmask, SIGQUIT);

    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0){
        printf("set sigblock error!\n");
        exit(1);
    }
    sleep(5);
    if(sigpending(&pendmask) < 0){
        printf("sigpending error!\n");
        exit(1);
    }
    if(sigismember(&pendmask, SIGQUIT)){
        printf("SIGQUIT pending\n");
    }
    if (sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0){
        printf("sigprocmask error!\n");
        exit(1);
    }
    printf("SIGQUIT unblock\n");
    sleep(5);
    exit(0);
}

static void sig_quit(int signo){
    printf("caught SIGQUIT\n");
    if (signal(SIGQUIT,SIG_DFL) == SIG_ERR){
        printf("can't reset SIGQUIT");
        exit(1);
    }
}
