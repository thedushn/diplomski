//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_FUNCTIONS_H
#define DIPLOMSKI_FUNCTIONS_H

#include <sys/types.h>
#include <unistd.h>
#include "common.h"
#include "pthread.h"

bool            thread_break;/*bool that if set to false tells other threads to exit before they allocate memory or open files*/
pthread_mutex_t mutex_send;/*mutex that protects the socket */
pthread_cond_t  cpu_cond; /*condtion_variable that tells if the jiffies_total_delta variable can be read*/
typedef struct _Commands Commands; /* structure used to tell thread what to do*/
struct __attribute__((__packed__))_Commands {


    char command[16];
    char task_id[256];
};

struct tm local_time; /*structure used to set task duration*/

struct tm1 begin_time;  /*structure used to set task duration*/

void *sending(void *socket);

void *accept_command(void *socket);

ssize_t test_send(int socket);

ssize_t test_recv(int socket);

void send_signal_to_task(char *task_id, char *signal);



#endif //DIPLOMSKI_FUNCTIONS_H
