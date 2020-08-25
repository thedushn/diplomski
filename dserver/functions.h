//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_FUNCTIONS_H
#define DIPLOMSKI_FUNCTIONS_H

#include <sys/types.h>
#include <unistd.h>
#include "common.h"



void *sending(void *socket);

void *accept_c(void *socket);

ssize_t test_send(int socket);

void send_signal_to_task(char *task_id, char *signal);

struct tm local_time;

struct tm1 begin_time;

#endif //DIPLOMSKI_FUNCTIONS_H
