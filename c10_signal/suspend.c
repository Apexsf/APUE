#define _POSIX_C_SOURCE 200112L

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

static void sig_int(int);


void pr_mask(const char *str)
{
        sigset_t       sigset;
        int            errno_save;

        errno_save = errno;             /* we can be called by signal handlers */
        if (sigprocmask(0, NULL, &sigset) < 0) {
                printf("sigprocmask error");
                exit(1);
        } else {
                printf("%s", str);
                if (sigismember(&sigset, SIGINT))
                        printf(" SIGINT");
                if (sigismember(&sigset, SIGQUIT))
                        printf(" SIGQUIT");
                if (sigismember(&sigset, SIGUSR1))
                        printf(" SIGUSR1");
                if (sigismember(&sigset, SIGALRM))
                        printf(" SIGALRM");

                /* remaining signals can go here  */

                printf("\n");
        }

        errno = errno_save;             /* restore errno */
}

static void sig_int(int signo){
    pr_mask("\nIn sig_int, the signal mask : ");
}


int main(){
    sigset_t new_mask, old_mask,wait_mask;
    printf("program start\n");
    if(signal(SIGINT,sig_int) == SIG_ERR){
        printf("signal(SIGINT) error!\n");
        exit(1);
    }
    sigemptyset(&new_mask);
    sigemptyset(&old_mask);
    sigemptyset(&wait_mask);

    sigaddset(&wait_mask, SIGUSR1);
    sigaddset(&new_mask,SIGINT);

    if(sigprocmask(SIG_BLOCK,&new_mask,&old_mask) < 0){
        printf("SIG_BLOCK error!\n");
        exit(1);
    }

    pr_mask("In critical region, the signal mask : ");

    if(sigsuspend(&wait_mask) != -1){
        printf("suspend error!\n");
        exit(1);
    }

    pr_mask("Returned from sigsuspend, the signal mask : ");

    if(sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0){
        printf("SIG_SETMASK error.\n");
        exit(1);
    }

    printf("program exit.\n");

    
}


