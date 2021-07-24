#include <stdlib.h>
#include <stdio.h>

static void exit_type_1(void);

static void exit_type_2(void);


int main(){
    if(atexit(&exit_type_1) != 0){
        printf("register exit type 1 failed\n");
    }
    if(atexit(&exit_type_2) != 0){
        printf("register exit type 2 failed\n");
    }

    if(atexit(&exit_type_2) != 0){
        printf("register exit type 2 failed\n");
    }
    return 0;
}

static void exit_type_1(void){
    printf("exit type 1 is called\n");
}

static void exit_type_2(void){
    printf("exit type 2 is called\n");
}