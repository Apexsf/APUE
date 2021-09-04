#define _POSIX_C_SOURCE  200112L
#define _GNU_SOURCE  

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
#include <sys/un.h>


#if defined(SCM_CREDENTIALS)
#define CREDSTRUCT ucred
#define SCM_CREDTYPE SCM_CREDENTIALS
#define CR_UID uid
#define CREDOPT SO_PASSCRED
#else
#error passing credentials is unsupported
#endif  

#define RIGHTSLEN CMSG_LEN(sizeof(int))
#define CREDSLEN CMSG_LEN(sizeof(struct CREDSTRUCT))
#define CONTROLLEN (RIGHTSLEN + CREDSLEN)
#define MAXLINE 4096

static struct cmsghdr* cmptr = NULL;

int send_fd(int fd, int fd_to_send){
    struct CREDSTRUCT* credp;
    struct cmsghdr* cmp;
    struct iovec iov[1];
    struct msghdr msg;
    char buf[2];

    iov[0].iov_base = buf;
    iov[0].iov_len = 2;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    if (fd_to_send < 0){
        msg.msg_control = NULL;
        msg.msg_controllen = 0;
        buf[1] = -fd_to_send;
        if(buf[1] == 0){
            buf[1] = 1;
        }
    }else{
        if(cmptr == NULL & (cmptr = malloc(CONTROLLEN)) == NULL){
            return -1;
        }
        msg.msg_control = cmptr;
        msg.msg_controllen = CONTROLLEN;
        
        cmp = cmptr;
        cmp->cmsg_level = SOL_SOCKET;
        cmp->cmsg_type = SCM_RIGHTS;
        cmp->cmsg_len = RIGHTSLEN;
        *(int*)CMSG_DATA(cmp) = fd_to_send;
        
        cmp = CMSG_NXTHDR(&msg,cmp);
        cmp->cmsg_level = SOL_SOCKET;
        cmp->cmsg_type = SCM_CREDTYPE;
        cmp->cmsg_len = CREDSLEN;
        credp = (struct CREDSTRUCT*) CMSG_DATA(cmp);
    #if defined(SCM_CREDENTIALS)
        credp->uid = geteuid();
        credp->gid = getegid();
        credp->pid = getpid();
    #endif
        buf[1] = 0; 
    }

    buf[0] = 0;
    if(sendmsg(fd, &msg, 0) != 2){
        return -1;
    }
    return 0;
} 


int recv_ufd(int fd, uid_t* uidptr, ssize_t (*userfunc)(int , const void*, size_t)){
    struct cmsghdr *cmp;
    struct CREDSTRUCT *credp;
    char *ptr;
    char buf[MAXLINE];

    struct iovec iov[1];
    struct msghdr msg;
    
    int nr;
    int newfd = -1;
    int status = -1;

#if defined(CREDOPT)
    const int on = 1;
    if(setsockopt(fd, SOL_SOCKET, CREDOPT, &on, sizeof(int)) < 0){
        printf("setsockopt error\n");
        return -1;
    }
#endif  

    while(1){
        iov[0].iov_base = buf;
        iov[0].iov_len = sizeof(buf);
        msg.msg_iov = iov;
        msg.msg_iovlen = 1;
        msg.msg_name = NULL;
        msg.msg_namelen = 0;
        if(cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL){
            return -1;
        }
        msg.msg_control = cmptr;
        msg.msg_controllen = CONTROLLEN;
        if((nr = recvmsg(fd, &msg, 0)) < 0){
            printf("recvmsg error\n");
            return -1;
        } else if(nr == 0){
            printf("connection closed by server\n");
            return -1;
        }

        for(ptr = buf ; ptr < &buf[nr]; ){
            if(*ptr++ == 0){
                if(ptr != &buf[nr-1]){
                    printf("msg format error\n");
                    exit(1);
                }
                status = *ptr & 0xFF;
                if(status == 0){
                    if(msg.msg_controllen != CONTROLLEN){
                        printf("status = 0 but no fd\n");
                        exit(1);
                    }
                    /*process the control data */
                    for(cmp = CMSG_FIRSTHDR(&msg); cmp != NULL; cmp = CMSG_NXTHDR(&msg,cmp)){
                        if(cmp->cmsg_level != SOL_SOCKET){
                            continue;
                        }
                        switch(cmp->cmsg_type){
                            case SCM_RIGHTS:
                                newfd = *(int *)CMSG_DATA(cmp);
                                break;
                            case SCM_CREDTYPE:
                                credp = (struct CREDSTRUCT*) CMSG_DATA(cmp);
                                *uidptr = credp->CR_UID;
                        }
                    }
                }else{
                    newfd = -status;
                }
                nr -= 2;
            }
        }
        if(nr > 0 && (*userfunc)(STDERR_FILENO, buf, nr) != nr){
            return -1;
        }
        if(status > 0){
            return newfd;
        }
    }
}



int main(){ 
 
    
}