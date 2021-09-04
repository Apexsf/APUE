#define _POSIX_C_SOURCE  200112L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>

#define BUFFLEN 128
#define MAXADDRLEN 256

int set_cloexec(int fd){
    int val;
    if((val = fcntl(fd,F_GETFD,0)) < 0){
        return -1;
    }
    val |= FD_CLOEXEC;
    return fcntl(fd, F_SETFD, val);
}

int initserver(int type, const struct sockaddr* addr, socklen_t alen, int qlen){
    int fd;
    int err = 0;
    if((fd = socket(addr->sa_family, type, 0)) < 0){
        return -1;
    }
    if(bind(fd, addr, alen) < 0){
        goto errout;
    }


    if(type == SOCK_STREAM || type == SOCK_SEQPACKET){
        if(listen(fd, qlen) < 0){
            goto errout;
        }
    }

    return fd;

    errout:
        err = errno;
        close(fd);
        errno = err;
        return -1;
}

void perr(const char* str){
    printf("%s\n",str);
    exit(1);
}

void serve(int sockfd){
    int n;
    socklen_t alen;
    FILE* fp;
    char buf[BUFFLEN];
    char abuf[MAXADDRLEN];
    struct sockaddr* addr = (struct sockaddr*)abuf;
    set_cloexec(sockfd);

    while(1){
        alen = MAXADDRLEN;
        if((n = recvfrom(sockfd, buf, BUFFLEN, 0, addr, &alen)) < 0){
            perr("recvfrom error");
        }
        if((fp = popen("/usr/bin/uptime", "r")) == NULL){
            sprintf(buf,"error:%s\n",strerror(errno));
            sendto(sockfd, buf, strlen(buf), 0, addr, alen);
        } else{
            if(fgets(buf, BUFFLEN, fp) != NULL){
                sendto(sockfd, buf, strlen(buf), 0, addr, alen);
            }
            pclose(fp);
        }
    }
}

int main(){
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int sockfd;
    if((sockfd = initserver(SOCK_DGRAM, &addr, sizeof(addr),0)) < 0){
        perr("init server error");
    }

    serve(sockfd);
    exit(0);

}