//
// Created by dushn on 4.9.20..
//

#ifndef DCLIENT_WRITTING_H
#define DCLIENT_WRITTING_H

#include "common.h"
#include "main_header.h"


int interrupts_write(Interrupts *array);

int cpu_write(Cpu_usage cpu_usage);

int cpu_read(Collection **array, Time_Managment **time_managment, int *number, int *step_counter);

#endif //DCLIENT_WRITTING_H
