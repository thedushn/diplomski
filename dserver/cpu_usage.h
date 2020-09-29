//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_CPU_USAGE_H
#define DIPLOMSKI_CPU_USAGE_H

#include "common.h"
#include "pthread.h"

pthread_mutex_t mutex_jiff; /* mutex that protects the task list */

struct DataItem {           /*structure that contains cpu stats about a task */
    __uint64_t      cpu_user;
    __uint64_t      cpu_system;
    unsigned int    pid;
    char            name[256];
    struct DataItem *next;
};

struct Cpu_data{
    __uint64_t cpu_user;
    __uint64_t cpu_system;
};


struct DataItem *task_details; /*pointer to the link list of task cpu stats*/

int hash_size;   /* number of tasks that are in the list */


int cpu_percentage(Cpu_usage *array);

void * send_cpu(void *socket);


int get_cpu_percent(__uint64_t jiffies_user, __uint64_t jiffies_system, Task *task);


struct Cpu_data search(bool *ima, struct Cpu_data new_data, Task *task);

#endif //DIPLOMSKI_CPU_USAGE_H
