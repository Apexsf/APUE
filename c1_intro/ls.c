#include "../include/apue.h"
#include <dirent.h>
#include <errno.h>
#include <stdarg.h>	

static void
err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
	char	buf[MAXLINE];

	vsnprintf(buf, MAXLINE-1, fmt, ap);
	if (errnoflag)
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf)-1, ": %s",
		  strerror(error));
	strcat(buf, "\n");
	fflush(stdout);		/* in case stdout and stderr are the same */
	fputs(buf, stderr);
	fflush(NULL);		/* flushes all stdio output streams */
}

void
err_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap);
	exit(1);
}

void
err_sys(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	exit(1);
}


int main(int argc, char* argv[]){
    argc = 2;
    char* argv_[2];
    argv_[0] = "./ls";
    argv_[1] = "./";
    argv = argv_;
    DIR * dp;
    struct dirent *dirp;
    if(argc != 2){
        err_quit("usage:is directory_name");
    }
    if((dp = opendir(argv[1])) == NULL){
        err_sys("can't open %s", argv[1]);
    }
    while((dirp = readdir(dp)) != NULL){
        printf("%s\n",dirp->d_name);
    }
    closedir(dp);
    exit(0);
}