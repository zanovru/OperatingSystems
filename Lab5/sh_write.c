#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

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
    sem_t* sem;
    key_t key = ftok(SH_FILE, 65); 
    
    int shmid = shmget(key,sizeof(sh_value),0666|IPC_CREAT); 
    
    sh_value *str;
  
    sh_value value;
    time_t t;
    str = (sh_value*) shmat(shmid,(void*)0,0); 

    if( (sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0777,0)) == SEM_FAILED)
    {
        printf("sem_open: errno: %d", errno);
        return 1;
    }
    while(1)
    {
        t = time(NULL);
        strncpy((char*)value.str, ctime(&t), TIME_BUFFER_SIZE_STR-1);
        value.str[TIME_BUFFER_SIZE_STR-1]='\0';
        value.pid = getpid();
        printf("%s %d\n", value.str, value.pid);

        *str = value;
        sem_post(sem);
        sleep(1);
    }  

} 
