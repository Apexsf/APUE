// #include <signal.h>
// #include <unistd.h>
// #include <stdio.h>


// void foo_handler(int signo){
//     for(int i = 0; i < 5; i++){
//         sleep(1);
//         printf("In handler!\n");
//     }
// }

// int main(){
//     signal(SIGALRM, foo_handler);
//     alarm(1);
//     for(int i = 0; i < 5; i++){
//         sleep(1);
//         printf("In main!\n");
//     }
//     return 0;
// }



#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void foo_handler(int signo){
    printf("In handler!\n");
    sleep(2);
}


int main(){
    signal(SIGALRM,foo_handler);
    alarm(2);
    pause();
    printf("In main!\n");
    return 0;
}