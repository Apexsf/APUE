#include <unistd.h>
#include <stdio.h>

size_t readn(int fd, void* ptr,size_t size){
    size_t left = size;
    size_t nread;
    while(left > 0){
        if ((nread = read(fd, ptr,size)) < 0){
            if (left == size) return -1;
            else break;
        }else if (nread == 0){
                break;
        }
        left -= nread;
        ptr += nread;
    }
    return size - left;
}

size_t writen(int fd, void* ptr, size_t size){
    size_t left = size;
    size_t nwrite;
    while(left > 0){
        if((nwrite = write(fd,ptr,size)) < 0){
            if(left == size) return -1;
            else break;
        } else if (nwrite == 0){
            break;
        }
        left -= nwrite;
        ptr += nwrite;
    }
    return size - left;
}

int main(){

}