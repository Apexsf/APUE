#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>

#define COPYINCR (1024*1024*1024)

void perr(char* str){
    printf("%s\n", str);
    exit(1);
}

int main(int argc, char* argv[]){

    int fdin, fdout;
    void *src, *dst;
    size_t copysz;
    struct stat sbuf;
    off_t fsz = 0;

    if (argc != 3) perr("invalid args");
    if ((fdin = open(argv[1], O_RDONLY)) < 0) perr("open failed");
    if ((fdout =open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) < 0) perr("open failed");
    if(fstat(fdin, &sbuf) < 0) perr("fstat failed");
    if(ftruncate(fdout,sbuf.st_size) < 0) perr("trunc error");

    while (fsz < sbuf.st_size){
        if (sbuf.st_size <= COPYINCR) copysz = sbuf.st_size;
        else copysz = COPYINCR;

        if ((src = mmap(0, copysz, PROT_READ, MAP_SHARED, fdin, fsz)) == MAP_FAILED) perr("mmap failed");
        if((dst = mmap(0, copysz, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, fsz)) == MAP_FAILED) perror("map failed");

        memcpy(dst, src, copysz);
        munmap(src, copysz);
        munmap(dst, copysz);
        fsz += copysz;
    }
    exit(0);
}