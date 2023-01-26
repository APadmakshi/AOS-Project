#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat * create_process_stat(process* proc) {
	process_stat * ps = (process_stat *) malloc(sizeof(process_stat));
	ps->proc = proc;
	ps->wait_time = 0;
	ps->turnaround_time = 0;
	ps->response_time = 0;

	ps->run_time = 0;
	ps->start_time = -1;
	ps->end_time = -1;
	return ps;
}

average_stats first_come_first_serve_np(linked_list * processes) {
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
	printf("\nFirst Come First Serve:\n");
	while(t<100 || scheduled_process!=NULL) {
		//check for incoming new process and enqueue it in the queue
		if(proc_ptr != NULL) {
			process * new_process = (process *)(proc_ptr->data);
			if(new_process->arrival_time <= t) {
				enqueue(process_queue,create_process_stat(new_process));
				proc_ptr = proc_ptr->next;
			}
		}

		//if there is no scheduled process, then check process queue and schedule it
		if(scheduled_process == NULL && process_queue->size > 0) {
			scheduled_process = (process_stat *) dequeue(process_queue);
		}

		if(scheduled_process != NULL) {
			process * proc = scheduled_process->proc;

			//add current running process to the time chart
			printf("%c",proc->pid);

			//update current processes stat
			if(scheduled_process->start_time == -1) {
				scheduled_process->start_time = t;
			}
			scheduled_process->run_time++;

			if(scheduled_process->run_time >= proc->run_time) {
				scheduled_process->end_time = t;
				add_node(ll,scheduled_process);
				scheduled_process = NULL;
			}
		} else {
			printf("_");
		}
		//increase the time
		t++;
	}
	printf("\n");

	//Print Process Stat
	return print_policy_stat(ll);
}
