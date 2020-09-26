//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_FUNCTIONS_H
#define DIPLOMSKI_FUNCTIONS_H

#include <sys/types.h>
#include <unistd.h>
#include "common.h"
#include "pthread.h"

bool thread_break;
pthread_mutex_t mutex_send;
pthread_cond_t cpu_cond;
typedef struct _Commands Commands;
struct __attribute__((__packed__))_Commands {


    bool show;
    __uint32_t mem;
    char command[16];
    char task_id[256];
};

void *sending(void *socket);

void *accept_c(void *socket);

ssize_t test_send(int socket);

ssize_t test_recv(int socket);

void send_signal_to_task(char *task_id, char *signal);

struct tm local_time;

struct tm1 begin_time;

#endif //DIPLOMSKI_FUNCTIONS_H
