#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat * create_process_stat(process* proc);

average_stats round_robin_p(linked_list * processes, int time_slice) {
	int t = 0;

	//Create Process Queue
	queue *process_queue = (queue *)create_queue();
	node * proc_ptr = processes->head;
	if(processes->head == NULL) {
		fprintf(stderr,"No Process to schedule\n");
	}
	//while process queue is not empty or time quanta is less than 100
	process_stat * scheduled_process = NULL;

	linked_list *ll = create_linked_list();
	printf("\nRR :\n");
	node * cur_node = NULL;
	int cur_run = 0;
	while(t<100 || process_queue->size > 0) {
		//check for incoming new process and enqueue it in the queue only when its arrival time is before 100
		if(proc_ptr != NULL && t<100) {
			process * new_process = (process *)(proc_ptr->data);
			while(proc_ptr!=NULL && new_process->at <= t) {
				enqueue(process_queue,create_process_stat(new_process));
				proc_ptr = proc_ptr->next;
				if(proc_ptr!=NULL)
					new_process = (process *)(proc_ptr->data);
			}
		}

		//if there is no scheduled process, then check process queue and schedule it
		if(cur_node == NULL) {
			cur_run = 0;
			cur_node = process_queue->head;
		} else if(cur_run == time_slice) {
			cur_run = 0;
			cur_node = cur_node->next;
			if(cur_node == NULL) {
				cur_node = process_queue->head;
			}
		}

		if(cur_node != NULL) {
			scheduled_process = (process_stat *) cur_node->data;
			process * proc = scheduled_process->proc;

			if(t>=100) {
				if(scheduled_process->start_time == -1) {
					//Don't start any new process, remove it from process_queue
					free(scheduled_process);
					//dequeue(scheduled_process);
					node * next = cur_node->next;
					remove_node(process_queue,cur_node->data);
					cur_node = next;
					cur_run = 0;
					continue;
				}
			}
			//add current running process to the time chart
			printf("%c",proc->process_id);
			cur_run++;
			//update current processes stat
			if(scheduled_process->start_time == -1) {
				scheduled_process->start_time = t;
			}
			scheduled_process->rt++;

			if(scheduled_process->rt >= proc->rt) {
				scheduled_process->end_time = t;
				add_node(ll,scheduled_process);
				node * next = cur_node -> next;
				remove_node(process_queue, cur_node->data);
				cur_node = next;
				cur_run = 0;
			}
		} else {
			printf("_");
		}
		//increase the time
		t++;
	}

	return print_policy_stat(ll);
}
