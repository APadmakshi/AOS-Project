#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat * create_process_stat(process* proc);

// function to compare time to completion processes while sorting
int compare_remaining_time(void * data1, void * data2)
{
  process_stat * ps1 = (process_stat *) data1;
	process_stat * ps2 = (process_stat *) data2;
	if(((((process *)ps1->proc)->rt) - (ps1->rt)) < ((((process *)ps2->proc)->rt) - (ps2->rt))) {
		return -1;
	} else {
		return 1;
	}
}

// function to print contents of queue for testing purpose during shortest remaining time to completion
void print_queue_srtc(queue * q)
{
  if (q->head != NULL)
  {
    node * n = q->head;
    printf("Queue Contents:\n");
    while(n != NULL)
    {
      process_stat* ps = n->data;
      process * p = ps->proc;
      printf("Process Id: %c Remaining Time: %f\n",p->process_id,(p->rt - ps->rt));
      n = n->next;
    }
  }
  return;
}

// Implementation of shortest remaining time to completion preemptive //
average_stats shortest_remaining_time_p(linked_list * processes)
{
  int t = 0; // quanta

  // creation of a queue of processes
  queue * process_queue = create_queue();
  // creation of linked list for managaing process order in preemeption
  linked_list * ll = create_linked_list();

  node * process_pointer = processes->head;
  if(processes->head == NULL) {
		fprintf(stderr,"No process to schedule\n");
	}

  //while process queue is not empty or time quanta is less than 100
  process_stat * scheduled_process = NULL;
  printf("\n\nSRT to Complete Preemptive:\n");
  while(t<100 || scheduled_process!=NULL )
  {
    //printf("Time %d\n",t);
    if(scheduled_process!=NULL)
    {
      enqueue(process_queue,scheduled_process);
      scheduled_process = NULL;
    }
    //check for incoming new process and enqueue it in the queue
		if(process_pointer != NULL) {
			process * new_process = (process *)(process_pointer->data);
			while(process_pointer !=NULL && new_process->at <= t) {
				enqueue(process_queue,create_process_stat(new_process));
				//sort(process_queue,compare_remaining_time);
				process_pointer = process_pointer->next;
				if(process_pointer!=NULL)
        {
					new_process = (process *)(process_pointer->data);
        }
			}
      // sort all the processes that have arrived based on their remaining running time to completion
      sort(process_queue,compare_remaining_time);
    }
    //print_queue_srtc(process_queue);

    //if there is no scheduled process, then check process queue and schedule it
		if(scheduled_process == NULL && process_queue->size > 0) {
			scheduled_process = (process_stat *) dequeue(process_queue);

      // If the process has not started before quanta 100, remove the process from the queue and take the next process in queue for execution
      while(t>=100 && scheduled_process->start_time == -1)
      {
        scheduled_process = (process_stat *) dequeue(process_queue);
      }
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

        //printf("Process name %c", proc->process_id);
        //printf("Process name remaining time %f\n", (proc->rt - scheduled_process->rt));


        if(scheduled_process->rt >= proc->rt) {
          scheduled_process->end_time = t;
          add_node(ll,scheduled_process);
          scheduled_process = NULL;
        }
      }else {
    			printf("_");
    	}
  		//increase the time
  		t++;
  }
  //Print Process Stat
  return print_policy_stat(ll);
}
