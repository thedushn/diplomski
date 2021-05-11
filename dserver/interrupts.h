//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_INTERRUPTS_H
#define DIPLOMSKI_INTERRUPTS_H

#include "common.h"


int interrupt_usage(Interrupts **array, __int32_t *num);
int interrupt_usage2(Interrupts2 **array);
void * send_interrupts(void *socket);

void compare_interrupts(Interrupts *new_interrupts, Interrupts *old_interrupts, Interrupts *send_interrupts, int n);

int myCompare(const void *a, const void *b);

void clean_interrupts();
void clean_interrupts2();
int insert_new_interrupts(Interrupts2 **array, Interrupts2 *new_interrupts);

void sort_interrupts(Interrupts2 **array,int number);

bool scan_numbers(__uint64_t *CPU, char **ptr, const char *endp, int  *index, int *cpu_index, bool end_of_string);

bool irq_scan(Interrupts2 *element,char **ptr,const char *endp,int  *index,bool end_of_string);

bool scan_name(Interrupts2 *array,char *string,const char *endptr,int *i,bool end_of_string);

void delete_old_interrupts(Interrupts2 **array,int *number);
void delete_old_interrupts2(Interrupts2 **array);

void refresh_interrupt_data(Interrupts2 *new_interrupts, Interrupts2 *old_interrupts);

int compare_interrupts2(Interrupts2 *new_interrupts, Interrupts2 *old_interrupts, Interrupts2 **array);

int buffer_interrupts(Interrupts2 *array, char *cp_array, int *cpu_indx);
void * send_interrupts2(void *socket);
void sort_interrupts2(Interrupts2 **array);
#endif //DIPLOMSKI_INTERRUPTS_H
