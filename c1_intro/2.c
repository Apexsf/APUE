#include <string.h>
#include <stdio.h>
#include <errno.h>
int main(){
    for(int i = 1; i < 100; i++){
        printf("%d : %s\n",i,strerror(i));
        perror(strerror(i));
        printf("\n");
    }
    errno = EAGAIN;
    perror("./32");
}