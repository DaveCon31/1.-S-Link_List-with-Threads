#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


typedef struct node{
	int val;
	void (*callback)(int);	//argument type can be used as void* for generic type
	struct node *next;
} node_t;			//node_t as node data structure type	
	
node_t *head = NULL;		//global to have 1 argument for: add and del @alternative: use as static
node_t *last_node = NULL;	//global to have 1 argument for: add and del @alternative: use as static



void print_int(int element)
{
	printf("%d ", element);
}



void add(int value)
{

	node_t *new_node = malloc(sizeof(node_t));		//use calloc? or Hirschmann malloc?
	if(NULL == new_node){
		printf("Memory allocation for new_node failed!\n");
	}
	
	new_node->next = NULL;
	new_node->val = value;
	new_node->callback = print_int;		//set function pointer
	if(NULL != last_node){				//link new_node with last node 
		last_node->next = new_node;
	}
	
	if(NULL == head){			
		head = new_node;			//set first node as head
	}
	
	last_node = new_node;
	
}



void print_list(void)
{	

	node_t *temp = head;
	while(NULL != temp)
	{
		temp->callback(temp->val);		//use function pointer	
		temp = temp->next;
		if(temp!=NULL)	printf(" ---> ");
	}
	
	printf("\n");
	
}


	
int main()
{

	add(7);
	add(10);
	add(11);
	add(1);
	print_list();
	
}



	
	


