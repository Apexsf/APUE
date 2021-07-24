#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void print_signal(int signo){
    printf("the received signal is : %d\n",signo);
}

int main(){
    signal(SIGALRM, &print_signal);
    alarm(5);
    sleep(2);
    int remain_seconds = alarm(0);
    printf("the remain seconds is %d\n",remain_seconds);
    sleep(2);
}