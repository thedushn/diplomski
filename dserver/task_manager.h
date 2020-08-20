//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_TASK_MANAGER_H
#define DIPLOMSKI_TASK_MANAGER_H

#include "common.h"


int get_task_list(T_Collection **array, __int32_t *task_num);

int get_task_details(int pid, Task *task);

void differenceBetweenTimePeriod(struct tm start, struct tm1 stop, struct tm1 *diff);

#endif //DIPLOMSKI_TASK_MANAGER_H
