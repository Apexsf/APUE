#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 4096
int main(){
    char buf[BUFFER_SIZE];
    int n;
    while ((n = read(STDIN_FILENO, buf, BUFFER_SIZE)) > 0){
        if (write(STDOUT_FILENO,buf,n) != n){
            printf("write error");
        }
    }
    if (n < 0){
        printf("read error");
    }
    exit(0);
}