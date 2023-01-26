#ifndef _utility_h_
#define _utility_h_

struct node_s {
	struct node_s *next;
	struct node_s *prev;
	void* data;
};

typedef struct node_s node;

struct linked_list_s {
	node * head;
	node * tail;
	int size;
};

typedef struct linked_list_s linked_list;


node* create_node(void* data);
linked_list * create_linked_list();
void add_node(linked_list* ll, void* data);
void remove_node(linked_list* ll, void* data);
void add_after(linked_list* ll, node *after_node, void* data);
void sort(linked_list *ll, int (*cmp)(void *data1, void *data2));
void swap_nodes(node *a, node *b);


// Queue Implementation //

typedef struct linked_list_s queue;

queue * create_queue();
void enqueue(queue * q, void * data);
void * dequeue(queue * q);



#endif
