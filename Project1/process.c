#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "utility.h"
#include <time.h>

int compare(void * data1, void * data2) {
	process *p1 = (process *)data1;
	process *p2 = (process *)data2;
	if(p1->at < p2->at) {
		return -1;
	} else if(p1->at == p2->at){
		return 0;
	} else {
		return 1;
	}
}

linked_list * generate_processes(int n) {
	linked_list * process_list = create_linked_list();
	char process_id = 'A';
	unsigned char priority;
	float at, rt;
	time_t t;


   /* Intializes random number generator */
   srand((unsigned) time(&t));

	if(process_list == NULL) fprintf(stderr,"Not able to create Linked Lists\n");

	while(n--) {
		at = rand() % 100;
		rt = (float)((rand() % 10)+1);
		priority = (rand()%4)+1;
		process *p = create_process(process_id,at,rt,priority);

		add_node(process_list,p);
		process_id++;
	}

	sort(process_list,compare);

	node * ptr = process_list->head;
	process_id = 'A';
	while(ptr!=NULL) {
		((process *)ptr->data)->process_id = process_id;
		if(process_id == 'Z') process_id = 'a' - 1;
		process_id++;
		ptr=ptr->next;
	}

	return process_list;
}

process * create_process(char process_id, float at, float rt, unsigned char priority) {
	process * proc = (process *) malloc(sizeof(process));
	proc->process_id = process_id;
	proc->at = at;
	proc->rt = rt;
	proc->priority = priority;
	return proc;
}

process * get_copy_of_process(process * proc){}
