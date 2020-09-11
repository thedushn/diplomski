//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_TASK_MANAGER_H
#define DIPLOMSKI_TASK_MANAGER_H

#include "common.h"
typedef struct _Thread_task Thread_task;

struct _Thread_task{

    int pid;
    int result;
    Task *task;
    pthread_t pthread;
    Thread_task *next;

};
void * send_task(void *socket);
int get_task_list(T_Collection **array, __int32_t *task_num);
int get_task_list2(T_Collection **array, __int32_t *task_num);

int get_task_details(int pid, Task *task);

void differenceBetweenTimePeriod(struct tm start, struct tm1 stop, struct tm1 *diff);
void * get_task_details2(void *ptr);

#endif //DIPLOMSKI_TASK_MANAGER_H
