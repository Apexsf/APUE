#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>

struct timeval end;
unsigned long long count;

void checktime(char* str){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    if(tv.tv_sec >= end.tv_sec && tv.tv_usec >= end.tv_usec){
        printf("%s count = %lld\n", str, count);
        exit(0);
    }
}



int main(int argc, char* argv[]){
    pid_t pid;
    int nzero,ret;
    int adj = 0;
    char* s;
    setbuf(stdout,NULL);

#if defined(NZERO)
    nzero = NZERO
#elif defined(_SC_NZERO)
    nzero = sysconf(_SC_NZERO);
#else
#error NZERO undefined
#endif
    printf("nzero is : %d\n",nzero);

    if(argc == 2){
        adj = strtol(argv[1],NULL,10);
    }

    if((pid = fork()) < 0){
        printf("fork failure\n");
        exit(1);
    }else if (pid == 0){
        s = "child";
        printf("the current nice value of child is %d, adjusted by %d\n",nice(0) + nzero, adj);
        //check man 2 nice, we  usually need to set and check errno when using nice
        errno = 0;
        if((ret = nice(adj)) == -1 && errno != 0){
            printf("add nice error\n");
            exit(1);
        }
        printf("now the current nice value of child is %d\n",ret + nzero); //or nice(0) + nzero
    }else{
        s = "parent";
        printf("the current nice value of parent is %d\n", nice(0) + nzero);
    }
    gettimeofday(&end, NULL);
    end.tv_sec += 10;
    while(1){
        ++count;
        checktime(s);
    }

    exit(0);

}