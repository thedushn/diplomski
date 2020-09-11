//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_FUNCTIONS_H
#define DIPLOMSKI_FUNCTIONS_H


#include "gtk/gtk.h"
#include "common.h"

ssize_t test_recv(int socket);

ssize_t test_send(int socket);



int
data_transfer(int socket, Cpu_usage *cpu_usage1, Network *network, Memory_usage *memory_usage,
              T_Collection **task_array, D_Collection **devices_array, int *task_num, int *dev_num);
int command_sender(char *text);

void printerino();

#endif //DIPLOMSKI_FUNCTIONS_H
