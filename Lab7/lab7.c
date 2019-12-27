#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <errno.h>

#define READERS_COUNT 10
#define BUFFER_SIZE 10


static pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void* reader(void *arg);
void* writer(void *arg);

void* writer(void *arg)
{
    
    char * counter = (char*)arg;
    while(1)
    {
        pthread_rwlock_trywrlock(&rwlock);
        printf("Writer updates.\n");
        (*counter)++;

        pthread_rwlock_unlock(&rwlock);
	    sleep(1);
    }    
}

void* reader(void *arg)
{
    const char* counter = (char*) arg;
    while(1)
    {
  
        pthread_rwlock_rdlock(&rwlock);
        printf("\tTID(%lu)=%d\n", pthread_self(), *counter);
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
     }
}

int main ()
{
    // int counter = 0; Used for tests
    char buffer[BUFFER_SIZE];
    buffer[0]=0;
	pthread_t read [READERS_COUNT];
    pthread_t write;

    pthread_rwlock_init(&rwlock, NULL);


    pthread_create(&write, NULL, writer, buffer);
	for (int i = 0; i<READERS_COUNT; ++i)
	{
		pthread_create(&read[i], NULL, reader, buffer);
	}
          
    pthread_join(write, NULL);
    for (int i = 0; i<READERS_COUNT; ++i)
	{
        pthread_join(read[i], NULL);
	}

    pthread_rwlock_destroy(&rwlock);
        
	return 0;
}

