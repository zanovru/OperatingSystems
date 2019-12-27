#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define TIME_BUFFER_SIZE_STR 25
#define SEMAPHORE_NAME "/semaphore1"

typedef struct _sh_value
{
    char str[TIME_BUFFER_SIZE_STR];
    pid_t pid;
} sh_value;

#define SH_FILE "shmfile"
  
int main() 
{ 
    sem_t *sem;
    key_t key = ftok(SH_FILE,65); 
    if(key == -1)
    {
        printf("key errno:%d\n", errno);
        return 1;
    }
  
    
    int shmid = shmget(key,sizeof(sh_value),0666|IPC_CREAT); 
        
    if(shmid == -1)
    {
        printf("shmid errno:%d\n", errno);
        return 1;
    }

    sh_value local_value;

    if( (sem = sem_open(SEMAPHORE_NAME,0)) == SEM_FAILED)
    {
        printf("sem_open: errno: %d", errno);
        return 1;
    }

    while(1)
    {
        
        sh_value* value = (sh_value*) shmat(shmid,(void*)0,0); 
        if(value == (sh_value*)-1)
        {
            printf("shmat errno:%d\n", errno);
            return 1;
        }

        memcpy(&local_value, value, sizeof(sh_value));
    
        printf("Data read from memory: %d %s\n", value->pid, value->str); 
        
        
        if(shmdt(value) != 0)
        {
            printf("shmdt errno:%d\n", errno);
            return 1;
        }
        
        if(sem_wait(sem) < 0)
        {
            printf("sem_wait errno:%d\n", errno);
            return 1;
        }
    }
} 
