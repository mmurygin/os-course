#ifndef SCHEDULER_H
#define SCHEDULER_H

void scheduler_setup(int timeslice);
void new_thread(int thread_id);
void exit_thread();
void block_thread();
void wake_thread(int thread_id);
void timer_tick();
int current_thread();

#endif
