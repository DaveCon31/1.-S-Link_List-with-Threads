#ifndef ll_API
#define ll_API

typedef struct node {
	void *val;
	struct node *next;
} node_t;    //node_t as node data type

typedef struct my_ll ll_t;    //opaque data type for link list
 
//set validation data for input
//callback should return 0 for success and -1 for failure
void ll_set_data_validation_callback(int (*ptr_callback_validate)(void *data));

//initialize linked list with last_node & head = NULL
//MUST BE CALLED FOR EVERY NEW LIST CREATED OTHERWISE SEGFAULT
ll_t *ll_create(void (*print_val)(void *val), int (*comparator)(void *first, void* second));

//adds a node at the end of the list with a value
void ll_add_end(ll_t *list, void *value);

//removes the node with a value
void ll_delete(ll_t *list, void *value);

//returns the address of node with specified data
node_t *ll_search_node(ll_t *list, void *value);

//sorting the list using bubble sort algorithm
void ll_sort_list(ll_t *list);

//deletes entire list, deallocated everything
void ll_flush_list(ll_t *list);

//prints every node's data (value)
void ll_print_list(ll_t *list);

//destroy list
void ll_destroy_list(ll_t **list);

#endif		//ll_API
