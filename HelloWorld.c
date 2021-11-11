#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3

pthread_barrier_t barrier;	//threads sync using barrier

typedef struct node{
	int val;
	void (*callback)(int);	//argument type can be used as void* for generic type
	struct node *next;
} node_t;			//node_t as node data structure type	
	
node_t *head = NULL;		//global to have 1 argument for: add,delete,sort_list,print_list 
node_t *last_node = NULL;	//global to have 1 argument for: add,delete,sort_list,print_list 




void print_int(int element)
{
	printf("%d ", element);
}


void add(int value)
{

	node_t *new_node = malloc(sizeof(node_t));		//use calloc? or Hirschmann malloc?
	if (NULL == new_node){
		printf("Memory allocation for new_node failed! (add) \n");
	}
	
	new_node->next = NULL;
	new_node->val = value;
	new_node->callback = print_int;		//set function pointer
	if (NULL != last_node){				//link new_node with last node if linked list is not empty
		last_node->next = new_node;
	}
	
	if (NULL == head){			
		head = new_node;			//set first node as head if linked list is empty
	}
	
	last_node = new_node;
	printf("Node with value: %d added!\n",value);
	
}




void delete(int value)
{

	node_t *temp = head;
	int count_element = 0, flag_delete = 0;
	node_t *previous_node = malloc(sizeof(node_t));
	
	if (NULL == previous_node){
		printf("Memory allocation for previous_node failed! (delete)\n");
	}
	
	while (NULL != temp)
	{
		count_element++;
		if (temp->val == value)
		{
			printf("Deleting Node no. %d with value: %d\n",count_element,value);
			previous_node->next = temp->next;
			free(temp);
			
			flag_delete = 1;
		}
		
		if (1 == flag_delete)	break;		//deleting only 1 node (first found) if multiple nodes have the same value
		previous_node = temp;
		temp = temp->next;
	}
	
	if (0 == flag_delete){
		printf("Searched node doesn't exist!\n");
	}

}




void swap(node_t *a, node_t *b)			//used for sort_list
{
	int temp = a->val;
	a->val = b->val;
	b->val = temp;

}

void sort_list(void)					//bubble sort
{
	int swapped, i;
	node_t *ptr2 = NULL;
	node_t *ptr1 = malloc(sizeof(node_t));
	if (NULL == ptr1){
		printf("Memory allocation for previous_node failed! (sort_list)\n");
	}
	
	if (NULL == head)	return;		//exit if linked list is empty
	
	do
	{
		swapped = 0;
		ptr1 = head;
		
		while (ptr1->next != ptr2)
		{
			if (ptr1->val > ptr1->next->val)
			{
				swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		ptr2 = ptr1;
	}
	while (swapped);
	
	printf("Linked list has been sorted! \n");
}



void flush_list(void)
{
	node_t *temp, *cursor = malloc(sizeof(node_t));
	if(temp || cursor == NULL){
		printf("Memory allocation failed! (flush_list)\n");
	}
	 
	if (NULL != head)
	{
		cursor = head->next;
		head->next = NULL;
		while (NULL != cursor)
		{
			temp = cursor->next;
			free(cursor);
			cursor = temp;
		
		} 
	}
	
	head = NULL;					//flush first element too
	printf("Linked list has been deleted!\n");
	
}




void print_list(void)
{	
	
	node_t *temp = head;

	printf("Printing linked list ... \n");
	
	if (NULL == head){				//check if list is empty to prevent seg fault
		printf("List is empty !!! Add new elements.\n");
		return;
	}
	
	
	while (NULL != temp)
	{
		temp->callback(temp->val);		//use function pointer	
		temp = temp->next;
		if (temp!=NULL)	printf(" ---> ");
	}
	
	printf("\n");
	
}


	
void *sync_routine(void *arg)			//wait to create all threads before executing 
{	
	int internal_tid = *(int*) arg;
	printf("Waiting at the barrier... Thread no: %d\n", internal_tid+1);
	pthread_barrier_wait(&barrier);
	printf("\n");
	printf("We passed the barrier ! Thread no: %d\n", internal_tid+1);
	
	switch(internal_tid)
	{
		case 0:
			add(2);
			add(4);
			add(10);
			delete(2);
			sort_list();
			delete(10);
			delete(5);	
			break;
	
		case 1:
			add(11);
			add(1);
			delete(11);
			add(8);
			print_list();
			break;
			
		case 2:
			add(30);
			add(25);
			add(100);
			sort_list();
			print_list();
			delete(100);
			break;
			
	}
	
	free(arg);
}




int main()
{
	pthread_t th[NUM_THREADS-1];
	int i = 0;
	pthread_barrier_init(&barrier, NULL, NUM_THREADS);
	
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
	
	return 0;
}



	
	


