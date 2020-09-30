//
// Created by dushn on 23.7.20..
//

#include "cpu_usage.h"
#include "functions.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/socket.h>


#define BUFFER_SIZE 1024

pthread_cond_t cpu_cond= PTHREAD_COND_INITIALIZER; /*cond_variable that checks if the jiffies_total_delta is being used */
bool test;/*flag for checking if we need to wait for pthread_cond_signal */

__uint64_t jiffies_total_delta[CPU_NUM + 1]; /*total cpu jiffies that occurred */
__uint64_t jiffies_system[CPU_NUM];
__uint64_t jiffies_total[CPU_NUM];
__uint64_t jiffies_user[CPU_NUM];
__uint64_t jiffies_user_old[CPU_NUM];
__uint64_t jiffies_system_old[CPU_NUM];
__uint64_t jiffies_total_old[CPU_NUM];

float      cpu_user[CPU_NUM];
float      cpu_system[CPU_NUM];
__uint64_t user[CPU_NUM];
__uint64_t user_nice[CPU_NUM];
__uint64_t idle[CPU_NUM];
__uint64_t msystem[CPU_NUM];
float      percentage[CPU_NUM];
/*
 * function send_cpu():send data about cpu usage to client
 * input : socket to send data to
 * output : returns a value less then zero if something did not work
 * */
void *send_cpu(void *socket) {


    ssize_t   ret;
    Cpu_usage cpu_usage = {0};
    Data      data;

    pthread_mutex_lock(&mutex_send);
    while (thread_break == false) { /*if other threads have failed close this thread before it allocates any memory*/
        ret = -100;
        pthread_mutex_unlock(&mutex_send);
        pthread_exit(&ret);
    }
    pthread_mutex_unlock(&mutex_send);

    ret = cpu_percentage(&cpu_usage);
    if (ret < 0) {
        printf("Error getting cpu data!\n\t");
        pthread_exit(&ret);

    }


    memset(&data, 0, sizeof(Data));
    data.size = CPU_USAGE;
    cpu_write(cpu_usage);
    data.unification.cpu_usage = cpu_usage;

    pthread_mutex_lock(&mutex_send);

    ret = send((*(int *) socket), &data, sizeof(Data), 0);
    pthread_mutex_unlock(&mutex_send);

    if (ret < 0) {
        printf("Error sending data\n return = %d\n", (int) ret);


        pthread_exit(&ret);

    }
    if (ret == 0) {
        printf("Error sending data\n return = %d\n", (int) ret);
        printf("socket closed\n");


        ret = -1;
        pthread_exit(&ret);
    }


    pthread_exit(&ret);
}

/*
 * function cpu_percentage(): parsing data from /proc/stat
 * input : pointer to structure of data about cpu usage that we want to send to the client
 * output : returns a non zero value if something didn't work correctly
 * */
int cpu_percentage(Cpu_usage *array) {


    FILE *file;
    char *filename = "/proc/stat";
    char buffer[BUFFER_SIZE];
    char dummy[4];

    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, BUFFER_SIZE, file) == NULL){

        printf("error opening file\n");
        return -1;
    }


    for (int j = 0; j < CPU_NUM; j++) {
        if(fgets(buffer, BUFFER_SIZE, file) != NULL){

            sscanf(buffer, "%s %" SCNu64 " %" SCNu64 " %" SCNu64 " %" SCNu64 "", dummy, &user[j],
                   &user_nice[j], &msystem[j], &idle[j]
            );
        }



    }
    fclose(file);

    pthread_mutex_lock(&mutex_jiff);
    test=false;
    for (int i = 0; i < CPU_NUM; i++) {


        jiffies_user_old[i] = jiffies_user[i];
        jiffies_system_old[i] = jiffies_system[i];
        jiffies_total_old[i] = jiffies_total[i];

        jiffies_user[i] = user[i] + user_nice[i];
        jiffies_system[i] = msystem[i];

        jiffies_total[i] = jiffies_user[i] + jiffies_system[i] + idle[i];



        if (jiffies_total[i] > jiffies_total_old[i]) {


            jiffies_total_delta[i] = jiffies_total[i] - jiffies_total_old[i];

            cpu_user[i]   = (float)(jiffies_user[i] - jiffies_user_old[i])  / (float) (jiffies_total_delta[i]);
            cpu_system[i] = (float)(jiffies_system[i] - jiffies_system_old[i]) / (float) (jiffies_total_delta[i]);

        }

        percentage[i] = (cpu_user[i] + cpu_system[i]) * 100;



    }
    jiffies_total_delta[CPU_NUM] = jiffies_total_delta[0]+jiffies_total_delta[1]+jiffies_total_delta[2]+ jiffies_total_delta[3];



        test=true;
   pthread_cond_signal(&cpu_cond);

    pthread_mutex_unlock(&mutex_jiff);


    for (int i = 0; i < CPU_NUM; i++) {


        if (sprintf(array->percentage[i], "%f", percentage[i]) < 0) {

            printf("converting didn't work %s \n", array->percentage[i]);


            return -1;
        }
    }


    return 0;
}
/*
 * function search(): searches the linked list to find a tasks cpu usage data save the old data and replace it with the
 * new cpu usage data
 * input : bool  pointer that we change to  true if the task is already in the list,
 * structure of data about cpu usage that we want to save in the list and the pointer to the task we are checking if it
 * exist in the list
 * output : returns a structure about a tasks cpu usage data
 * */

struct Cpu_data search(bool *ima, struct Cpu_data new, Task *task) {


        struct DataItem *temp=task_details;
        for(int i=0;i<hash_size;i++){

               if(temp->pid==task->pid) {
                   *ima =true;

                      if( strcmp(task->name,temp->name)!=0){//if the pid was already taken
                          strcpy(temp->name,task->name);
                          temp->cpu_user=new.cpu_user;
                          temp->cpu_system=new.cpu_system;
                          return new;
                      }
                   struct Cpu_data old;
                   old.cpu_system=temp->cpu_system;
                   old.cpu_user=temp->cpu_user;

                   temp->cpu_user=new.cpu_user;
                   temp->cpu_system=new.cpu_system;
                   return old;
               }
            temp=temp->next;
        }


    return new;
}


/*
 * function get_cpu_percent(): calculates the cpu percentage that the task is doing
 *
 * input : jiffies_user, jiffies_system, and pointer to a structure Task to which we parse the cpu percent
 * output : returns a non zero value if something goes wrong
 * */
int get_cpu_percent(__uint64_t jiffies_user, __uint64_t jiffies_system, Task *task) {

    struct Cpu_data      old;
    struct Cpu_data      new;
    struct DataItem     *temp;
    float  cpu_user   = 0;
    float  cpu_system = 0;
    bool   exists;

    pthread_mutex_lock(&mutex_jiff);
    exists = false;
    new.cpu_system=jiffies_system;
    new.cpu_user=jiffies_user;


    old= search(&exists, new, task);


    if (exists == false) {/*if the task doesnt exist in the list*/

        temp=(struct DataItem *) calloc(1, sizeof(struct DataItem));

        if (temp == NULL) {
            free(temp);
            printf("calloc error %d \n", errno);
            pthread_mutex_unlock(&mutex_jiff);
            return -1;
        }


        temp->cpu_user  =jiffies_user;
        temp->cpu_system=jiffies_system;
        temp->pid       =task->pid;

        strcpy(temp->name,task->name);

        temp->next      =task_details;
        task_details    =temp;



        hash_size++;



    }




    while(test==false) /*if the cpu usage thread didn't acquire data about jiffies */
    pthread_cond_wait(&cpu_cond,&mutex_jiff);

    if (jiffies_total_delta[CPU_NUM] > 0) {


        cpu_user   = (float) ((jiffies_user) - (old.cpu_user)) * 100 / (float) (jiffies_total_delta[CPU_NUM]);
        cpu_system = (float) ((jiffies_system - old.cpu_system) * 100) / (float) jiffies_total_delta[CPU_NUM];

    } else {

        cpu_user = cpu_system = 0;

    }

    pthread_mutex_unlock(&mutex_jiff);


    if (sprintf(task->cpu_user, "%f", cpu_user) < 0) {

        printf("conversion didn't work %s \n", task->cpu_user);
        return -1;
    }
    if (sprintf(task->cpu_system, "%f", cpu_system) < 0) {

        printf("conversion didn't work %s \n", task->cpu_system);
        return -1;
    }

    return 0;
}

