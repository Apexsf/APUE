#define _POSIX_C_SOURCE  200809L
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMNUM 800000L // num ber of numbers to sort
#define NTHR 8 //num of working threads
#define TNUM (NUMNUM / NTHR)

long nums[NUMNUM];  // the array need to be sorted 
long snum[NUMNUM]; // the array that have sroted

pthread_barrier_t br;

/* generage random for array start */
void generate_rand(long* start, long gene_num, int range_min, int range_max){
    long* tmp = start;
    long rand_num;
    long range = range_max - range_min;
    for(int i = 0; i < gene_num; ++i){
        rand_num = (long) (rand() / (float)RAND_MAX * range + 0.5);
        start[i] = rand_num;
    }
}

/* check if the array is sorted */
int check_sort(long * start ,int check_num){
    for ( int i = 1; i < check_num; i++){
        if (start[i -1] > start[i]) return 0;
    }
    return 1;
}

int complong(const void* arg1, const void* arg2){
    long l1 = *(long*)arg1;
    long l2 = *(long*)arg2;
    if(l1 < l2){
        return -1;
    } else if (l1 > l2){
        return 1;
    } else {
        return 0;
    }
}

void* thr_fn(void* start){
    start = (long*)start;
    qsort(start,TNUM,sizeof(long),complong);
    pthread_barrier_wait(&br);
    /* go off the perform more work */
    return (void*)0;
}

void merge(){
    long idxes[NTHR];
    long num_min;
    int idx_min;
    for(int i = 0; i < NTHR; i++){
        idxes[i] = i * TNUM;
    }
    for(int i = 0; i < NUMNUM; i++){
        num_min = __LONG_MAX__;
        for( int j = 0; j < NTHR; j++){
            if( (idxes[j] < (j+1)*TNUM) && nums[idxes[j]] < num_min){
                num_min = nums[idxes[j]];
                idx_min = j;
            }
        }
        snum[i] = nums[idxes[idx_min]];
        idxes[idx_min]++;
    }
}

void* test(void* arg){
    printf("test\n");
}

int main(){
    pthread_t tids[NTHR];

    generate_rand(nums,NUMNUM,0,NUMNUM);

    if (pthread_barrier_init(&br,NULL,NTHR+1) != 0 ){
        printf("barrier init failed !\n");
        exit(1);
    }

    for(int i = 0; i < NTHR; i++){
        if(pthread_create(&tids[i], NULL, thr_fn, &nums[i*TNUM]) != 0){
            printf("thread create failed !\n");
            exit(1);
        }
    }

    int ret = pthread_barrier_wait(&br);
    merge();
    if (check_sort(snum,NUMNUM)){
        printf("correctly sorted.\n");
    } else {
        printf("sorted failed.\n");
    }
    
    pthread_barrier_destroy(&br);
    return 0;
}