#define _POSIX_C_SOURCE  200809L
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

typedef struct job{
    struct job* j_next;
    struct job* j_prev;
    pthread_t j_id;
    /* more stuff here */
}job;

typedef struct queue{
    job* q_head;
    job* q_tail;
    pthread_rwlock_t q_lock;
}queue;

int queue_init(queue* qp){
    int err;
    qp->q_head = NULL;
    qp->q_tail = NULL;
    err = pthread_rwlock_init(&qp->q_lock,NULL);
    if(err!=0){
        return err;
    }
    /* contine initialization*/
    return 0;
}

/* insert a job at the head of the queue */
void job_insert(queue* qp, job* jp){
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next  = qp->q_head;
    jp->j_prev = NULL;
    if(qp->q_head != NULL){
        qp->q_head->j_prev = jp;
    }else{
        qp->q_tail = jp;
    }
    qp->q_head = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

/* and the a job on the tail of the queue*/
void job_append(queue* qp, job* jp){    
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_prev = qp->q_tail;
    jp->j_next = NULL;
    if(qp->q_tail != NULL){
        qp->q_tail->j_next = jp;
    }else{
        qp->q_head = jp;
    }
    qp->q_tail = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

/* remove  the given job from a queue.*/
void job_remove(queue* qp, job* jp){
    pthread_rwlock_wrlock(&qp->q_lock);
    if(qp->q_head == jp){
        qp->q_head = jp->j_next;
        if(qp->q_tail == jp){
            qp->q_tail = NULL;
        }else{
            jp->j_next->j_prev = jp->j_prev; /*e qual to set qp->q_head_>j_prev to NULL ? */
        }
    } else if (qp->q_tail == jp){
        qp->q_tail = jp->j_prev;
        jp->j_prev->j_next = NULL;
    } else{
        jp->j_prev->j_next = jp->j_next;
        jp->j_next->j_prev = jp->j_prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}

/* Find a job for the given thread ID. */
job* job_find(queue* qp, pthread_t id){
    job* jp;
    if(pthread_rwlock_rdlock(&qp->q_lock)!=0){
        return NULL;
    }
    for(jp = qp->q_head; jp != NULL; jp = jp->j_next){
        if(pthread_equal(jp->j_id,id)){
            break;
        }
    }
    pthread_rwlock_unlock(&qp->q_lock);
    return jp;
}

int main(){

}