#define _POSIX_C_SOURCE  200112L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFLEN 128
#define TIMEOUT 20

void sigalrm(int signo){

}

void print_uptime(int sockfd, struct sockaddr* addr, int addrlen){
    int n;
    char buf[BUFFLEN];
    buf[0] = 0;

    if(sendto(sockfd,buf,1,0,addr,addrlen) < 0){
        perr("sendto error");
    }
    alarm(TIMEOUT);
    if((n = recvfrom(sockfd,buf,BUFFLEN, 0,NULL,NULL)) < 0){
        if(errno != EINTR){
            alarm(0);
        }
        perr("recvfrom error");
    }
    alarm(0);
    write(STDOUT_FILENO, buf, n);
}

void perr(const char* str){
    printf("%s\n",str);
    exit(1);
}


int main(int argc, char* argv[]){
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = sigalrm;
    sigemptyset(&sa.sa_mask);
    if(sigaction(SIGALRM, &sa, NULL) < 0){
        perr("sigaction error");
    }

    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_DGRAM, 0)) < 0){
        perr("socket error");
    }
    
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    
    print_uptime(sockfd,(struct sockaddr*)&addr, sizeof(addr));

    exit(0);



}
