#ifndef ll_API
#define ll_API

typedef struct node {
	int val;
	struct node *next;
} node_t;    //node_t as node data type	

typedef struct list {
	node_t *head;
	node_t *last_node;
	void (*print_val)(void *val);
} ll_t;    //ll_t as linked list data type    

//set validation data for input
// callback should return 0 for success and -1 for failure
void ll_set_data_validation_callback(int (*ptr_callback_validate)(void *data));

//initialize linked list with last_node & head = NULL
//MUST BE CALLED FOR EVERY NEW LIST CREATED OTHERWISE SEGFAULT
void ll_create(ll_t *list, void (*print_val)(void *val));

//function for callback_validate to validate head node
int validate(ll_t *list, void *name);    
		
//adds a node at the end of the list with a value
void ll_add_end(ll_t *list, void *value);

//removes the node with a value
void ll_delete(ll_t *list, void *value);

//sorting the list using bubble sort algorithm
void ll_sort_list(ll_t *list);

//deletes entire list, deallocated everything
void ll_flush_list(ll_t *list);

//prints every node's data (value)
void ll_print_list(ll_t *list);

#endif		//ll_API
