#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>


typedef struct foo{
    int a, b, c, d;
}foo;

void print_foo(const char *s, foo* fp){
    printf("%s\n",s);
    printf("a : %d, b : %d, c : %d, d : %d\n",fp->a, fp->b, fp->c, fp->d);
}

void* th(void* arg){
    foo* fp = malloc(sizeof(foo));
    fp->a = 1;
    fp->b = 2;
    fp->c = 3;
    fp->d = 4;
    print_foo("elements of fp : ",fp);
    return (void*)fp;
}

int main(){
   pthread_t tid;
   foo* tret;
   if(pthread_create(&tid, NULL, th, NULL) != 0){
       printf("pthread create failed!\n");
       exit(1);
   }

   if(pthread_join(tid,(void*)&tret) != 0){
       printf("pthread join failed\n");
       exit(1);
   }

   print_foo("in main : ", tret);

   return 0;

}