#ifndef ll_struct
#define ll_struct

#ifdef __cplusplus
extern "C" {
#endif

typedef struct node{
	int val;
	void (*callback)(int);	//argument type can be used as void* for generic type
	struct node *next;
} node_t;			//node_t as node data structure type	
	
node_t *head = NULL;		//global to have 1 argument for: add,delete,sort_list,print_list 
node_t *last_node = NULL;	//global to have 1 argument for: add,delete,sort_list,print_list

int (*callback_validate)(void *name);	//VALIDATE HEAD 

#ifdef __cplusplus
}
#endif

#endif		//ll_struct
