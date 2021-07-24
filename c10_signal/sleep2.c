#define _POSIX_C_SOURCE 200112L

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void sig_alrm(int signo){
    //do  nothing
}

unsigned int sleep(unsigned int seconds){
    struct sigaction newact, oldact;
    sigset_t newmask,oldmask,suspmask;
    unsigned int unslept;

    //set our handler, and set previous information about the old handler of SIGALRM
    newact.sa_handler = sig_alrm;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGALRM,&newact,&oldact);

    //block SIGALRM and save the current signal mask
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGALRM);
    sigprocmask(SIG_BLOCK,&newmask,&oldmask);
    
    alarm(seconds);
    

    //make sure SIGALRM isn't blocked
    suspmask = oldmask;
    sigdelset(&suspmask, SIGALRM);

    //wait for any signal to be caught
    sigsuspend(&suspmask);
    //some signal has been caught, and the sigsuspend finishes, the mask of the process is set to the former mask, which blocks SIGALRM

    unslept = alarm(0);

    sigaction(SIGABRT,&oldact,NULL);
    sigprocmask(SIG_SETMASK,&oldmask,NULL);
    return unslept;

}

int main(){
    sleep(2);
    return 0;
}
