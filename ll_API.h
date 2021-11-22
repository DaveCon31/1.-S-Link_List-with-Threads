#ifndef ll_API
#define ll_API

#ifdef __cplusplus
extern "C" {
#endif
#include "ll_struct.h"

void print_int(int element)
{
	printf("%d ", element);
}



int validate(void *name){					//VALIDATE HEAD

	if(NULL == head){
		printf("Linked list is empty! (%s)\n", (const char*)name);
		return -1;
	}
	return 0;
}		



void add(int value)
{	

	printf("add function has been called!!!	");
	node_t *new_node = malloc(sizeof(node_t));	
	if (NULL == new_node){
		printf("Memory allocation for new_node failed! (add) \n");
	}
	
	new_node->next = NULL;
	new_node->val = value;
	new_node->callback = print_int;		         //set function pointer
	
	if (NULL == head){			
		head = new_node;				//set first node as head if linked list is empty
	}
	
	if (NULL != last_node){				//link new_node with last node if linked list is not empty
		last_node->next = new_node;
	
	}
	
	last_node = new_node;
	printf("Node with value: %d added!\n",value);

}




void delete(int value)
{	

	printf("delete function has been called!!!	");
	if(callback_validate("delete") == -1)		return;		//VALIDATE WITH CALLBACK
	
	node_t *temp = head;
	int count_element = 0, flag_delete = 0;
	node_t *previous_node = NULL;
	
	if(head->val == value){
		node_t *front = head;
		head = head->next;
		front->next = NULL;
		free(front);
		last_node = head;
		printf("Node head with value: %d deleted!\n",value);
		return;
	}
	
	while (NULL != temp)
	{
		count_element++;
		if (temp->val == value)
		{
			printf("Node no.%d with value: %d deleted!\n",count_element,value);
			previous_node->next = temp->next;
			free(temp);
			
			flag_delete = 1;
		}
		
		if (flag_delete == 1)	break;		//deleting only 1 node (first found) if multiple nodes have the same value
		previous_node = temp;
		temp = temp->next;
	}
	
	if (flag_delete == 0){
		printf("Node with value: %d doesn't exist!\n",value);
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
	printf("sort_list has been called!!!	");
	int swapped;
	node_t *ptr2 = NULL;
	node_t *ptr1;
	
	if(callback_validate("sort_list") == -1)		return;	//VALIDATE WITH CALLBACK
	
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
	if(callback_validate("flush_list") == -1)		return;	//VALIDATE WITH CALLBACK
	
	node_t *temp = NULL;			
	while(NULL != head){
		temp = head;
		head = head->next;
		free(temp);	
	}
	
	printf("Linked list has been deleted!\n");
}




void print_list(void)
{	
	printf("print_list function has been called!!!	");
	printf("Printing linked list: ");
	if(callback_validate("print_list") == -1)	return;		//VALIDATE WITH CALLBACK
	
	node_t *temp = head;
	while (NULL != temp)
	{
		temp->callback(temp->val);		//use function pointer	
		temp = temp->next;
		if (NULL != temp)	printf(" ---> ");
	}
	
	printf("\n");
}

#ifdef __cplusplus
}
#endif

#endif		//ll_API

