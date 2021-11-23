#ifndef ll_API
#define ll_API

typedef struct node {
	int val;
	void (*callback)(int);    //argument type can be used as void* for generic type
	struct node *next;
} node_t;    //node_t as node data type	

typedef struct list {
	node_t *head;
	node_t *last_node;
} ll_t;    //ll_t as linked list data type    

int (*callback_validate)(ll_t *list, void *name);    //VALIDATE HEAD 

//initialize linked list with last_node & head = NULL
//MUST BE CALLED FOR EVERY NEW LIST CREATED OTHERWISE SEGFAULT
void ll_create(ll_t *list);

//function for callback to print int elements
void print_int(int element);

//function for callback_validate to validate head node
int validate(ll_t *list, void *name);    
		
//adds a node at the end of the list with a value
void ll_add_end(ll_t *list, int value);

//removes the node with a value
void ll_delete(ll_t *list, int value);

//sorting the list using bubble sort algorithm
void ll_sort_list(ll_t *list);

//deletes entire list, deallocated everything
void ll_flush_list(ll_t *list);

//prints every node's data (value)
void ll_print_list(ll_t *list);

#endif		//ll_API
