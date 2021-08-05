#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>

#define BSZ 4096

unsigned char buf[BSZ];

unsigned char translate(unsigned char c){
    if(isalpha(c)){
        if(c > 'n'){
            c -= 13;
        } else if(c >= 'a'){
            c += 13;
        } else if (c >= 'N'){
            c -= 13;
        } else {
            c += 13;
        }
    }
    return c;
}

int main(int argc, char* argv[]){
    int ifd, ofd, i, n , nw;
    if (argc != 3){
        printf("usage: rot13 infile outfile\n");
        exit(1);
    }
    if((ifd = open(argv[1], O_RDONLY)) < 0){
        printf("can't open %s\n",argv[1]);
        exit(1);
    }
    if((ofd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC,S_IRWXU)) < 0){
        printf("can't create %s\n",argv[2]);
        exit(1);
    }
    while ((n = read(ifd, buf, BSZ)) > 0){
        for(int i = 0; i < n; i++){
            buf[i] = translate(buf[i]);
        }
        if((nw = write(ofd, buf, n)) != n){
            if(nw < 0){
                printf("write failed\n");
                exit(1);
            }else{
                printf("short write (%d/%d)\n",nw,n);
                exit(1);
            }
        }
    }
    fsync(ofd);
    exit(0);
}