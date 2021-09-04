#define _POSIX_C_SOURCE  200112L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <poll.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define NQ 3
#define MAXMSZ 512
#define KEY 0x123

struct threadinfo {
    int qid;
    int fd;
};

struct mymesg{
    long mtype;
    char mtext[MAXMSZ];
};

void perr(char* str){
    printf("%s\n",str);
    exit(1);
}

void* helper(void* arg){
    int n;
    struct mymesg m;
    struct threadinfo *tip = arg;

    while(1){
        memset(&m,0,sizeof(m));
        if((n = msgrcv(tip->qid, &m, MAXMSZ, 0, MSG_NOERROR)) < 0){
            perr("msgrcv");
        }
        if(write(tip->fd,m.mtext,n) < 0){
            perr("write error");
        }
    }
}

int main(){
    int i, n, err;
    int fd[2];
    int qid[NQ];
    struct pollfd pfd[NQ];
    struct threadinfo ti[NQ];
    pthread_t tid[NQ];
    char buf[MAXMSZ];

    // int m1 = msgget((KEY+1),IPC_CREAT|0666);

    for(int i = 0; i < NQ; i++){
        if((qid[i] = msgget((KEY+i), IPC_CREAT | 0666)) < 0){
            perr("msgget error");
        }
        printf("queue ID %d is %d\n", i, qid[i]);

        if(socketpair(AF_UNIX, SOCK_DGRAM, 0, fd) < 0){
            perr("socketpair error");
        }

        pfd[i].fd = fd[0];
        pfd[i].events = POLLIN;
        ti[i].qid = qid[i];
        ti[i].fd = fd[1];
        if((err = pthread_create(&tid[i], NULL, helper, &ti[i])) != 0){
            perr("pthread_create error");
        }
    }

    while(1){
        if (poll(pfd, NQ, -1) < 0){
            perr("poll error");
        }
        for(int i = 0; i< NQ; i++){
            if(pfd[i].revents & POLLIN){
                if((n = read(pfd[i].fd, buf, sizeof(buf))) < 0){
                    perr("read error");
                }
                buf[n] = 0;
                printf("queue id %d, message %s\n", qid[i], buf);
            }  
        }
    }
    exit(0);
}