#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>

int main(){
    int c;
    char buf[4096];
    printf("tinglf\n");
    //while((c = fgetc(stdin)) != EOF){
    int n = 0;
    // while((n = read(STDIN_FILENO,buf,4096))!=-1){
    //     for(int i =0; i < n; i++){
    //         if(isupper(buf[i])){
    //             buf[i] = tolower(buf[i]);
    //         }
    //     }
    //     printf("%s\n",buf);

    //     // c = buf[0];
    //     // if(isupper(c)){
    //     //     c = tolower(c);
    //     // }
    //     // if(fputc(c,stdout) == EOF){
    //     //     printf("wrong");
    //     //     exit(1);
    //     // }
    //     //  printf("feng\n");
    //     // if(c == '\n'){
    //     //     fflush(stdout);
    //     // }

    // }

        if(setvbuf(stdin, NULL, _IOLBF,0) != 0){
            printf("setvbuf error\n");
            exit(1);
        }
        if(setvbuf(stdout, NULL, _IOLBF,0) != 0){
            printf("setvbuf error\n");
        }

       while( fgets(buf, 4096, stdin) != NULL){
            //buf[n] = '\0';
            printf("in child : %s",buf);
        }
    printf("n : %d",n);
}
