#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat * create_process_stat(process* proc);

// compares priority and arival time for hpf preemptive
int compare_priority(void * data1, void * data2)
{
  process_stat * ps1 = (process_stat *) data1;
	process_stat * ps2 = (process_stat *) data2;
	if(((((process *)ps1->proc)->priority)) < ((((process *)ps2->proc)->priority))) {
		return -1;
	}
	else if (((((process *)ps1->proc)->priority)) == ((((process *)ps2->proc)->priority)) &&  (((process *)ps1->proc)->at < (((process *)ps2->proc)->at)))
  	{
    		return -1;
  	}
   	else 
	{
		return 1;
	}
}

// testing
// function to print contents of queue during hpf preemptive
void printqueue(queue * q)
{
  if (q->head != NULL)
  {
    node * n = q->head;
    printf("Queue Contents:\n");
    while(n != NULL)
    {
      process_stat* ps = n->data;
      process * p = ps->proc;
      printf("Process id: %c priority: %u arrival time: %f \n",p->process_id,p->priority,p->at);
      n = n->next;
    }
  }
  return;
}

// Implementation of HPF preemptive

average_stats highest_priority_first_p(linked_list * processes)
{
  int t = 0; // quanta
  // creation of a queue of processes based on priority
  queue * process_queue_1 = create_queue();
  queue * process_queue_2 = create_queue();
  queue * process_queue_3 = create_queue();
  queue * process_queue_4 = create_queue();
  // creation of linked list for managaing process order in preemeption
  linked_list * ll_1 = create_linked_list();
  linked_list * ll_2 = create_linked_list();
  linked_list * ll_3 = create_linked_list();
  linked_list * ll_4 = create_linked_list();

  node * process_pointer = processes->head;
  if(processes->head == NULL) {
		fprintf(stderr,"No process to schedule\n");
	}
  printf("\nHPF Preemptive:\n");
  //while process queue is not empty or time quanta is less than 100
  process_stat * scheduled_process = NULL;
  while(t<100 || scheduled_process!=NULL )
  {
    if(scheduled_process!=NULL)
    {
      if(scheduled_process->proc->priority == 1) enqueue(process_queue_1,scheduled_process);
      if(scheduled_process->proc->priority == 2) enqueue(process_queue_2,scheduled_process);
      if(scheduled_process->proc->priority == 3) enqueue(process_queue_3,scheduled_process);
      if(scheduled_process->proc->priority == 4) enqueue(process_queue_3,scheduled_process);

    }
    //check for new incoming process and enqueue it in the queue
    if(process_pointer != NULL) {
	process * new_process = (process *)(process_pointer->data);
	while(process_pointer !=NULL && new_process->at <= t) {
        	if(new_process->priority == 1) enqueue(process_queue_1,create_process_stat(new_process));
        	if(new_process->priority == 2) enqueue(process_queue_2,create_process_stat(new_process));
        	if(new_process->priority == 3) enqueue(process_queue_3,create_process_stat(new_process));
        	if(new_process->priority == 4) enqueue(process_queue_3,create_process_stat(new_process));
		sort(process_queue_1,compare_priority);
                sort(process_queue_2,compare_priority);
        	sort(process_queue_3,compare_priority);
        	sort(process_queue_4,compare_priority);
		process_pointer = process_pointer->next;
		if(process_pointer!=NULL)
        	{
					new_process = (process *)(process_pointer->data);
        	}	
	}
      // sort all the processes that have arrived based on their remaining running time to completion
      //sort(process_queue,compare_priority);
    }

    //if there is no scheduled process, then check process queue and schedule it
    if(scheduled_process == NULL) {
      if (process_queue_1->size > 0) scheduled_process = (process_stat *) dequeue(process_queue_1);
      else if (process_queue_2->size > 0) scheduled_process = (process_stat *) dequeue(process_queue_2);
      else if (process_queue_3->size > 0) scheduled_process = (process_stat *) dequeue(process_queue_3);
      else if (process_queue_4->size > 0) scheduled_process = (process_stat *) dequeue(process_queue_4);
      if (t>=100 && scheduled_process->start_time == -1){
        //free(scheduled_process);
        scheduled_process = NULL;
        continue;
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

        if(scheduled_process->rt >= proc->rt) {
          scheduled_process->end_time = t;
          if(scheduled_process->proc->priority == 1) add_node(ll_1,scheduled_process);
          else if(scheduled_process->proc->priority == 2) add_node(ll_2,scheduled_process);
          else if(scheduled_process->proc->priority == 3) add_node(ll_3,scheduled_process);
          else if(scheduled_process->proc->priority == 4) add_node(ll_4,scheduled_process);
          //add_node(ll,scheduled_process);
          scheduled_process = NULL;
          //free(scheduled_process);
        }
      }else {
    			printf("_");
  		}
  		//increase the quanta
  		t++;
    }
    //Print Process Stat
    average_stats avg1,avg2,avg3,avg4,avg;
    printf("\nFor Priority - Queue - 1");
    avg1 = print_policy_stat(ll_1);
    printf("\nFor Priority - Queue - 2");
    avg2 = print_policy_stat(ll_2);
    printf("\nFor Priority - Queue - 3");
    avg3 = print_policy_stat(ll_3);
    printf("\nFor Priority - Queue - 4");
    avg4 = print_policy_stat(ll_4);

    avg.avg_response_time = (avg1.avg_response_time + avg2.avg_response_time + avg3.avg_response_time + avg4.avg_response_time)/4 ;
    avg.avg_wait_time = (avg1.avg_wait_time + avg2.avg_wait_time + avg3.avg_wait_time + avg4.avg_wait_time)/4 ;
    avg.avg_turnaround = (avg1.avg_turnaround + avg2.avg_turnaround + avg3.avg_turnaround + avg4.avg_turnaround)/4 ;
    avg.avg_throughput = (avg1.avg_throughput + avg2.avg_throughput + avg3.avg_throughput + avg4.avg_throughput) ;

    printf("\nThe average times of HPF Preemptive for all queues:\n");
    printf("Average Response Time(RT) = %.1f\n",avg.avg_response_time);
    printf("Average Wait Time(WT) = %.1f\n",avg.avg_wait_time);
    printf("Average Turn Around Time(TAT) = %.1f\n",avg.avg_turnaround);

    return avg;

}
