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
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

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

void serve(int sockfd){
    int clfd;
    FILE* fp;
    char buf[BUFFLEN];
    set_cloexec(sockfd);
    while(1){
        if((clfd = accept(sockfd, NULL,NULL)) < 0){
            perr("accept error");
        }
        set_cloexec(clfd);
        if((fp = popen("/usr/bin/uptime","r")) == NULL){
            sprintf(buf,"error : %s\n", strerror(errno));
            send(clfd, buf, strlen(buf),0);
        } else {
            while(fgets(buf, BUFFLEN, fp) != NULL){
                send(clfd, buf, strlen(buf), 0);
            }
            pclose(fp);
        }
        close(clfd);
    }
    
}

void perr(const char* str){
    printf("%s\n",str);
    exit(1);
}

int main(int argc, char* argv[]){
    int  n;
    char *host;
    
    if(argc != 1){
        perr("wrong args");
    }

    if((n = sysconf(_SC_HOST_NAME_MAX)) < 0){
        n = HOST_NAME_MAX;  /*  best guess */
    }

    if((host = malloc(n)) == NULL){
        perr("malloc error");
    }

    if(gethostname(host, n) < 0){
        perr("gethostname error");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(5000);

    int listen_fd = initserver(SOCK_STREAM, &addr,sizeof(addr),16);
    serve(listen_fd);

    exit(0);
}