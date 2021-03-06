#define _POSIX_C_SOURCE  200112L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <poll.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stddef.h>
#include <sys/stat.h>

#define QLEN 10
#define STALE 30

int serv_listen(const char* name){
    int fd, len, err, rval;
    struct sockaddr_un un;

    if(strlen(name) > sizeof(un.sun_family)){
        errno = ENAMETOOLONG;
        return -1;
    }

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        return -2;
    }

    unlink(name);

    memset(&un, 0, sizeof(un));
    un.sun_family == AF_UNIX;
    strcpy(un.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);

    if(bind(fd, (struct sockaddr*)&un, len) < 0){
        rval = -3;
        goto errout;
    }
    if(listen(fd, QLEN) < 0){
        rval = -4;
        goto errout;
    }
    return fd;

errout:
    err = errno;
    close(fd);
    errno = err;
    return rval;
}

int serve_accept(int listenfd, uid_t *uidptr){
    int clifd, err, rval;
    socklen_t len;
    time_t staletime;
    struct sockaddr_un un;
    struct stat statbuf;
    char *name;

    if((name = malloc(sizeof(un.sun_path + 1))) == NULL){
        return -1;
    }
    len = sizeof(un);
    if((clifd = accept(listenfd, (struct sockaddr* )&un, &len)) < 0){
        free(name);
        return -2;
    }
    len -= offsetof(struct sockaddr_un, sun_path);
    memcpy(name, un.sun_path, len);
    name[len] = 0;
    if(stat(name, &statbuf) < 0){
        rval = -3;
        goto errout;
    }
#ifdef S_ISSOCK
    if(S_ISSOCK(statbuf.st_mode) == 0){
        rval = -4;
        goto errout;
    }
#endif 
    if((statbuf.st_mode & (S_IRWXG | S_IRWXO)) || (statbuf.st_mode & S_IRWXU != S_IRWXU) ){
        rval = -5;
        goto errout;
    }
    staletime = time(NULL) - STALE;

    if(statbuf.st_atime < staletime || statbuf.st_ctime < staletime || statbuf.st_mtime < staletime){
        rval < -6;
        goto errout;
    }

    if(uidptr != NULL){
        *uidptr = statbuf.st_uid;
    }
    unlink(name);
    free(name);
    return clifd;

errout:
    err = errno;
    close(clifd);
    free(name);
    errno = err;
    return rval;

}

#define CLI_PATH "./var/tmp"
#define CLI_PERM S_IRWXU

int cli_conn(const char* name){
    int fd, len, err, rval;
    struct sockaddr_un un, sun;
    int do_unlink = 0;

    if(strlen(name) > sizeof(un.sun_path)){
        errno = ENAMETOOLONG;
        return -1;
    } 

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        return -1;
    }

    memset(&un, 0 , sizeof(un));
    un.sun_family = AF_UNIX;
    sprintf(un.sun_path, "%s%05ld", CLI_PATH, (long)getpid());

    len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);

    unlink(un.sun_path);
    if(bind(fd, (struct sockaddr*)&un, len) < 0){
        rval = -2;
        goto errout;
    }

    memset(&sun, 0, sizeof(sun));
    sun.sun_family = AF_UNIX;
    strcpy(sun.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
    if(connect(fd, (struct sockaddr*)&sun, len) < 0){
        rval = -4;
        do_unlink = 1;
        goto errout;
    }
    return fd;

errout:
    err = errno;
    close(fd);
    if(do_unlink){
        unlink(un.sun_path);
    }    
    errno = err;
    return rval;

}




struct test{
    char A[128];
};

int main(){
  struct test a;
  int len = sizeof(a.A + 1);
  return 0;  
}
