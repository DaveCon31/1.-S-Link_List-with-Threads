#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include "ll_API.h"
#include <time.h>

enum { NUM_THREADS = 3
};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_barrier_t barrier;					//threads sync for creation using barrier
int current = 0;

void *sync_routine(void *arg)				
{	

	
	int internal_tid = *(int*) arg;
	printf("Waiting at the barrier... Thread no: %d\n", internal_tid+1);
	pthread_barrier_wait(&barrier);			//wait to create all threads before executing 
	
	pthread_mutex_lock(&lock);
	printf("We passed the barrier! Thread no: %d\n", internal_tid+1);
	pthread_mutex_unlock(&lock);
	
	pthread_mutex_lock(&lock);
	while(internal_tid > current)	pthread_cond_wait(&cond, &lock);	
	
	switch(internal_tid)
	{
		case 0:
			delete(2);
			sort_list();
			flush_list();
			print_list();
			printf("\nThread: %d ===================== \n",internal_tid+1);
			add(2);
			print_list();
			add(4);
			print_list();
			add(10);
			print_list();
			delete(2);
			print_list();
			sort_list();
			print_list();
			delete(10);
			print_list();
			delete(5);
			print_list();
			break;
			
		case 1:
			printf("\nThread: %d ===================== \n",internal_tid+1);
			add(11);
			print_list();
			add(1);
			print_list();
			delete(11);
			print_list();
			add(8);
			print_list();
			break;
		
		case 2:
			printf("\nThread: %d ===================== \n",internal_tid+1);
			add(30);
			print_list();
			add(25);
			print_list();
			add(100);
			print_list();
			sort_list();
			print_list();
			delete(100);
			print_list();
			break;
	
	}
	current++;
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&lock);
	free(arg);
	printf("Thread exited?!\n");
	return (void*)0;
}
	
	

int main()
{
	pthread_t th[NUM_THREADS-1];
	int i;
	pthread_barrier_init(&barrier, NULL, NUM_THREADS);
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);
	
	callback_validate = validate;		//set function pointer to validate head
		
	for (i = 0; i < NUM_THREADS; i++) {
		int *a = malloc(sizeof(int)); 
		*a = i;
		if (pthread_create(&th[i], NULL, &sync_routine, a) != 0) {
			perror("Failed to create thread!\n");
		}
	}
	
	for (i = 0; i < NUM_THREADS; i++) {
		if (pthread_join(th[i], NULL) != 0) {
			perror("Failed to join thread!\n");
		}
	}
	
	pthread_barrier_destroy(&barrier);
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);
	
	printf("\n----------------------------\nThreads executions finished!\n");
	print_list();
	flush_list();
	print_list();
	return 0;
}



	
	


