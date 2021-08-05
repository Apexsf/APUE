 #define _POSIX_C_SOURCE 200112
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int make_thread(void * (*fn) (void *), void* arg){
    int err;
    pthread_t tid;
    pthread_attr_t attr;
    err = pthread_attr_init(&attr);
    if(err != 0){
        return err;
    }
    err = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    if (err == 0){
        err = pthread_create(&tid,&attr,fn,arg);
    }
    pthread_attr_destroy(&attr);
    return err;
}

typedef struct to_info{
    void (*to_fn) (void*);
    void* to_arg;
    struct timespec to_wait;
} to_info;

void* time_helper(void* arg){
    to_info* tip;
    tip = (to_info*) arg;
    clock_nanosleep(CLOCK_REALTIME,0,&tip->to_wait,NULL);
    (*tip->to_fn)(tip->to_arg);
    free(arg);
    return 0;
}

void timeout(const struct timespec* when, void (*func) (void*), void* arg){
    struct timespec now;
    struct to_info* tip;
    int err;
    clock_gettime(CLOCK_REALTIME, &now);
    if ((when->tv_sec > now.tv_sec) || (when->tv_sec == now.tv_sec && when->tv_nsec > now.tv_nsec)){
        tip = malloc(sizeof(to_info));
        if(tip != NULL){
            tip->to_fn = func;
            tip->to_arg = arg;
            tip->to_wait.tv_sec = when->tv_sec - now.tv_sec;
            if(when->tv_nsec >= now.tv_nsec){
                tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
            } else{
                tip->to_wait.tv_sec--;
                tip->to_wait.tv_nsec = when->tv_nsec + 1000000000 - now.tv_nsec;
            }
            err = make_thread(time_helper, (void*) tip);
            if(err == 0){
                return;
            }else{
                free(tip);
            }
        }
    }
    /*we get here if (a) when <= now, (b)malloc fails(return NULL), (c) we can't make a thread so we just call the function now */
    (*func)(arg);
}

pthread_mutexattr_t attr;
pthread_mutex_t mutex;

void test_func(void* arg){
    pthread_mutex_lock(&mutex);
    printf("In test_func : %d\n",arg);
    pthread_mutex_lock(&mutex);
}

int main(){
    int err, condition, arg;
    struct timespec when;
    if((err = pthread_mutexattr_init(&attr)) != 0){
        printf("init mutex attribute failed\n");
        exit(1);
    }
    if((err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP)) != 0){
        printf("set mutex attribute to recursive failed\n");
        exit(1);
    }
    if((err = pthread_mutex_init(&mutex,&attr))!=0){
        printf("init mutex failed\n");
        exit(1);
    }

    /* continue to processing */
    pthread_mutex_lock(&mutex);
    /* check the condition under the protection of a lock to make the check and the call to
    timeout atmic */

    condition = 1;
    if(condition){
        clock_gettime(CLOCK_REALTIME,&when);
        when.tv_sec += 5;
        timeout(&when, test_func,(void*)22);
    }
    pthread_mutex_unlock(&mutex);
    /* continue to processing */
    sleep(8);
    exit(0);
}