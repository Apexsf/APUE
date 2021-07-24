#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    execlp("ls","ls","/",(char*)0);
    printf("does these print?\n");
    exit(0);
}