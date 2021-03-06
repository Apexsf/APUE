#define _POSIX_C_SOURCE  200809L

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


int main(void){
    int err;
    struct timespec tout;
    struct tm* tmp;
    char buf[64];

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&lock);
    printf("mutex is locked\n");
    clock_gettime(CLOCK_REALTIME, &tout);
    tmp = localtime(&tout.tv_sec);
    strftime(buf,sizeof(buf),"%r",tmp);
    printf("the current time is %s\n", buf);
    tout.tv_sec += 10;
    /* caution : this could lead to deadlock ! */
    err = pthread_mutex_timedlock(&lock,&tout);
    clock_gettime(CLOCK_REALTIME,&tout);
    tmp = localtime(&tout.tv_sec);
    strftime(buf, sizeof(buf), "%r", tmp);
    printf("the time is noow %s\n",buf);
    if(err == 0){
        printf("mutex locked again!\n");
    } else{
        printf("can't lock mutex again : %s\n",strerror(err));
    }
    exit(0);

}