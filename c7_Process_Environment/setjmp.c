#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
int main(){
   jmp_buf env;
   int val;
   val = setjmp(env);
   printf("the value is : %d\n", val);
   if (val){
      printf("the value is : %d\n", val);
      exit(0);
   }

   longjmp(env,0);
}
