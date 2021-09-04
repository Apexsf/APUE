// #define _POSIX_C_SOURCE  200112L

// #include <sys/socket.h>
// #include <netdb.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <errno.h>
// void perr(const char* str){
//     printf("error : %s\n",str);
//     exit(1);
// }

// int main(int argc, char* argv[]){
//     struct addrinfo *ailist, *aip;
//     struct addrinfo hint;
//     struct sockaddr_in *sinp;

//     argc = 3;
//     char* argv_[3];
//     argv_[0] = "getaddrinfo";
//     argv_[1] = "www.baidu.com";
//     argv_[2] = NULL;
//     argv = argv_;

//     const char *addr;
//     int err;
//     char abuf[INET_ADDRSTRLEN];

//     if (argc != 3){
//         printf("usage : %s nodename service\n",argv[0]);
//         exit(1);
//     }
//     hint.ai_flags = AI_CANONNAME;
//     hint.ai_family = 0;
//     hint.ai_socktype = 0;
//     hint.ai_protocol = 0;
//     hint.ai_addrlen = 0;
//     hint.ai_canonname = NULL;
//     hint.ai_addr = NULL;
//     hint.ai_next = NULL;


//     if((err = getaddrinfo(argv[1],argv[2],&hint,&ailist)) != 0){
//         printf("%s\n",gai_strerror(err));
//         perr("getaddrinfo");
//     }
//     exit(0);

// }
