#include <stdio.h>
#include <unistd.h>
int main(){
    int buf[4096];
    if (read(STDIN_FILENO,buf,4096) < 0){
        printf("read error!");
    }
    printf("this is output from input:\n %s\n",buf);
}