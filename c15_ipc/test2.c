

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>



char* buf = NULL;

int main(){
    
    pid_t pid;
    int pfd[2];
    buf = malloc(4096);
    if(pipe(pfd) < 0) exit(1);
    if((pid = fork()) < 0) exit(1);
    else if(pid == 0){
        int n = 0;
        int c = 0;
        close(pfd[1]);
        if(pfd[0] != STDIN_FILENO){
            dup2(pfd[0],STDIN_FILENO);
            close(pfd[0]);
        }
       while((n = read(STDIN_FILENO,buf,4096)) !=-1){
            printf("child : %s\n",buf);
            buf[n] = '\0';
            c = buf[0];
            if(isupper(c)){
                c = tolower(c);
            }
            if(fputc(c,stdout) == EOF){
                printf("wrong");
                exit(1);
            }
            printf("feng\n");
            if(c == '\n'){
                fflush(stdout);
            }
        }
        printf("n : %d",n);
    }
    close(pfd[0]);
    while (1){
        read(STDIN_FILENO, buf, 4096);
        printf("read\n");
        write(pfd[1],buf,4096);
        printf("write\n");
    }


}


// #include <sys/types.h>
// #include <sys/wait.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <string.h>



// char* buf = NULL;

// int main(){
    
//     pid_t pid;
//     int pfd[2];
//     buf = malloc(4096);
//     int n = 0;
//     if(pipe(pfd) < 0) exit(1);
//     if((pid = fork()) < 0) exit(1);
//     else if(pid == 0){
//         close(pfd[1]);
//         if(pfd[0] != STDIN_FILENO){
//             dup2(pfd[0],STDIN_FILENO);
//             close(pfd[0]);
//         }
//         while(read(STDIN_FILENO,buf,4096)  != -1){
//             printf("in child : %s",buf);
//         }
//     }
//     close(pfd[0]);
//     while (1){
//         read(STDIN_FILENO, buf, 4096);
//         write(pfd[1],buf,4096);
//     }


// }