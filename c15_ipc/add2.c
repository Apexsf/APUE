#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define LINESIZE 128


// int main(){
//     char line[LINESIZE];
//     int n, int1, int2;
//     while((n = read(STDIN_FILENO,line,LINESIZE )) > 0){
//         line[n] = '\0';
//         if(sscanf(line, "%d%d", &int1, &int2) == 2){
//             sprintf(line, "%d\n", int1+int2);
//             n = strlen(line);
//             if(write(STDOUT_FILENO,line,n) != n){
//                 printf("write error\n");
//                 exit(1);
//             }
//         }else{
//             write(STDOUT_FILENO,"invalid args\n", 13);
//             printf("write error\n");
//             exit(1);
//         }
//     }
// }


int
main(void)
{
    if(setvbuf(stdin, NULL, _IOLBF,0) != 0){
        printf("setvbuf error\n");
        exit(1);
    }
    if(setvbuf(stdout, NULL, _IOLBF,0) != 0){
        printf("setvbuf error\n");
    }
    int int1, int2;
    char line[LINESIZE];
    while (fgets(line, LINESIZE, stdin) != NULL) {
        if (sscanf(line, "%d%d", &int1, &int2) == 2) {
            if (printf("%d\n", int1 + int2) == EOF){
                printf("printf error");
                exit(1);
            }
            
        } else {
            if (printf("invalid args\n") == EOF){
                printf("printf error");
                exit(1);
            }
        }
    }
    exit(0);
}