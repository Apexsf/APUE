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

#define MAXMSZ 512

struct mymesg{
    long mtype;
    char mtext[MAXMSZ];
};

void perr(char* str){
    printf("%s\n");
    exit(1);
}


int main(int argc, char *argv[]){
    key_t key;
    long qid;
    size_t nbytes;
    struct mymesg m;

    // char* argv_[3];
    // argv_[0] = "./sendmsg";
    // argv_[1] = "0x123";
    // argv_[2] = "hello world";
    // argc = 3;
    // argv = argv_;

    if(argc != 3){
        perr("usage : sendmsg KEY message");
    }

    key = strtol(argv[1],NULL,0);
    if((qid = msgget(key, 0)) < 0){
        perr("msgget error");
    }

    memset(&m, 0, sizeof(m));
    strncpy(m.mtext, argv[2], MAXMSZ-1);
    nbytes = strlen(m.mtext);

    m.mtype = 1;
                                
    if(msgsnd(qid, &m, nbytes, 0) < 0){
        perr("can't send message");
    }
    exit(0);
}