//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_INTERRUPTS_H
#define DIPLOMSKI_INTERRUPTS_H

#include "common.h"


int interrupt_usage2(Interrupts **array, __int32_t *j);

void * send_interrupts(void *socket);

void sort2(Interrupts *new_interrupts, Interrupts *old_interrupts, Interrupts **send_interrupts, int n);

void sort(Interrupts *array, int n);

void clean_interrupts();

#endif //DIPLOMSKI_INTERRUPTS_H
