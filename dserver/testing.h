//
// Created by dushn on 23.9.20..
//

#include "common.h"

#ifndef DSERVER_TESTING_H
#define DSERVER_TESTING_H

int interrupts_write(Interrupts *array, Interrupts *array2, char *filename, int size);
int memory_write(Memory_usage *memory_usage);

int task_write(Task *task);
int netw_write(Network network);
#endif //DSERVER_TESTING_H
