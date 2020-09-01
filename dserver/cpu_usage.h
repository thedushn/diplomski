//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_CPU_USAGE_H
#define DIPLOMSKI_CPU_USAGE_H

#include "common.h"



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


struct DataItem *task_details;
int hash_size;
int cpu_number();

int cpu_percentage(int ncpu, Cpu_usage *cpu_usage);
void send_cpu(void *socket);



int get_cpu_percent(__uint64_t jiffies_user, __uint64_t jiffies_system, Task *task);


struct Cpu_data search(bool *ima, struct Cpu_data new_data, Task *task);

#endif //DIPLOMSKI_CPU_USAGE_H
