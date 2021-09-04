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

#define CONTROLLEN CMSG_LEN(sizeof(int))
#define MAXLINE 4096

static struct  cmsghdr *cmptr = NULL;


ssize_t writen(int fd, const void* ptr, size_t n){
    size_t nleft;
    size_t nwritten;
    
    nleft = n;
    while(nleft  > 0){
        if((nwritten = write(fd, ptr, nleft)) < 0){
            if(nleft == n){
                return -1;
            } else {
                break;
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n-nleft;
}

int send_fd(int fd, int fd_to_send){
    struct iovec iov[1];
    struct msghdr msg;
    char buf[2];

    iov[0].iov_base = buf;
    iov[0].iov_len = 2;

    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    if(fd_to_send < 0){
        msg.msg_control = NULL;
        msg.msg_controllen = 0;
        buf[1] = -fd_to_send;
        if(buf[1] == 0){
            buf[1] = 1;
        }
    }else{
        if(cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL){
            return -1;
        }
        cmptr->cmsg_level = SOL_SOCKET;
        cmptr->cmsg_type = SCM_RIGHTS;
        cmptr->cmsg_len = CONTROLLEN;
        
        msg.msg_control = cmptr;
        msg.msg_controllen = CONTROLLEN;
        *(int*)CMSG_DATA(cmptr) = fd_to_send;
        buf[1] = 0;
    }
    buf[0] = 0;
    if(sendmsg(fd, &msg, 0) != 2){
        return -1;
    }
    return 0;
}

int send_err(int fd, int errcode, const char* msg){
    int n;
    if((n = strlen(msg)) > 0){
        if(writen(fd, msg, n) != n){
            return -1;
        }
    }

    if(errcode >= 0){
        errcode = -1;
    }

    if(send_fd(fd, errcode) < 0){
        return -1;
    }
}

int recv_fd(int fd, ssize_t (*userfunc) (int , const void*, size_t)){
    int newfd, nr, status;
    char* ptr;
    char buf[MAXLINE];
    struct iovec iov[1];
    struct msghdr msg;
    status = -1;
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
        } else if (nr == 0){
            printf("connection closed by server");
            return -1;
        }

        for(ptr = buf; ptr < &buf[nr];){
            if(*ptr++ == 0){
                if(ptr != &buf[nr-1]){
                    printf("message format error\n");
                    exit(1);
                }
            }
            status = *ptr & 0xFF;
            if(status == 0){
                if(msg.msg_controllen != CONTROLLEN){
                    printf("status = 0 buf no fd");
                }
            } else{
                newfd = -status;
            }
            nr -= 2;
        }
    }

    if(nr > 0 && (*userfunc) (STDERR_FILENO, buf, nr) != nr){
        return -1;
    }
    if(status > 0){
        return newfd;
    }
}


int main(){
    
}