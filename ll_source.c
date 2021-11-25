#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ll_API.h"

#define NDEBUG    //disable/enable printfs (comment to enable printfs)

#ifdef NDEBUG
#define DEBUG_PRINTF(...)
#else
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#endif

int (*callback_validate)(void *data) = NULL;    //Validate data type; set by user

int validate_head(ll_t *list, void *name)    //VALIDATE HEAD list
{   
	if (list->head == NULL) {
		DEBUG_PRINTF("Linked list is empty! (%s)\n", (const char*)name);
		return -1;
	}
	return 0;
}		

void ll_create(ll_t* list, void (*ptr_print_val)(void *val)) 
{
	DEBUG_PRINTF("Creating linked list ...");
	list->head = NULL;
	list->last_node = NULL;
	list->print_val = ptr_print_val;
	DEBUG_PRINTF(" Created.\n"); 
}

void ll_set_data_validation_callback(int (*ptr_callback_validate)(void *data))
{
	callback_validate = ptr_callback_validate;
}

void ll_add_end(ll_t *list, void *value)
{	
	DEBUG_PRINTF("ll_add_end function has been called!!!	");
	
	if (callback_validate != NULL) {
		if (callback_validate(value) != 0)
			return;
	}
	
	node_t *new_node = malloc(sizeof(node_t));	
	if (new_node == NULL) {
		DEBUG_PRINTF("Memory allocation for new_node failed! (add) \n");
		return;
	}
	new_node->next = NULL;
	new_node->val = value;
	
	if (list->head == NULL) {			
		list->head = new_node;    //set first node as head if linked list is empty
	}
	if (list->last_node != NULL) {    
		list->last_node->next = new_node;    //link new_node with last node if linked list is not empty
	}
	
	DEBUG_PRINTF("Node with value: ");
	list->print_val(new_node->val);
	DEBUG_PRINTF(" added!\n");
	list->last_node = new_node;
}

void ll_delete(ll_t *list, void *value, int (*comparator)(void *first, void* second))
{
	DEBUG_PRINTF("ll_delete function has been called!!!	");
	if (validate_head(list, "ll_delete") == -1) {    //validate head list		
		return;    
	}
	node_t *temp = list->head;
	int count_element = 0, flag_delete = 0;
	node_t *previous_node = NULL;

	if (comparator(list->head->val, value) == 0) {
		list->head = temp->next;
		DEBUG_PRINTF("Node head with value: ");
		list->print_val(temp->val); 
		DEBUG_PRINTF("deleted!\n");
		free(temp);
		list->last_node = NULL;    //correctly reset last_node in this corner case
		return;
	}

	while (temp != NULL) {
		count_element++;
		if (comparator(temp->val, value) == 0) {
			DEBUG_PRINTF("Node no.%d ",count_element);
			DEBUG_PRINTF("with value: ");
			list->print_val(temp->val); 
			DEBUG_PRINTF("deleted!\n");
			if (temp->next != NULL)
				previous_node->next = temp->next;
			else {
				previous_node->next = NULL;
				list->last_node = previous_node;    //correctly reset last_node in this corner case
			}
			
			free(temp);
			flag_delete = 1;
		}
		if (flag_delete == 1) {
			return;    //deleting only 1 node (first found) if multiple nodes have the same value
		}
		previous_node = temp;
		temp = temp->next;
	}
	
	DEBUG_PRINTF("Node head with value: ");
	list->print_val(value); 
	DEBUG_PRINTF("doesn't exist!\n");
}

node_t *ll_search_node(ll_t *list, void *value, int (*comparator)(void *first, void* second))
{
	DEBUG_PRINTF("ll_search_node function has been called!!!	");
	if (validate_head(list, "ll_search_node") == -1) {    //validate head list		
		return NULL;
	}
	
	node_t *temp = list->head;
	while (temp != NULL) {
		if (comparator(temp->val, value) == 0) {
			DEBUG_PRINTF("Node found!\n");
			return temp;		
		}
		temp = temp->next;
	}
	DEBUG_PRINTF("Node not found! Returning NULL.\n");
	return NULL;
}

void swap(node_t *a, node_t *b)    //used for sort_list
{
	void *temp = a->val;
	a->val = b->val;
	b->val = temp;
}

void ll_sort_list(ll_t *list, int (*comparator)(void *first, void* second))    //bubble sort
{
	DEBUG_PRINTF("sort_list has been called!!!    ");
	int swapped;
	node_t *ptr2 = NULL;
	node_t *ptr1 = NULL;    //was uninitialized
	if (validate_head(list, "ll_sort_list") == -1) {    //validate head list
			return;
	}
	
	do {
		swapped = 0;
		ptr1 = list->head;
		
		while (ptr1->next != ptr2) {
			if (comparator(ptr1->val, ptr1->next->val) == 1) {
				swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		ptr2 = ptr1;
	}
	while (swapped);
	DEBUG_PRINTF("Linked list has been sorted! \n");
}

void ll_flush_list(ll_t *list)
{
	if (validate_head(list, "ll_flush_list") == -1) {    //validate head list
		return;
	}
	
	node_t *temp = NULL;	
	while (list->head != NULL) {
		temp = list->head;
		list->head = list->head->next;
		free(temp);	
	}
	DEBUG_PRINTF("Linked list has been deleted!\n");
}

void ll_print_list(ll_t *list)
{	
	DEBUG_PRINTF("print_list function has been called!!!	");
	DEBUG_PRINTF("Printing linked list: ");
	if (validate_head(list, "ll_print_list") == -1) {    //validate head list
		return;
	}
	
	node_t *temp = list->head;
	while (temp != NULL) {
		list->print_val(temp->val);    //use function pointer	
		temp = temp->next;
		if (temp != NULL) {
			DEBUG_PRINTF(" ---> ");
		}
	}
	DEBUG_PRINTF("\n");
}
