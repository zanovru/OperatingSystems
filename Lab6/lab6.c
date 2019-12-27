#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define PT_NUM 9
#define BUFFER_SIZE 10


static pthread_mutex_t mutex;

void* reader(void *arg);
void* writer(void *arg);

void* writer(void *arg)
{
    char * counter = (char*)arg;
     while(1)
     {
        pthread_mutex_lock(&mutex);
        printf("Writer updates.\n");
        (*counter)++;

        pthread_mutex_unlock(&mutex);
	    sleep(1);
     }
     
}

void* reader(void *arg)
{
     const char* counter = (char*) arg;

     while(1)
     {
        pthread_mutex_lock(&mutex);
        printf("\tTID(%lu)=%d\n", pthread_self(), *counter);
        pthread_mutex_unlock(&mutex);
	    sleep(1);
     }
}

int main ()
{
    // int counter = 0; Used for tests
    char buffer[BUFFER_SIZE];
    buffer[0]=0;
	pthread_t read [PT_NUM];
    pthread_t write;

    pthread_create(&write, NULL, writer, buffer);
	for (int i = 0; i<PT_NUM; ++i)
	{
		pthread_create(&read[i], NULL, reader, buffer);
	}
                
    pthread_join(write, NULL);
    for (int i = 0; i<PT_NUM; ++i)
	{
        pthread_join(read[i], NULL);
	}
        
	return 0;
}

