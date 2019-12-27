// Use Makefile for compiling
// clang version 7.0.0-3 (tags/RELEASE_700/final)
// Target: x86_64-pc-linux-gnu
// Thread model: posix
// InstalledDir: /usr/bin
// Found candidate GCC installation: /usr/bin/../lib/gcc/x86_64-linux-gnu/8
// Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/8
// Selected GCC installation: /usr/bin/../lib/gcc/x86_64-linux-gnu/8
// Candidate multilib: .;@m64
// Selected multilib: .;@m64

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

// Only for killing child process in specific situations
// #include <wait.h>
// #include <signal.h>

#define FIFO_PATH "/tmp/time_fifo"

#define TIME_BUFFER_SIZE_STR 25

typedef struct _fifo_value
{
    char str[TIME_BUFFER_SIZE_STR];
    pid_t pid;
} fifo_value;

int main()
{

    if(mkfifo(FIFO_PATH, 0666) == -1)
    {
        printf("Can't make FIFO with name %s\n", FIFO_PATH);
        exit(1);
    }
    else
    {
        printf("FIFO with name %s created successfully.\n", FIFO_PATH);
    }
    

     pid_t child_pid;
     int fifo_fd;
    if((child_pid = fork()) != 0)
    {
        // Parent pid
        fifo_value value;
        time_t t = time(NULL);
        strncpy((char*)value.str, ctime(&t), TIME_BUFFER_SIZE_STR-1);
        value.str[TIME_BUFFER_SIZE_STR-1]='\0';
        value.pid = getpid();
        printf("It's parent process\n");
        printf("\tPPID: %d\n\tPID: %d\n",getppid(), getpid());
        fifo_fd = open(FIFO_PATH, O_WRONLY);
        if(fifo_fd == -1)
        {
            printf("Can't open FIFO %x\n", fifo_fd);
            int res = 0;
            kill(child_pid, SIGINT);
            waitpid(child_pid, &res, 0);
            unlink(FIFO_PATH);
            exit(1);
        }
        write(fifo_fd, &value, sizeof(fifo_value));
        // printf("\tWritten time is %s\n", value.str);
        close(fifo_fd);
        unlink(FIFO_PATH);
    }
    else
    {
        // Forked pid
        printf("It's child process\n");
        printf("\tPPID: %d\n\tPID: %d\n",getppid(), getpid());
        fifo_fd = open(FIFO_PATH, O_RDONLY);
        if(fifo_fd == -1)
        {
            printf("Can't open FIFO %x\n", fifo_fd);            
            exit(1);
        }
        fifo_value value;
        read(fifo_fd, &value, sizeof(fifo_value));
        sleep(1);
        time_t lt = time(NULL);
        printf("\tTime is %s from pid: %d\n\tThis thread time is %s", value.str, value.pid, ctime(&lt));
        close(fifo_fd);
    }
    waitpid(child_pid,0,0);
    return 0;
}
