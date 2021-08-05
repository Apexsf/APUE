#include <stdio.h>
#include <stdlib.h>
#include <aio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define BUFFERSIZE 10
#define RENUM 8
enum aio_state{
    AIO_FREE = 0,
    AIO_READ = 1,
    AIO_WRITE = 2
};

struct aio_helper{
    enum aio_state m_state;
    struct aiocb m_aiocb;
    unsigned char m_buf[BUFFERSIZE];
    int m_islast;   
};


void perr(const char* str){
    printf("Error Encountered on %s\n", str);
    printf("%s\n",strerror(errno));
    exit(1);
}

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


struct aio_helper ah_list[RENUM];

int main(){

    int ifd, ofd;
    off_t off = 0;
    int work_aio = 0;
    struct stat statbuf;
    const struct aiocb* p_ai_list[RENUM];

    if ((ifd = open("./input",O_RDONLY)) < 0) perr("open");
    if ((ofd = open("./out", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU)) < 0) perr("open");
    if(fstat(ifd, &statbuf) < 0) perr("fstat");
    off_t file_size = statbuf.st_size;
    
    //init 
    for(int i = 0; i < RENUM; i++){
        ah_list[i].m_state = AIO_FREE;
        ah_list[i].m_islast = 0;
        ah_list[i].m_aiocb.aio_buf = ah_list[i].m_buf;
        ah_list[i].m_aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
        p_ai_list[i] = NULL;
    }

    while(1){
        for(int i = 0; i < RENUM; i++){
            if(ah_list[i].m_state == AIO_FREE){
                if(off < file_size) {
                    ah_list[i].m_aiocb.aio_fildes = ifd;
                    ah_list[i].m_aiocb.aio_nbytes = BUFFERSIZE;
                    ah_list[i].m_aiocb.aio_offset = off;
                    off += BUFFERSIZE;
                    work_aio++;
                    if(aio_read(&ah_list[i].m_aiocb) < 0) perr("aio_read");
                    p_ai_list[i] = &ah_list[i].m_aiocb;
                    ah_list[i].m_state = AIO_READ;
                    if(off >= file_size){
                        ah_list[i].m_islast = 1;
                    }
                }
            }else if(ah_list[i].m_state == AIO_READ){
                int err = aio_error(&ah_list[i].m_aiocb);
                if (err == EINPROGRESS) continue;
                else if (err != 0)  perr("aio_error");
                int read_size = aio_return(&ah_list[i].m_aiocb);
                if( read_size != BUFFERSIZE && !ah_list[i].m_islast) perr("aio_return");
                for( int j = 0; j < read_size ; j++){
                    ah_list[i].m_buf[j] = translate(ah_list[i].m_buf[j]);
                }

                ah_list[i].m_state = AIO_WRITE;
                ah_list[i].m_aiocb.aio_fildes = ofd;
                ah_list[i].m_aiocb.aio_nbytes = read_size;

                if(aio_write(&ah_list[i].m_aiocb) < 0) perr("aio_write");

            }else if (ah_list[i].m_state == AIO_WRITE){
                int err = aio_error(&ah_list[i].m_aiocb);
                if (err == EINPROGRESS) continue;
                else if (err != 0)  perr("aio_error");
                int write_size = aio_return(&ah_list[i].m_aiocb);
                if( write_size != BUFFERSIZE && !ah_list[i].m_islast) perr("aio_return");

                ah_list[i].m_state = AIO_FREE;
                work_aio--;
                p_ai_list[i] =NULL;
            }
        }

        if(!work_aio) {
            if(off >= file_size){
                break;
            }
        }
        else{
            if(aio_suspend(p_ai_list,RENUM,NULL) < 0) perr("aio_suspend");
        }
    }

    ah_list[0].m_aiocb.aio_fildes = ofd;
    if(aio_fsync(O_SYNC, &ah_list[0].m_aiocb) < 0) perr("aio_fsync\n");
    exit(0);

}
