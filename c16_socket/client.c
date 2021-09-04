#define _POSIX_C_SOURCE  200112L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFLEN 128
#define MAXSLEEP 128

int connect_retry(int domain, int type, int protocol, const struct sockaddr* addr, socklen_t alen){
    int numsec, fd;
    for(numsec = 1; numsec <= MAXSLEEP; numsec <<= 1){
        if((fd = socket(domain, type, protocol)) < 0){
            return -1;
        }
        if(connect(fd, addr, alen) == 0){
            return fd;
        }
        close(fd);
        if(numsec <= MAXSLEEP/2){
            sleep(numsec);
        }
    }
    return -1;
}

void print_uptime(int sockfd){
    int n;
    char buf[BUFFLEN];
    while((n = recv(sockfd, buf, BUFFLEN,0)) > 0){
        write(STDOUT_FILENO, buf, n);
    }
    if(n < 0){
        perr("recv error");
    }
}

void perr(const char* str){
    printf("%s\n",str);
    exit(1);
}

int main(){
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int fd;
    if((fd = connect_retry(AF_INET, SOCK_STREAM, 0, (struct sockaddr* )&addr, sizeof(addr))) < 0){
        perr("connect retry error");
    }
    print_uptime(fd);
    exit(0);
}