#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


// int main(){
//     int fork_id = fork();
//     int fd;
//     const char* write_str = "hello world!\n";
//     fd = open("./ao.txt",O_APPEND | O_CREAT | O_RDWR);
//     printf("%d\n",getpid());
//     if (write(fd, "hello world!\n",strlen(write_str)) < 0){
//         printf("write error");
//     }
// }

int main(){
    int fd;
    __off_t t;
    const char* write_str = "123456789\n";
    fd  = open("./ao.txt",O_CREAT | O_RDWR);
    lseek(fd,0,SEEK_END);
    for (int i = 0; i < 10; i++){
        sleep(1);
        t = lseek(fd, 0, SEEK_CUR);
        printf("%d : %d\n",i,t);
        if(pwrite(fd,write_str,strlen(write_str), lseek(fd,0,SEEK_END)) != strlen(write_str)){
            printf("write error");
        }
    }
    exit(0);
}