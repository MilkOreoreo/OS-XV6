#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int parent_fd[2], child_fd[2];
    //create two new pipes
    pipe(parent_fd);
    pipe(child_fd);
    
    if(fork() == 0) //child process
    {
        char messages[5];
        read(parent_fd[0], messages, sizeof messages);
        int pid = getpid();
        printf("%d: received %s\n", pid, messages);
        write(child_fd[1], "pong", 4);
    }
    else //parent process
    {
        char messages[5];
        int pid = getpid();
        write(parent_fd[1], "ping", 4);
        read(child_fd[0], messages, sizeof messages);
        printf("%d: received %s\n", pid, messages);
    }
    exit(0);
}