#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "ll_API.h"

#define NUM_THREADS 3

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_barrier_t barrier;    //threads sync for creation using barrier
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

int int_comparator(void *first_argument, void *second_argument)
{
	if (*(int *)first_argument > *(int *)second_argument)
		return 1;
	if (*(int *)first_argument == *(int *)second_argument)
		return 0;
	else
		return -1;
}

int float_comparator(void *first_argument, void *second_argument)
{
	if (*(float *)first_argument > *(float *)second_argument)
		return 1;
	if (*(float *)first_argument == *(float *)second_argument)
		return 0;
	else
		return -1;
}

int double_comparator(void *first_argument, void *second_argument)
{
	if (*(double *)first_argument > *(double *)second_argument)
		return 1;
	if (*(double *)first_argument == *(double *)second_argument)
		return 0;
	else
		return -1;
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
	node_t *n1;
	int x = 1, y = 85, z = 123;
	float x_f = 1.121, y_f = 85.123, z_f = 999.21312;
	double x_df = 17657.1345321, y_df = 851231.123, z_df = 9994565464.2546541312;
	int internal_tid = *(int*) arg;
	printf("Waiting at the barrier... Thread no: %d\n", internal_tid+1);
	pthread_barrier_wait(&barrier);    //wait to create all threads before executing 

	pthread_mutex_lock(&lock);
	printf("We passed the barrier! Thread no: %d\n", internal_tid+1);
	pthread_mutex_unlock(&lock);

	pthread_mutex_lock(&lock);
	while (internal_tid > current) {
		pthread_cond_wait(&cond, &lock);	
	}

	switch(internal_tid) {
		case 0:
			printf("\nThread: %d ===================== \n",internal_tid+1);
			ll_create(&l1, print_int);
			ll_add_end(&l1, &x);
			ll_add_end(&l1, &y);
			ll_print_list(&l1);
			ll_delete(&l1, &y, int_comparator);
			ll_print_list(&l1);
			ll_add_end(&l1, &z);
			
			n1 = ll_search_node(&l1, &z, int_comparator);
			printf("Node searched data: ");
			print_int(n1->val);
			
			ll_print_list(&l1);
			ll_add_end(&l1, &x);
			ll_print_list(&l1);
			ll_add_end(&l1, &y);
			
			n1 = ll_search_node(&l1, &y, int_comparator);
			printf("Node searched data: ");
			print_int(n1->val);
			
			ll_print_list(&l1);
			ll_delete(&l1, &x, int_comparator);
			ll_print_list(&l1);
			ll_delete(&l1, &x, int_comparator);
			ll_delete(&l1, &y, int_comparator);
			ll_print_list(&l1);
			ll_delete(&l1, &x, int_comparator);
			ll_delete(&l1, &z, int_comparator);
			print_int(n1->val);
			ll_add_end(&l1, &y);
			ll_add_end(&l1, &y);
			ll_sort_list(&l1, int_comparator);
			
			ll_print_list(&l1);
			ll_flush_list(&l1);
			ll_print_list(&l1);
			break;

		case 1:
			printf("\nThread: %d ===================== \n",internal_tid+1);
			ll_create(&l2, print_float);
			ll_add_end(&l2, &z_f);
			ll_add_end(&l2, &x_f);
			ll_add_end(&l2, &y_f);
			ll_print_list(&l2);
			ll_sort_list(&l2, float_comparator);
			ll_print_list(&l2);
			
			n1 = ll_search_node(&l2, &y_f, float_comparator);
			printf("Node searched data: ");
			print_float(n1->val);
			n1 = ll_search_node(&l2, &z_f, float_comparator);
			printf("Node searched data: ");
			print_float(n1->val);
			
			ll_delete(&l2, &z_f, float_comparator);
			
			ll_print_list(&l2);
			ll_flush_list(&l2);
			ll_print_list(&l2);
			break;
		
		case 2:
			printf("\nThread: %d ===================== \n",internal_tid+1);
			ll_create(&l3, print_double);
			ll_add_end(&l3, &x_df);
			ll_add_end(&l3, &y_df);
			ll_add_end(&l3, &z_df);
			
			n1 = ll_search_node(&l3, &y_df, double_comparator);
			printf("Node searched data: ");
			print_double(n1->val);
			n1 = ll_search_node(&l3, &z_df, double_comparator);
			printf("Node searched data: ");
			print_double(n1->val);
			
			ll_print_list(&l3);
			ll_sort_list(&l3, double_comparator);
			ll_print_list(&l3);
			ll_delete(&l3, &z_df, double_comparator);
			
			ll_print_list(&l3);
			ll_flush_list(&l3);
			ll_print_list(&l3);
			break;
	}
	
	current++;
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&lock);
	free(arg);
	printf("####Thread exited?!####\n");
	return (void*)0;
}
	
	
int main()
{
	pthread_t th[NUM_THREADS-1];
	int i;
	pthread_barrier_init(&barrier, NULL, NUM_THREADS);
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);
	
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



	
	


