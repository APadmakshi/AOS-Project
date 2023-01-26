#ifndef _stat_h_
#define _stat_h_
#include "process.h"

enum State {UNUSED, EMBRYO, SLEEPING, RUNNING, ZOMBIE};

struct process_stat_s {
	process * proc;
	float wait_time;
	float turnaround_time;
	float response_time;

	float start_time;
	float end_time;
	float run_time;

	enum State state;
};
typedef struct process_stat_s process_stat;

struct stat_s {
	linked_list * process_queue;
	linked_list * time_chart;
	float avg_turnaround_time;
	float avg_waiting_time;
	float avg_response_time;
	float throughput;
	int total_quanta;
};
typedef struct stat_s scheduling_stat;

struct average_stats_s{
	float avg_response_time;
	float avg_wait_time;
	float avg_turnaround;
	float avg_throughput;
};

typedef struct average_stats_s average_stats;

average_stats first_come_first_serve_np(linked_list * processes);
average_stats shortest_job_first_np(linked_list * processes);
average_stats shortest_remaining_time_p(linked_list * processes);
average_stats round_robin_p(linked_list * processes,int time_slice);
average_stats highest_priority_first_p(linked_list * processes);
average_stats highest_priority_first_np(linked_list * processes);
average_stats highest_priority_first_p_aging(linked_list * processes);
average_stats highest_priority_first_np_aging(linked_list * processes);

// Print Stat//
average_stats print_policy_stat(linked_list * ll);
#endif
