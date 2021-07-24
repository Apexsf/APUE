#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[]){
    int i = 1;
    for(char** argv_temp = argv; *argv_temp != 0; argv_temp++){
        printf("the %dth argument is : %s\n",i, *argv_temp);
        i++;
    }
    i = 1;
    for(char** envp_temp = envp; *envp_temp != 0; envp_temp++){
        printf("the %dth environment variable is : %s\n",i,*envp_temp);
        i++;
    }
} 