#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define PAGER "${PAGER:-more}"
#define MAXLINE 4096

int main(int argc, char* argv[]){
    FILE *fpin, *fpout;
    char line[MAXLINE];
    if(argc != 2) exit(1);
    
    if((fpin = fopen(argv[1], "r")) == NULL) exit(1);
    if((fpout = popen(PAGER,"w")) == NULL) exit(1);

    while(fgets(line, MAXLINE, fpin) != NULL){
        if(fputs(line, fpout) == EOF) exit(1);
    }

    if (ferror(fpin)) exit(1);
    if (pclose(fpout) == -1) exit(1);
    
    exit(0);
    
}