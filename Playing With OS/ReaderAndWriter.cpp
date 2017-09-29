/*
Write a multi-threaded C program that gives readers priority over writers concerning a shared (global) variable.
Essentially, if any readers are waiting, then they have priority over writer threads -- writers can only write when there are no readers.
This program should adhere to the following constraints:

	Multiple readers/writers must be supported (5 of each is fine)
	Readers must read the shared variable X number of times
	Writers must write the shared variable X number of times
	Readers must print:
		The value read
		The number of readers present when value is read
	Writers must print:
		The written value
		The number of readers present were when value is written (should be 0)
	Before a reader/writer attempts to access the shared variable it should wait some random amount of time
		Note: This will help ensure that reads and writes do not occur all at once
	Use pthreads, mutexes, and condition variables to synchronize access to the shared variable

*/
#include <stdio.h>
#include <pthread.h>
#define BS 8 //BUFFER SIZE = BS
#define READER 10  //no. of reader threads
#define WRITER 10 //no. of writer threads
int buf[BS];
int num = 0,add=0;    //add points to where to write more data, num is no. of elements in the buffer
int count=0;
int num_of_read=0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; //mutex to control read and write
pthread_cond_t read = PTHREAD_COND_INITIALIZER;  //conditional variable
pthread_cond_t write = PTHREAD_COND_INITIALIZER; // cv - write

void * reader(void *param); //function for readers nd writter 
void * writer(void * param);

int main()
{
int i,rid[READER],wid[WRITER];
pthread_t rtid[READER]; 
pthread_t wtid[WRITER];


for(i=0;i<WRITER;i++){
	wid[i]=i;
	pthread_create(&wtid[i],NULL,writer,&wid[i]);
}
for (i=0;i<READER;i++){		
	rid[i] = i;    //to number reader threads by Ids
	pthread_create(&rtid[i],NULL,reader,&rid[i]);
	
}

for (i=0;i<READER;i++){
	pthread_join(rtid[i],NULL);	
	printf("ALL READER THREADS JOINED\n");
	}
for (i=0;i<WRITER;i++){
	pthread_join(wtid[i],NULL);
	printf("ALL WRITER THREADS JOINED\n");
}
return 0;
}

void * reader(void * arg){
	int * a = (int *) arg;
	int id = * a;
	pthread_mutex_lock(&m);
			//condition to put
		num_of_read++;   //number of reader waiting...............
		while(count == -1){
			pthread_cond_wait(&read,&m);   //wait for writer to broadcast
		}
		count++;                    
	pthread_mutex_unlock(&m);
//  CRITICAL SECTION OF CODE    
	int i=0;
	for(i=0;i<BS;i++)
	{
		printf("READING....from %d thread ",id);
		printf(" %d\n",buf[i]);
	}
	// Exit critical section
	pthread_mutex_lock(&m);
		count--;
		num_of_read--;
		if (count==0){
			pthread_cond_signal(&write);        //signal write its job is done
		}
	pthread_mutex_unlock(&m);
	
}
void * writer(void * arg){
	int * a = (int *) arg;
	int id = * a;
	pthread_mutex_lock(&m);
		while (count>0)
		{
			pthread_cond_wait(&write,&m);
		}
		count = -1;
	pthread_mutex_unlock(&m);
	//Critical section
	int i;
	 for(i=0;i<BS;i++){
	 	printf("Writing from thread %d:\n",id);
		 buf[i] = id;
	 	}
	pthread_mutex_lock(&m);
	count++;
	if (num_of_read==0)
	{
	pthread_cond_signal(&write);
	}
	else{
		pthread_cond_broadcast(&read);
	}
	pthread_mutex_unlock(&m);
}


