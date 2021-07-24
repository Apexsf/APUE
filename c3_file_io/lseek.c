// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>

// int main(){
//     char buffer[4096];
//     int f_read = open("./tex.txt",O_RDWR);
//     off_t cursor_p;

//     cursor_p = lseek(f_read,3,SEEK_SET);
//     printf("the current position of cursor is : %d\n",cursor_p);
//     read(f_read,buffer,10);
//     printf(buffer);
//     printf("\n");
//     fflush(stdout);

//     cursor_p = lseek(f_read,-5,SEEK_CUR);
//     printf("the current position of cursor is : %d\n",cursor_p);
//     read(f_read,buffer,10);
//     printf(buffer);
//     printf("\n");
//     fflush(stdout);

//     cursor_p = lseek(f_read,-10,SEEK_END);
//     printf("the current position of cursor is : %d\n",cursor_p);
//     read(f_read,buffer,10);
//     printf(buffer);
//     printf("\n");
//     fflush(stdout);
// }


// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>

// int main(){
//     int f_read = open("./tex.txt",O_RDWR);
//     char buffer[4096];

//     printf("first read and output:\n");
//     read(f_read,buffer,10);
//     printf(buffer);
//     printf("\n");
//     fflush(stdout);
    
    
//     printf("second read and output:\n");
//     read(f_read,buffer,10);
//     printf(buffer);
//     printf("\n");
// }

// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <errno.h>
// #include <string.h>
// int main(void)
// {
// if(lseek(STDIN_FILENO, 0, SEEK_CUR) == -1){
//     printf("cannot seek\n");
//     printf("The errno is : %s\n", strerror(errno));
// }else{
//     printf("seek OK\n");
// }
// exit(0);
// }

//#include "apue.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define	FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

char	buf1[] = "abcdefghij";
char	buf2[] = "ABCDEFGHIJ";

int main(void)
{
	int	fd;
    if ((fd = creat("file.hole",FILE_MODE)) < 0){
        printf("open error\n");
        exit(1);
    }
    if (write(fd, buf1, 10)!=10){
        printf("write error");
        exit(1);
    }

    if (lseek(fd,100,SEEK_SET) == -1){
        printf("seek error");
        exit(1);
    }

    if (write(fd,buf2,10) != 10){
        printf("write error");
        exit(1);
    }
	exit(0);
}
