#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void cleanup(void* arg){
    printf("cleanup : %s",(char*)arg);
}

void* th_fn1(void* arg){
    printf("thread 1 start\n");
    pthread_cleanup_push(cleanup, "thread1 first handler\n");
    pthread_cleanup_push(cleanup, "thread1 second handler\n");
    pthread_exit((void*)1);
    pthread_cleanup_pop(0);    
    pthread_cleanup_pop(0);
}

void* th_fn2(void* arg){
    printf("thread 2 start\n");
    pthread_cleanup_push(cleanup, "thread2 first handler\n");
    pthread_cleanup_push(cleanup, "thread2 second handler\n");
    return (void*)1;
    pthread_cleanup_pop(0);    
    pthread_cleanup_pop(0);
}

void* th_fn3(void* arg){
    printf("thread 3 start\n");
    pthread_cleanup_push(cleanup, "thread3 first handler\n");
    pthread_cleanup_push(cleanup, "thread3 second handler\n");
    pthread_cleanup_pop(0);    
    pthread_cleanup_pop(0);
}



int main(){
    pthread_t tid_1, tid_2, tid_3, tid_4;
    if(pthread_create(&tid_1, NULL, th_fn1, NULL)!=0) exit(1);
    if(pthread_join(tid_1,NULL) != 0) exit(1);

    if(pthread_create(&tid_2, NULL, th_fn2, NULL)!=0) exit(1);
    if(pthread_join(tid_2,NULL) != 0) exit(1);

    if(pthread_create(&tid_3, NULL, th_fn3, NULL)!=0) exit(1);
    if(pthread_join(tid_3,NULL) != 0) exit(1);

    return 0; 
}