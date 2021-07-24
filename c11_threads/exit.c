#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* thr_fn1(void* arg){
    printf("thread 1 returning\n");
    return ((void*)1);
}

void* thr_fn2(void* arg){
    printf("thread 2 exiting\n");
    pthread_exit((void*)2);
}

int main(){
    pthread_t tid1, tid2;
    void* tret;

    if(pthread_create(&tid1, NULL, thr_fn1, NULL) != 0){
        printf("pthread create failed\n");
        exit(1);
    }

    if(pthread_create(&tid2, NULL,
    thr_fn2, NULL) != 0){
        printf("pthread create failed\n");
        exit(1);
    }

    if(pthread_join(tid1,&tret) != 0){
        printf("pthread join failed\n");
        exit(1);
    }

    printf("thread 1 returning code is :  %ld\n",(long)tret);

    if(pthread_join(tid2,&tret)!=0){
        printf("pthread join failed\n");
        exit(1);
    }

    printf("thread2 exiting code is : %ld\n",tret);

    return 0;
}