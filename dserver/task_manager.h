//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_TASK_MANAGER_H
#define DIPLOMSKI_TASK_MANAGER_H


#include "common.h"
#include "pthread.h"
typedef struct _Thread_task Thread_task; /*structure of a linked list used to gather data about tasks and results of return values*/

struct _Thread_task{

    __uint32_t pid;
    int result;
    Task *task;
    pthread_t pthread;
    Thread_task *next;


};

void * send_task(void *socket);

int get_task_list(T_Collection **array, __int32_t *task_num);

void differenceBetweenTimePeriod(struct tm start, struct tm1 stop, struct tm1 *diff);

void * get_task_details(void *ptr);

#endif //DIPLOMSKI_TASK_MANAGER_H
