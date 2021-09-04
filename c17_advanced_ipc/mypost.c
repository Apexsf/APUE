#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#define KEY 0x132
#define NQ 3
#define MSGSIZE 1024

struct msgbuf{
    long mtype;
    char mtext[MSGSIZE];
};

void perr(const char* str){
    printf("%s, possible reason : %s\n",str, strerror(errno));
    exit(1);
}

int main(){
    

    







    int msgid;
    if((msgid = msgget(KEY,0)) < 0){
        perr("msgget error");
    }
    struct msgbuf mb;
    mb.mtype  = 1;
    strncpy(mb.mtext, "hello",6);
    if((msgsnd(msgid, &mb, MSGSIZE, IPC_NOWAIT)) < 0){
        perr("msgsnd error");
    }
    
    exit(0);

}