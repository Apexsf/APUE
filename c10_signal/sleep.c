#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <setjmp.h>

// void sleep_handler(int signo){
//     printf("handling\n");
// }

// unsigned int my_sleep(unsigned int seconds){
//     if (signal(SIGALRM, sleep_handler) == SIG_ERR){
//         printf("register signal error!\n");
//         exit(1);
//     } 
//     alarm(seconds);
//     pause();
//     return alarm(0);
// }

// int main(){
//     int ret;
//     ret = my_sleep(3);
//     printf("%d\n",ret);
//     ret = my_sleep(2);
//     printf("%d\n",ret);
//     ret = my_sleep(4);
//     printf("%d\n",ret);
//  }

static jmp_buf env_alrm;

void sleep_handler(int signo){
    siglongjmp(env_alrm,1);
}

unsigned int my_sleep(unsigned int seconds){
    if(signal(SIGALRM, sleep_handler)==SIG_ERR){
        printf("signal error!\n");
        exit(1);
    }
    if (sigsetjmp(env_alrm,1) == 0){
        alarm(seconds);
        pause();
    }
    return alarm(0);
}

int main(){
    int ret;
    ret = my_sleep(3);
    printf("%d\n",ret);
    ret = my_sleep(2);
    printf("%d\n",ret);
    ret = my_sleep(4);
    printf("%d\n",ret);
}


