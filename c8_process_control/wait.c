// #include <sys/types.h>
// #include <sys/wait.h>
// #include <stdlib.h>
// #include <stdio.h>

// void pr_exit(int status) {
//     if (WIFEXITED(status))
//         printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
//     else if (WIFSIGNALED(status))
//         printf("abnormal termination, signal number = %d%s\n",WTERMSIG(status),
//     #ifdef WCOREDUMP 
//         WCOREDUMP(status) ? " (core file generated)" : "");
//     #else
//         "");
//     #endif
//     else if (WIFSTOPPED(status))
//         printf("child stopped, signal number = %d\n",WSTOPSIG(status));
// }

// int main(){
//     pid_t pid;
//     int status;
//     //test for normal exit
//     if((pid = fork()) < 0){
//         printf("create children process failled\n");
//         exit(0);
//     }else if (pid == 0){
//         exit(87);
//     }
//     if(wait(&status) != pid) printf("wait error !\n");
//     pr_exit(status);

//     //test for abnormal termination by signal
//     if((pid = fork()) < 0){
//         printf("create children process failled\n");
//         exit(0);
//     }else if (pid == 0){
//         abort();
//     }
//     if(wait(&status) != pid) printf("wait error !\n");
//     pr_exit(status);

//     //test for abnormal termination by signal
//     if((pid = fork()) < 0){
//         printf("create children process failled\n");
//         exit(0);
//     }else if (pid == 0){
//         status /= 0; ///* divide by 0 generates SIGFPE */
//     }
//     if(wait(&status) != pid) printf("wait error !\n");
//     pr_exit(status);
// }

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    pid_t pid;
    printf("the parent process id  = %ld\n",(long)getpid());
    if ((pid = fork()) < 0){
        printf("fork failure!\n");
        exit(1);
    }else if(pid == 0){
        if((pid = fork()) < 0){
            printf("fork failure\n");
            exit(1);
        } else if(pid > 0){
            printf("the first children process id  = %ld\n",(long)getpid());
            printf("the first children, parent id = %ld\n",(long)getppid());
            exit(0); // the first child, exit immediately
        }
        //the child of the first child, i.e, the parent is the first child
        sleep(2);
        printf("the second children, parent id = %ld\n",(long)getppid());
        exit(0);
    }
    if (waitpid(pid,NULL,0) != pid){ //wait the first child, which is exit immediately
        printf("wait error!\n");
        exit(1);
    }
    printf("the parent process is done!\n");
    exit(0);
}

// int
// main(void)
// {
// pid_t pid;
// if ((pid = fork()) < 0) {
// printf("fork error");
// } else if (pid == 0) { /* first child */
// if ((pid = fork()) < 0)
// printf("fork error");
// else if (pid > 0)
// exit(0); /* parent from second fork == first child */
// /*
// * We’re the second child; our parent becomes init as soon
// * as our real parent calls exit() in the statement above.
// * Here’s where we’d continue executing, knowing that when
// * we’re done, init will reap our status.
// */
// sleep(2);
// printf("second child, parent pid = %ld\n", (long)getppid());
// exit(0);
// }
// if (waitpid(pid, NULL, 0) != pid) /* wait for first child */
// printf("waitpid error");
// /*
// * We’re the parent (the original process); we continue executing,
// * knowing that we’re not the parent of the second child.
// */
// exit(0);
// }