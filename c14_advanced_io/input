#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>



char buf[500000];

void set_fl(int fd, int flags){
    int val;
    
    if((val = fcntl(fd, F_GETFL,0)) < 0){
        printf("get F_GETFL error\n");
        exit(1);
    }
    val |= flags;
    
    if(fcntl(fd, F_SETFL,val) < 0){
        printf("set F_SETFL error\n");
        exit(1);
    }
}

int main(){
    int ntowrite, nwrite;
    char* ptr;
    ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
    fprintf(stderr, "read %d bytes\n", ntowrite);

    set_fl(STDOUT_FILENO, O_NONBLOCK);

    ptr = buf;
    while(ntowrite > 0){
        errno = 0;
        nwrite = write(STDOUT_FILENO, ptr, ntowrite);
        fprintf(stderr, "nwrite = %d, errno :  %s\n", nwrite, strerror(errno));
        if(nwrite > 0){
            ptr += nwrite;
            ntowrite -= nwrite;
        }
    }
    exit(0);
}