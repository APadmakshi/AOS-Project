#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "utility.h"

struct process_s {
	unsigned char pid;
	float arrival_time;
	float run_time;
	unsigned char priority; 
};

typedef struct process_s process;

process * create_process(char pid, float arrival_time, float run_time, unsigned char priority);

process * get_copy_of_process(process * proc);

linked_list * generate_processes(int n);

#endif