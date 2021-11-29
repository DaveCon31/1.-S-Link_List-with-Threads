#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ll_API.h"

#ifdef NDEBUG
#define DEBUG_PRINTF(...)
#else
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#endif

typedef struct my_ll {
	node_t *head;
	node_t *tail;
	int (*comparator)(void *first, void* second);
	void (*print_val)(void *val);
} ll_t;    //ll_t as linked list data type

int (*callback_validate)(void *data) = NULL;    //Validate data type; set by user

int validate_head(ll_t *list)    //VALIDATE HEAD list
{
	if (list == NULL)
		return -1;
	if (list->head == NULL) {
		return -1;
	}
	return 0;
}		

ll_t* ll_create(void (*ptr_print_val)(void *val), int (*ptr_comparator)(void *first, void* second)) 
{
	ll_t *list = malloc(sizeof(ll_t));
	if (list == NULL) {
		DEBUG_PRINTF("Memory allocation for list failed!\n");
	}
	
	list->head = NULL;
	list->tail = NULL;
	list->print_val = ptr_print_val;
	list->comparator = ptr_comparator;
	return list;
}

void ll_set_data_validation_callback(int (*ptr_callback_validate)(void *data))
{
	callback_validate = ptr_callback_validate;
}

void ll_add_end(ll_t *list, void *value)
{	
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
	if (list->tail != NULL) {   
		list->tail->next = new_node;    //link new_node with last node if linked list is not empty
	}
	
	list->tail = new_node;
}

void ll_delete(ll_t *list, void *value)
{
	if (validate_head(list) == -1)
		return;
	
	node_t *temp = list->head;
	node_t *prev = NULL;
	while (temp != NULL) {
		if (list->comparator(temp->val, value) == 0) {
			if (temp == list->head)
				list->head = list->head->next;
			if (temp == list->tail)
				list->tail = prev;
			if (prev != NULL)
				prev->next = temp->next;
			break;
		}
		prev = temp;
		temp = temp->next;
	}
	free(temp);
	temp = NULL;
}

node_t *ll_search_node(ll_t *list, void *value)
{
	if (validate_head(list) == -1)    //validate head list		
		return NULL;

	
	node_t *temp = list->head;
	while (temp != NULL) {
		if (list->comparator(temp->val, value) == 0) 
			return temp;		
		temp = temp->next;
	}
	return NULL;
}

void swap(node_t *a, node_t *b)    //used for sort_list
{
	void *temp = a->val;
	a->val = b->val;
	b->val = temp;
}

void ll_sort_list(ll_t *list)    //bubble sort
{
	int swapped;
	node_t *ptr2 = NULL;
	node_t *ptr1 = NULL;    //was uninitialized
	if (validate_head(list) == -1)    //validate head list
			return;
	
	do {
		swapped = 0;
		ptr1 = list->head;
		
		while (ptr1->next != ptr2) {
			if (list->comparator(ptr1->val, ptr1->next->val) == 1) {
				swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		ptr2 = ptr1;
	}
	while (swapped);
}

void ll_flush_list(ll_t *list)
{
	if (validate_head(list) == -1)    //validate head list
		return;
	
	node_t *temp = NULL;	
	while (list->head != NULL) {
		temp = list->head;
		list->head = list->head->next;
		free(temp);
	}
	list->tail = NULL;
}

void ll_print_list(ll_t *list)
{	
	if (validate_head(list) == -1)    //validate head list
		return;
	
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

void ll_destroy_list(ll_t **list)
{
	ll_flush_list(*list);
	free(*list);    //free list resources
	*list = NULL;
}
