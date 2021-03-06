#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define NHASH 29
#define HASH(id) (((unsigned long)id) % NHASH)

typedef struct foo{
    int f_count;
    pthread_mutex_t f_lock;
    int f_id;
    struct foo* f_next;
}foo;

pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;
foo* fh[NHASH];

struct foo* foo_alloc(int id){
    struct foo* fp;
    int idx;
    if((fp == malloc(sizeof(foo))) != NULL){
        fp->f_count = 1;
        fp->f_id = id;
        if(pthread_mutex_init(&fp->f_lock,NULL) != 0){
            free(fp);
            return NULL;
        }
        idx = HASH(id);
        pthread_mutex_lock(&hashlock);
        fp->f_next = fh[idx];
        fh[idx] = fp;
        pthread_mutex_lock(&fp->f_lock);
        pthread_mutex_unlock(&hashlock);
        /* do some initializations more */
        pthread_mutex_unlock(&fp->f_lock);
    }
    return fp;
}

void foo_hold(foo * fp){
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}

foo* foo_find(int id){
    foo* fp;
    pthread_mutex_lock(&hashlock);
    for(fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next){
        if(fp->f_id == id){
            foo_hold(fp);
            break;
        }
    }
    pthread_mutex_unlock(&hashlock);
    return fp;
}

void foo_rele(foo* fp){
    foo* tfp;
    int idx;
    pthread_mutex_lock(&fp->f_lock);
    if (fp->f_count == 1){
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_lock(&hashlock);
        pthread_mutex_lock(&fp->f_lock);
        /* the condition need to be recheck,since the the state of fp can change in the middle of unlock f_lock and lock hashlock */
        if(fp->f_count != 1){
            fp->f_count--;
            pthread_mutex_unlock(&fp->f_lock);
            pthread_mutex_unlock(&hashlock);
            return;
        }
        idx = HASH(fp->f_id);
        tfp = fh[idx];
        if(tfp == fp){
            fh[idx] = fp->f_next;
        } else{
            while(tfp->f_next != fp) tfp = tfp->f_next;
            tfp->f_next = fp->f_next;
        }
        pthread_mutex_unlock(&hashlock);
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
    } else{
        fp->f_count--;
        pthread_mutex_unlock(&fp->f_lock);
    }
}

int main(){

}