//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_FUNCTIONS_H
#define DIPLOMSKI_FUNCTIONS_H

#include <sys/types.h>
#include <unistd.h>
#include "common.h"
#include "pthread.h"

#include <arpa/inet.h>


#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <wait.h>
bool            thread_break;/*bool that if set to false tells other threads to exit before they allocate memory or open files*/
pthread_mutex_t mutex_send;/*mutex that protects the socket */
pthread_cond_t  cpu_cond; /*condtion_variable that tells if the jiffies_total_delta variable can be read*/
typedef struct Commands Commands; /* structure used to tell thread what to do*/
struct __attribute__((__packed__))Commands {


    char command[16];
    char task_id[256];
};
typedef struct Thread_pack  Thread_pack;

struct Thread_pack {

    int socket;
    pthread_t thread_id;
    ssize_t ret_val;
};
Thread_pack thr_p_main[2];
struct tm local_time; /*structure used to set task duration*/

struct tm1 begin_time;  /*structure used to set task duration*/

void *sending(void *socket);

void *accept_command(void *socket);

ssize_t test_send(int socket);

ssize_t test_recv(int socket);

void send_signal_to_task(char *task_id, char *signal);

int connection(char *port, int *socket_send, int *socket_command);

void *get_in_addr(struct sockaddr *sa);

ssize_t send_data(int socket, Data data);

ssize_t send_cpu_num(int socket,int cpu_number);
#endif //DIPLOMSKI_FUNCTIONS_H
