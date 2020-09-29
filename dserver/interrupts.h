//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_INTERRUPTS_H
#define DIPLOMSKI_INTERRUPTS_H

#include "common.h"


int interrupt_usage(Interrupts **array, __int32_t *num);

void * send_interrupts(void *socket);

void compare_interrupts(Interrupts *new_interrupts, Interrupts *old_interrupts, Interrupts *send_interrupts, int n);

int myCompare(const void *a, const void *b);

void clean_interrupts();

#endif //DIPLOMSKI_INTERRUPTS_H
