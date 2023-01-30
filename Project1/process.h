#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "utility.h"

//create a process with the parameters - Processid, arrival time, run time, priority
struct process_s {
	unsigned char process_id;
	float at;
	float rt;
	unsigned char priority; 
};

typedef struct process_s process;

process * create_process(char process_id, float at, float rt, unsigned char priority);

process * get_copy_of_process(process * proc);

linked_list * generate_processes(int n);

#endif