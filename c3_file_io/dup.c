#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(){
    int fd;
    fd = open("./out", O_APPEND|O_RDWR|O_CREAT);
    dup2(fd, fileno(stdout));
    printf("hello world, this info is not shown in terminal, but the ./out file, since fd 1 is redirected.");
    exit(0);  
}