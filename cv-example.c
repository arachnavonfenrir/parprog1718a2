// Condition variable usage example.
// Compile with: gcc -O2 -Wall -pthread cv-example.c -o cv-example

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#define MESSAGES 20





struct messanger {
	int id;
	int *start,*end//the start and the end of the sorting tables
	int n;
	int flag=0;//condiotions to be messaged(0.-create threads, 1.-Join tables 2.-join threads and end)
};




void *work(Void *pool){
	
	//call qucksort table div 
	//if(n="some small num")
	pool->flag=2;//calls quicksort
	//merge back the tables
	pool->flag=3;//make the threads join
}

// global integer buffer
int global_buffer;
// global avail messages count (0 or 1)
int global_availmsg = 0;	// empty

// condition variable, signals a put operation (receiver waits on this)
pthread_cond_t msg_in = PTHREAD_COND_INITIALIZER;
// condition variable, signals a get operation (sender waits on this)
pthread_cond_t msg_out = PTHREAD_COND_INITIALIZER;

// mutex protecting common resources
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


// producer thread function
void *producer_thread(void *args) {
  int i;
  
  // send a predefined number of messages
  for (i=0;i<MESSAGES;i++) {
    // lock mutex
    pthread_mutex_lock(&mutex);
    while (global_availmsg>0) {	// NOTE: we use while instead of if! more than one thread may wake up
    				
      pthread_cond_wait(&msg_out,&mutex);  // wait until a msg is received - NOTE: mutex MUST be locked here.
      					   // If thread is going to wait, mutex is unlocked automatically.
      					   // When we wake up, mutex will be locked by us again. 
    }
    // send message
    printf("Producer: sending msg %d\n",i);
    global_buffer = i;
    global_availmsg = 1;
    
    // signal the receiver that something was put in buffer
    pthread_cond_signal(&msg_in);
    
    // unlock mutex
    pthread_mutex_unlock(&mutex);
  }
  
  // exit and let be joined
  pthread_exit(NULL); 
}
  
  
// receiver thread function
void *consumer_thread(void *args) {
  int i;
  
  // receive a predefined number of messages
  for (i=0;i<MESSAGES;i++) {
    // lock mutex
    pthread_mutex_lock(&mutex);
    while (global_availmsg<1) {	// NOTE: we use while instead of if! more than one thread may wake up
      pthread_cond_wait(&msg_in,&mutex); 
    }
    // receive message
    printf("Consumer: received msg %d\n",global_buffer);
    global_availmsg = 0;
    
    // signal the sender that something was removed from buffer
    pthread_cond_signal(&msg_out);
    
    // unlock mutex
    pthread_mutex_unlock(&mutex);
  }
  
  // exit and let be joined
  pthread_exit(NULL); 
}


int main() {
  
  pthread_t producer[4],consumer[4];
 struct messanger pool[4];
 


// create threads
	for(int i=0;i<4;i++){
		pool[i].id=i;
  pthread_create(&producer[i],NULL,producer_thread,NULL);
  pthread_create(&consumer[i],NULL,consumer_thread,NULL);
  }
  // then join threads
  if(pool[1].flaag=2/* message of destroy*/){
	  for(int i=0;i<4;i++){
  pthread_join(producers[i],NULL);
  pthread_join(consumers[i],NULL);
	}

	
  // destroy mutex - should be unlocked
  pthread_mutex_destroy(&mutex);
 	 // destroy cvs - no process should be waiting on these
 	 pthread_cond_destroy(&msg_out);
  	pthread_cond_destroy(&msg_in);
 }
  return 0;
}
