#include <stdio.h>
#include <stdlib.h>
#include "ll_API.h"

int (*callback_validate)(void *data) = NULL;    //Validate data type; set by user
void print_int(int element) 
{
	printf("%d ", element);
}

int validate(ll_t *list, void *name)    //VALIDATE HEAD list
{   
	if (list->head == NULL) {
		printf("Linked list is empty! (%s)\n", (const char*)name);
		return -1;
	}
	return 0;
}		

void ll_create(ll_t* list, void (*ptr_print_val)(void *val)) 
{
	printf("Creating linked list ...");
	list->head = NULL;
	list->last_node = NULL;
	list->print_val = ptr_print_val;
	printf(" Created.\n"); 
}

void ll_set_data_validation_callback(int (*ptr_callback_validate)(void *data))
{
	callback_validate = ptr_callback_validate;
}

void ll_add_end(ll_t *list, void *value)
{	
	printf("add function has been called!!!	");
	
	if (callback_validate != NULL) {
		if (callback_validate(value) != 0)
			return;
	}
	
	node_t *new_node = malloc(sizeof(node_t));	
	if (new_node == NULL) {
		printf("Memory allocation for new_node failed! (add) \n");
	}
	new_node->next = NULL;
	new_node->val = value;
	
	if (list->head == NULL) {			
		list->head = new_node;    //set first node as head if linked list is empty
	}
	if (list->last_node != NULL) {    
		list->last_node->next = new_node;    //link new_node with last node if linked list is not empty
	}
	
	list->last_node = new_node;
	printf("Node with value: %d added!\n",value);
}

void ll_delete(ll_t *list, void *value)
{	
	printf("delete function has been called!!!	");
	if (callback_validate(list, "delete") == -1) {    //VALIDATE WITH CALLBACK		
		return;    
	}
	node_t *temp = list->head;
	int count_element = 0, flag_delete = 0;
	node_t *previous_node = NULL;
	
	if (list->head->val == value) {
		node_t *front = list->head;
		list->head = list->head->next;
		front->next = NULL;
		free(front);
		list->last_node = list->head;
		printf("Node head with value: %d deleted!\n",value);
		return;
	}
	
	while (temp != NULL) {
		count_element++;
		if (temp->val == value) {
			printf("Node no.%d with value: %d deleted!\n",count_element,value);
			previous_node->next = temp->next;
			free(temp);
			flag_delete = 1;
		}
		if (flag_delete == 1) {
			break;    //deleting only 1 node (first found) if multiple nodes have the same value
		}
		previous_node = temp;
		temp = temp->next;
	}
	
	if (flag_delete == 0) {
		printf("Node with value: %d doesn't exist!\n",value);
	}
}

void swap(node_t *a, node_t *b)    //used for sort_list
{
	int temp = a->val;
	a->val = b->val;
	b->val = temp;
}

void ll_sort_list(ll_t *list)    //bubble sort
{
	printf("sort_list has been called!!!    ");
	int swapped;
	node_t *ptr2 = NULL;
	node_t *ptr1;    //NULL?!
	if (callback_validate(list, "sort_list") == -1) {    //VALIDATE WITH CALLBACK
			return;
	}
	
	do {
		swapped = 0;
		ptr1 = list->head;
		
		while (ptr1->next != ptr2) {
			if (ptr1->val > ptr1->next->val) {
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

void ll_flush_list(ll_t *list)
{
	if (callback_validate(list, "flush_list") == -1) {    //VALIDATE WITH CALLBACK
		return;
	}
	
	node_t *temp = NULL;	
	while (list->head != NULL) {
		temp = list->head;
		list->head = list->head->next;
		free(temp);	
	}
	printf("Linked list has been deleted!\n");
}

void ll_print_list(ll_t *list)
{	
	printf("print_list function has been called!!!	");
	printf("Printing linked list: ");
	if (callback_validate(list, "print_list") == -1) {    //VALIDATE WITH CALLBACK
		return;
	}
	
	node_t *temp = list->head;
	while (temp != NULL) {
		temp->callback(temp->val);    //use function pointer	
		temp = temp->next;
		if (temp != NULL) {
			printf(" ---> ");
		}
	}
	printf("\n");
}
