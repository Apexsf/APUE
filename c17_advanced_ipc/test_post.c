
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
#include <stdarg.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXLINE 4096
#define MAXMSZ 512

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
err_sys(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	exit(1);
}

void
err_exit(int error, const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, error, fmt, ap);
	va_end(ap);
	exit(1);
}

struct mymesg {
long mtype;
char mtext[MAXMSZ];
};
int
main(int argc, char *argv[])
{
key_t key;
long qid;
size_t nbytes;
struct mymesg m;
if (argc != 3) {
fprintf(stderr, "usage: sendmsg KEY message\n");
exit(1);
}
key = strtol(argv[1], NULL, 0);
if ((qid = msgget(key, 0)) < 0)
err_sys("can’t open queue key %s", argv[1]);
memset(&m, 0, sizeof(m));
strncpy(m.mtext, argv[2], MAXMSZ-1);
nbytes = strlen(m.mtext);
m.mtype = 1;
if (msgsnd(qid, &m, nbytes, 0) < 0)
err_sys("can’t send message");
exit(0);
}