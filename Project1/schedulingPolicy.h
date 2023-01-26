#ifndef _scheduling_policy_h
#define _scheduling_policy_h



//enum State {UNUSED, EMBRYO, SLEEPING, RUNNING, ZOMBIE};

struct process_stat_s {
	(Process *) proc;
	unsigned float wait_time;
	unsigned float completion_time;
	unsigned float turnaround_time;
	unsigned float response_time;

	enum State state;
}
typedef struct process_stat_s ProcessStat;

struct stat_s {
	(linked_list *) process_queue;
	(linked_list *) time_chart;
	unsigned float avg_turnaround_time;
	unsigned float avg_waiting_time;
	unsigned float avg_response_time;
	unsigned float throughput;
	int total_quanta;
}
typedef struct stat_s SchedulingStat;
#endif
