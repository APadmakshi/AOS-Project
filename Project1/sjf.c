#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat * create_process_stat(process* proc);

int compareRunTime(void * data1, void * data2) {
	process_stat * ps1 = (process_stat *) data1;
	process_stat * ps2 = (process_stat *) data2;
	if(((process *)ps1->proc)->rt < ((process *)ps2->proc)->rt) {
		return -1;
	} else {
		return 1;
	}
}

average_stats shortest_job_first_np(linked_list * processes) {
	int t = 0;

	//Create a process Queue
	queue *process_queue = (queue *)create_queue();
	node * proc_ptr = processes->head;
	if(processes->head == NULL) {
		fprintf(stderr,"No process to schedule\n");
	}
	//while process queue is not empty or time quanta is less than 100
	process_stat * scheduled_process = NULL;

	linked_list *ll = create_linked_list();
	printf("\nSJF :\n");
	while(t<100 || scheduled_process!=NULL) {
		//check for incoming new process and enqueue it in the queue
		if(proc_ptr != NULL) {
			process * new_process = (process *)(proc_ptr->data);
			while(proc_ptr!=NULL && new_process->at <= t) {
				enqueue(process_queue,create_process_stat(new_process));
				sort(process_queue,compareRunTime);
				proc_ptr = proc_ptr->next;
				if(proc_ptr!=NULL)
					new_process = (process *)(proc_ptr->data);
			}
		}

		//if there is no scheduled process, then check process queue and schedule it
		if(scheduled_process == NULL && process_queue->size > 0) {
			scheduled_process = (process_stat *) dequeue(process_queue);
		}

		if(scheduled_process != NULL) {
			process * proc = scheduled_process->proc;

			//add current running process to the time chart
			printf("%c",proc->process_id);

			//update current processes stat
			if(scheduled_process->start_time == -1) {
				scheduled_process->start_time = t;
			}
			scheduled_process->rt++;

			if(scheduled_process->rt >= proc->rt) {
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

	//Print Process Stats
	return print_policy_stat(ll);
}
