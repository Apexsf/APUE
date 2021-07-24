// #include <fcntl.h>
// #include <stdio.h>

// int main(int argc, char *argv[])
// {
// 	int	val;
// 	if (argc != 2){
// 		printf("error arg!\n");
//         exit(1);

//     }

// 	if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0){
// 		printf("fcntl error for fd %d\n", atoi(argv[1]));
//         exit(1);
//     }

// 	switch (val & O_ACCMODE) {
// 	case O_RDONLY:
// 		printf("read only");
// 		break;

// 	case O_WRONLY:
// 		printf("write only");
// 		break;

// 	case O_RDWR:
// 		printf("read write");
// 		break;

// 	default:
// 		printf("unknown access mode\n");
// 	}

// 	if (val & O_APPEND)
// 		printf(", append");
// 	if (val & O_NONBLOCK)
// 		printf(", nonblocking");
// 	if (val & O_SYNC)
// 		printf(", synchronous writes");

// #if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
// 	if (val & O_FSYNC)
// 		printf(", synchronous writes");
// #endif

// 	putchar('\n');
// 	exit(0);
// }

#include <fcntl.h>
#include <stdio.h>
void
set_fl(int fd, int flags) /* flags are file status flags to turn on */
{
	int		val;

	if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		printf("fcntl F_GETFL error\n");

	val |= flags;		/* turn on flags */

	if (fcntl(fd, F_SETFL, val) < 0)
		printf("fcntl F_SETFL error\n");
}

int main(){
    
}