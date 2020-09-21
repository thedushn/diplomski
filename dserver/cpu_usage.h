//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_CPU_USAGE_H
#define DIPLOMSKI_CPU_USAGE_H

#include "common.h"


pthread_mutex_t mutex_jiff;
struct DataItem {
    __uint64_t cpu_user;
    __uint64_t cpu_system;
    unsigned int pid;
    char name[256];
    struct DataItem *next;
};
struct Cpu_data{
    __uint64_t cpu_user;
    __uint64_t cpu_system;
};

__int32_t cpu_num;
struct DataItem *task_details;
int hash_size;

__int32_t cpu_number();

int cpu_percentage(int ncpu, Cpu_usage **array);
void * send_cpu(void *socket);

void free_cpu();


int get_cpu_percent(__uint64_t jiffies_user, __uint64_t jiffies_system, Task *task);


struct Cpu_data search(bool *ima, struct Cpu_data new_data, Task *task);

#endif //DIPLOMSKI_CPU_USAGE_H
