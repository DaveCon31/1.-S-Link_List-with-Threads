#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "ll_API.h"

#define NUM_THREADS 3

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_barrier_t barrier;					//threads sync for creation using barrier
int current = 0;

int validation_fn(void *data) 
{
	int int_data = *(int*)data;
	
	if (int_data < 0)
		return -1;
	
	if (int_data > 9999)
		return -1;
		
	return 0;
}

void print_int(void *element)
{
	printf("%d ", *(int *)element);
}

void print_float(void *element)
{
	printf("%f ", *(float *)element);
}

void print_double(void *element)
{
	printf("%lf ", *(double *)element);
}

void *sync_routine(void *arg)				
{	
	ll_t l1, l2, l3;
	int internal_tid = *(int*) arg;
	printf("Waiting at the barrier... Thread no: %d\n", internal_tid+1);
	pthread_barrier_wait(&barrier);			//wait to create all threads before executing 
	
	pthread_mutex_lock(&lock);
	printf("We passed the barrier! Thread no: %d\n", internal_tid+1);
	pthread_mutex_unlock(&lock);
	
	pthread_mutex_lock(&lock);
	while (internal_tid > current) {
		pthread_cond_wait(&cond, &lock);	
	}
	
	switch(internal_tid) {
		case 0:
			ll_create(&l1, print_int);
			printf("\nThread: %d ===================== \n",internal_tid+1);
			ll_add_end(&l1, *(int){2});
			ll_add_end(&l1, *(int){4});
			
			ll_print_list(&l1);
			ll_flush_list(&l1);
			ll_print_list(&l1);
			break;
			
		case 1:
			ll_create(&l2, print_float);
			printf("\nThread: %d ===================== \n",internal_tid+1);
			ll_add_end(&l2, *(float){11.0001});
			ll_add_end(&l2, *(float){1.18});
			
			ll_print_list(&l2);
			ll_flush_list(&l2);
			ll_print_list(&l2);
			break;
		
		case 2:
			ll_create(&l3, print_double);
			printf("\nThread: %d ===================== \n",internal_tid+1);
			ll_add_end(&l3, *(double){30.123215671231});
			ll_add_end(&l3, *(double){25821.12341});
			
			ll_print_list(&l3);
			ll_flush_list(&l3);
			ll_print_list(&l3);
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
	callback_validate = validate;
	
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
	/*ll_print_list();
	ll_flush_list();
	ll_print_list();*/
	return 0;
}



	
	


