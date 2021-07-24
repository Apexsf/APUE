#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[]){
    struct stat stat_buf;
    char* str_buf;
    for(int i = 1; i < argc; ++i){
        if(lstat(argv[i],&stat_buf) != 0){
            printf("get stat error : %s\n", strerror(errno));
            continue;
        }
        if (S_ISREG(stat_buf.st_mode)){
            str_buf = "regular";
        }else if (S_ISDIR(stat_buf.st_mode)){
            str_buf = "directory";
        }else if (S_ISCHR(stat_buf.st_mode)){
            str_buf = "character special";
        }else if(S_ISBLK(stat_buf.st_mode)){
            str_buf = "block special";
        }else if(S_ISFIFO(stat_buf.st_mode)){
            str_buf = "fifo";
        }else if(S_ISLNK(stat_buf.st_mode)){
            str_buf = "symbolic link";
        }else if(S_ISSOCK(stat_buf.st_mode)){
            str_buf = "socket";
        }else {
            str_buf = "unknown mode";
        }
        printf("%s : %s\n",argv[i],str_buf);
    }
}