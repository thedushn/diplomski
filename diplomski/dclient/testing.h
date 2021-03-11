//
// Created by dushn on 23.9.20..
//

#ifndef DCLIENT_TESTING_H
#define DCLIENT_TESTING_H


#include "common.h"
#include "main_header.h"

int interrupts_write(I_Collection *array);

int cpu_write(Cpu_usage cpu_usage);
int netw_write(unsigned long transmited, unsigned long received);
int memory_write(Memory_usage *memory_usage);
int task_write(T_Collection *array);
int device_write(D_Collection *array);
int netw_calculate(float *transmited, float *received);
int ifstat_calculate(float *received_kb, float *transmitted_kb);
int task_sort(T_Collection **array, int number);
int cpu_read(Cpu_list **array);
#endif //DCLIENT_TESTING_H
