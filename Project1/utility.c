#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "process.h"
#include "stat.h"


// linked_list Implementation Function Definitions //

//create new linked_list//

linked_list * create_linked_list()
{
	linked_list * new_ll =  (linked_list*) malloc(sizeof(linked_list));
	new_ll->head = NULL;
	new_ll->tail = NULL;
	new_ll->size = 0;
	return new_ll;
}



//create a new node//
node* create_node(void* data)
{
	node* new_node = (node*) malloc(sizeof(node));
	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;
	return new_node;
}

// Add a node to existing linked list//
void add_node(linked_list * ll, void * data)
{
	node * new_node = create_node(data);
	if(ll->size == 0)
	{
		ll->head = new_node;
		ll->tail = new_node;
		ll->size = 1 ;
	} else {
		new_node->prev = ll->tail;
		ll->tail->next = new_node;
		ll->tail = new_node;
		ll->size += 1;
	}
}

// Remove a node from existing linked_list //
void remove_node(linked_list* ll, void * data)
{
	node* current_node = ll->head;

	while(current_node != NULL && current_node->data != data) {
		current_node = current_node->next;
	}

	if(current_node != NULL) {
		if(current_node->prev != NULL) {
			current_node->prev->next = current_node->next;
		}
		if(current_node->next != NULL) {
			current_node->next->prev = current_node->prev;
		}
		if(ll->head == current_node) {
			ll->head = current_node->next;
		}
		if(ll->tail == current_node) {
			ll->tail = current_node->prev;
		}
		ll->size --;
		free(current_node);
	}
}

// Remove a node from existing linked_list //
void remove_head(linked_list* ll)
{
	node * current_node = ll->head;
	if(current_node != NULL) {
		ll->head = current_node->next;
		if(ll->tail == current_node) {
			ll->tail = current_node->prev;
		}
		ll->size --;
		free(current_node);
	}
}

// Add a new node after a particular node in an existing linked_list//
void add_after(linked_list* ll, node *after_node, void *data)
{
	node* new_node = create_node(data);

	node* next_node = after_node->next;
	new_node->next = next_node;
	if(next_node != NULL) next_node->prev = new_node;

	new_node->prev = after_node;
	after_node->next = new_node;

	if(ll->tail == after_node) {
		ll->tail = new_node;
	}

	ll->size++;
}

void sort(linked_list *ll, int (*cmp)(void *data1, void *data2)) {
	node *i = ll->head;
	while(i!=NULL) {
		node *j = i->next;
		while(j!=NULL) {
			void * p1 = i->data;
			void * p2 = j->data;
			if((*cmp)(p1,p2) > 0) {
				swap_nodes(i,j);
			}
			j=j->next;
		}
		i = i->next;
	}
}

void swap_nodes(node *a, node *b) {
	void * temp = a->data;
	a->data = b->data;
	b->data = temp;
}

// Queue Implementation //

queue * create_queue() {
	return create_linked_list();
}

// Enqueue function to add process at the end of the queue //
void enqueue(queue* q, void * data)
{
	node* new_node = create_node(data);

	new_node->prev = q->tail;
	if(q->tail != NULL) {
		q->tail->next = new_node;
		q->tail = new_node;
	} else {
		q->tail = new_node;
		q->head = new_node;
	}
	q->size += 1;
}

// Dequeue function to remove process from the end of the queue //
void* dequeue(queue* q)
{
	if(q->head != NULL) {
		node * current_node = q->head;//Address of q->head
		void * data = current_node->data;

		//Moving Head to next Node
		node * next_node = q->head->next;

		if(next_node != NULL) next_node->prev = NULL;
		q->head = next_node; //current_node = q->head = next_node


		//Maintaining boundary tail condition
		if(q->tail == current_node) {
			q->tail = NULL;
		}

		q->size--;
		free(current_node);
		return data;
	}
}

average_stats print_policy_stat(linked_list * ll)
{
	average_stats avg;
	//Print Process Stat of a scheduling algorithm
	printf("\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	printf("Process Name\t| Arrival Time | Start Time | End Time | Run Time | Response Time | Wait Time | Turn Around Time | Priority |\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	node * ptr = ll->head;
	float avg_response_time = 0;
	float avg_wait_time = 0;
	float avg_turnaround = 0;
	int process_count = 0;
	while(ptr!=NULL) {
		process_stat *stat = (process_stat *)ptr->data;
		if(stat == NULL) printf("No Stat\n");
		process *proc = (process *)stat->proc;
		if(proc == NULL) printf("No Process\n");
		float arrival_time = proc->arrival_time;
		float run_time = proc->run_time;
		float response_time = stat->start_time - arrival_time;
		float turnaround = stat->end_time - proc->arrival_time + 1;
		float wait_time = turnaround - run_time;
		unsigned char priority = proc->priority;
		avg_response_time += response_time;
		avg_wait_time += wait_time;
		avg_turnaround += turnaround;
		process_count++;
		printf("%16c|%14.1f|%12.1f|%10.1f|%10.1f|%15.1f|%11.1f| %17.1f|%10u|\n",proc->pid,arrival_time, stat->start_time, stat->end_time, run_time, response_time, wait_time, turnaround,priority);
		ptr = ptr->next;
	}
	avg.avg_throughput = process_count;
	if(process_count == 0) process_count = 1;
	avg_response_time = avg_response_time / process_count;
	avg_wait_time = avg_wait_time / process_count;
	avg_turnaround = avg_turnaround / process_count;
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	printf("%16s|%14.1f|%12.1f|%10.1f|%10.1f|%15.1f|%11.1f| %17.1f|\n"," Average",0.0, 0.0, 0.0,0.0,avg_response_time, avg_wait_time, avg_turnaround);
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	//return stat
	avg.avg_response_time = avg_response_time;
	avg.avg_wait_time = avg_wait_time;
	avg.avg_turnaround = avg_turnaround;

	return avg;
}
