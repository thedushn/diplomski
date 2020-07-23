//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_FUNCTIONS_H
#define DIPLOMSKI_FUNCTIONS_H


#include "gtk/gtk.h"
#include "common.h"

ssize_t test_recv(int socket);


void primanje3(int socket, Cpu_usage *cpu_usage1, Network *network, Memory_usage *memory_usage, GArray *array_devices,
               GArray *array_int, GArray *array_tasks
);


#endif //DIPLOMSKI_FUNCTIONS_H
