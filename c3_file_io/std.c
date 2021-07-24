#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
    char buffer[100];
    read(STDIN_FILENO,buffer,100);
    write(STDOUT_FILENO,buffer,strlen(buffer));
}