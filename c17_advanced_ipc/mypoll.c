#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <poll.h>


#define KEY 0x130
#define MSGSIZE 1024
#define BUFSIZE 1024
#define NQ 3

struct msgbuf{
    long mtype;
    char mtext[MSGSIZE];
};

struct thread_info{
    int msg_fd;
    int sock_fd;
};

void perr(const char* str){
    printf("%s, possible reason : %s\n",str, strerror(errno));
    exit(1);
}

void* recv_msg(void* args){
    while(1){
        struct thread_info* tf;
        struct msgbuf mb;
        int n;

        tf = (struct thread_info*)(args);
        if((n = msgrcv(tf->msg_fd, &mb, MSGSIZE, 0, 0)) < 0){
            perr("msgrcv error");
        }
        printf("recv_msg, %d : %s\n", tf->msg_fd,mb.mtext);
        if(write(tf->sock_fd, mb.mtext, strlen(mb.mtext)) < 0){
            perr("write error");
        }
    }

}



int main(){

    // int sps[2];
    // if(socketpair(AF_UNIX, SOCK_DGRAM, 0, sps) < 0){
    //     perr("sockpairs error");
    // }

    int msgids[NQ];
    struct thread_info tis[NQ];
    pthread_t tids[NQ];
    struct pollfd pld[3];
    
    for(int i = 0; i < NQ; i++){
        if((msgids[i] = msgget(KEY+i, IPC_CREAT | 0666)) < 0){
            perr("msgget");
        }
        tis[i].msg_fd = msgids[i];
        
        int fd[2];
        if((socketpair(AF_UNIX, SOCK_DGRAM, 0 ,fd)) < 0){
            perr("socketpairs error");
        }
        tis[i].sock_fd = fd[0];
        pld[i].fd = fd[1];
        pld[i].events = POLLIN;

        if(pthread_create(&tids[i], NULL, recv_msg, &tis[i]) != 0){
            perr("pthread_create error");
        }

    }

    char buf[BUFSIZ];
    int n;
    while(1){
        if(poll(pld, NQ,-1 ) < 0){
            perr("poll error");
        }
        for(int i = 0 ; i < NQ; i++){
            if(pld[i].revents & POLLIN){
                if((n = read(pld[i].fd,buf,BUFSIZ)) < 0){
                    perr("read error");
                }
                buf[n] = '\0';
                printf("in while : %s\n", buf);
            }
        }
    }


    
    for(int i = 0; i < NQ; i++){
        pthread_join(tids[i],NULL);
    }


    exit(0);
}