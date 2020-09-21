//
// Created by dushn on 23.7.20..
//

#include "cpu_usage.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/socket.h>


#define BUFFER_SIZE 1024
pthread_cond_t cpu_cond= PTHREAD_COND_INITIALIZER;
bool test;

__uint64_t *jiffies_total_delta = NULL;
__uint64_t *jiffies_system = NULL;
__uint64_t *jiffies_total = NULL;
__uint64_t *jiffies_user = NULL;
__uint64_t *jiffies_user_old = NULL;
__uint64_t *jiffies_system_old = NULL;
__uint64_t *jiffies_total_old = NULL;

float *cpu_user = NULL;
float *cpu_system = NULL;
__uint64_t *user = NULL;
__uint64_t *user_nice = NULL;
__uint64_t *idle = NULL;
__uint64_t *msystem = NULL;
float *percentage = NULL;

void *send_cpu(void *socket) {

    int sockfd = (*(int *) socket);


    Cpu_usage *cpu_usage_list = NULL;
    Cpu_usage *cpu_usage_list_temp = NULL;
    Data data;


    ssize_t ret = cpu_percentage(cpu_num, &cpu_usage_list);
    if (ret < 0) {
        printf("Error getting cpu data!\n\t");
        pthread_exit(&ret);

    }
    cpu_usage_list_temp = cpu_usage_list;
    for (int i = 0; i < cpu_num; i++) {
        memset(&data, 0, sizeof(Data));
        data.size = CPU_USAGE;

        data.unification.cpu_usage = *cpu_usage_list_temp;

    pthread_mutex_lock(&mutex_send);
    ret = send(sockfd, &data, sizeof(Data), 0);
    pthread_mutex_unlock(&mutex_send);

    if (ret < 0) {
        printf("Error sending data!\n\t");
        pthread_mutex_unlock(&mutex_send);
        free(cpu_usage_list);
        pthread_exit(&ret);

    }
    if (ret == 0) {

        printf("socket closed\n");
        pthread_mutex_unlock(&mutex_send);
        free(cpu_usage_list);
        ret = -1;
        pthread_exit(&ret);
    }
        cpu_usage_list_temp++;
    }

    free(cpu_usage_list);

    pthread_exit(NULL);
}

__int32_t cpu_number() {

    __int32_t c = 1; //cpu number must be at least 1
    FILE *file;
    char *filename = "/proc/cpuinfo";
    char buffer[1024];
    char *buffer2 = "processor";

    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, 1024, file) == NULL) {
        printf("the file cant open %s\n", filename);
        return 0;
    }

    while (fgets(buffer, 1024, file) != NULL) {
        if (strncmp(buffer2, buffer, strlen(buffer2)) == 0) {

            c++;
        }


    }
    fclose(file);

    jiffies_total_delta = calloc((size_t) (c + 1), sizeof(__uint64_t));
    if (jiffies_total_delta == NULL) {

        free(jiffies_total_delta);
        printf("calloc error %d \n", errno);
        return 0;
    }

    jiffies_system = calloc((size_t) c, sizeof(__uint64_t));
    if (jiffies_system == NULL) {

        free(jiffies_system);
        printf("calloc error %d \n", errno);
        return 0;
    }
    jiffies_total = calloc((size_t) c, sizeof(__uint64_t));
    if (jiffies_total == NULL) {

        free(jiffies_total);
        printf("calloc error %d \n", errno);
        return 0;
    }
    jiffies_user = calloc((size_t) c, sizeof(__uint64_t));
    if (jiffies_user == NULL) {

        free(jiffies_user);
        printf("calloc error %d \n", errno);
        return 0;
    }
    jiffies_user_old = calloc((size_t) c, sizeof(__uint64_t));
    if (jiffies_user_old == NULL) {

        free(jiffies_user_old);
        printf("calloc error %d \n", errno);
        return 0;
    }
    jiffies_system_old = calloc((size_t) c, sizeof(__uint64_t));
    if (jiffies_system_old == NULL) {

        free(jiffies_system_old);
        printf("calloc error %d \n", errno);
        return 0;
    }
    jiffies_total_old = calloc((size_t) c, sizeof(__uint64_t));
    if (jiffies_total_old == NULL) {

        free(jiffies_total_old);
        printf("calloc error %d \n", errno);
        return 0;
    }

    cpu_user = calloc((size_t) c, sizeof(float));
    if (cpu_user == NULL) {

        free(cpu_user);
        printf("calloc error %d \n", errno);
        return 0;
    }

    cpu_system = calloc((size_t) c, sizeof(float));
    if (cpu_system == NULL) {

        free(cpu_system);
        printf("calloc error %d \n", errno);
        return 0;
    }

    user = calloc((size_t) c, sizeof(__uint64_t));
    if (user == NULL) {

        free(user);
        printf("calloc error %d \n", errno);
        return 0;
    }
    user_nice = calloc((size_t) c, sizeof(__uint64_t));
    if (user_nice == NULL) {

        free(user_nice);
        printf("calloc error %d \n", errno);
        return 0;
    }
    idle = calloc((size_t) c, sizeof(__uint64_t));
    if (idle == NULL) {

        free(idle);
        printf("calloc error %d \n", errno);
        return 0;
    }
    msystem = calloc((size_t) c, sizeof(__uint64_t));
    if (msystem == NULL) {

        free(msystem);
        printf("calloc error %d \n", errno);
        return 0;
    }

    percentage = calloc((size_t) c, sizeof(float));
    if (percentage == NULL) {

        free(percentage);
        printf("calloc error %d \n", errno);
        return 0;
    }



    return c;
}

int cpu_percentage(int cpu_count, Cpu_usage **array) {




    Cpu_usage *temp_array;




    FILE *file;
    char *filename = "/proc/stat";
    char buffer[BUFFER_SIZE];
    char dummy[4];

    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, BUFFER_SIZE, file) == NULL){

        printf("error opening file\n");
        return -1;
    }




    for(int j=0;j<cpu_count;j++){
        if(fgets(buffer, BUFFER_SIZE, file) != NULL){

            sscanf(buffer, "%s %" SCNu64 " %" SCNu64 " %" SCNu64 " %" SCNu64 "", dummy, &user[j],
                   &user_nice[j], &msystem[j], &idle[j]
            );
        }



    }
    fclose(file);

    pthread_mutex_lock(&mutex_jiff);
    test=false;
    for (int i = 0; i < cpu_count; i++) {


        jiffies_user_old[i] = jiffies_user[i];
        jiffies_system_old[i] = jiffies_system[i];
        jiffies_total_old[i] = jiffies_total[i];

        jiffies_user[i] = user[i] + user_nice[i];
        jiffies_system[i] = msystem[i];

        jiffies_total[i] = jiffies_user[i] + jiffies_system[i] + idle[i];



        if (jiffies_total[i] > jiffies_total_old[i]) {


            jiffies_total_delta[i] = jiffies_total[i] - jiffies_total_old[i];

            cpu_user[i] = (float)(jiffies_user[i] - jiffies_user_old[i])  / (float) (jiffies_total_delta[i]);
            cpu_system[i] =(float) (jiffies_system[i] - jiffies_system_old[i]) / (float) (jiffies_total_delta[i]);

        }

        percentage[i] = (cpu_user[i] + cpu_system[i]) * 100;

        jiffies_total_delta[cpu_num] += jiffies_total_delta[i];

    }




        test=true;
   pthread_cond_signal(&cpu_cond);

    pthread_mutex_unlock(&mutex_jiff);

    temp_array = calloc((size_t) cpu_count, sizeof(Cpu_usage));
    if (temp_array == NULL) {
        free(temp_array);

        printf("calloc error %d \n", errno);

        return -1;
    }


    for (int i = 0; i < cpu_count; i++) {


        if (sprintf(temp_array[i].percentage, "%f", percentage[i]) < 0) {

            printf("converting didn't work %s \n", temp_array[i].percentage);
            free(temp_array);

            return -1;
    }


    }
    *array = temp_array;


    return 0;
}


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



int get_cpu_percent(__uint64_t jiffies_user, __uint64_t jiffies_system, Task *task) {

    struct Cpu_data old;
    struct Cpu_data new;
   struct DataItem *temp;
    float cpu_user=0;
    float cpu_system=0;



    bool ima = false;
    new.cpu_system=jiffies_system;
    new.cpu_user=jiffies_user;


    pthread_mutex_lock(&mutex_jiff);

    old= search(&ima, new, task);




    if (ima == false) {

        temp=(struct DataItem *) calloc(1, sizeof(struct DataItem));

        if (temp == NULL) {
            free(temp);
            printf("calloc error %d \n", errno);
            return -1;
        }

        temp->cpu_user=jiffies_user;
        temp->cpu_system=jiffies_system;
        temp->pid=task->pid;
        strcpy(temp->name,task->name);
        temp->next=task_details;
        task_details=temp;



        hash_size++;



    }




    while(test==false)
    pthread_cond_wait(&cpu_cond,&mutex_jiff);

    if (jiffies_total_delta[cpu_num] > 0) {

        cpu_user = (float) ((jiffies_user) - (old.cpu_user)) * 100 / (float) (jiffies_total_delta[cpu_num]);
        cpu_system = (float) ((jiffies_system - old.cpu_system) * 100) / (float) jiffies_total_delta[cpu_num];

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

void free_cpu() {

    if (jiffies_total_delta != NULL) {
        free(jiffies_total_delta);
    }
    if (jiffies_system != NULL) {

        free(jiffies_system);

    }
    if (jiffies_total != NULL) {

        free(jiffies_total);

    }
    if (jiffies_user != NULL) {

        free(jiffies_user);

    }
    if (jiffies_user_old != NULL) {

        free(jiffies_user_old);

    }
    if (jiffies_system_old != NULL) {

        free(jiffies_system_old);

    }
    if (jiffies_total_old != NULL) {

        free(jiffies_total_old);

    }
    if (cpu_user != NULL) {

        free(cpu_user);

    }
    if (cpu_system != NULL) {

        free(cpu_system);

    }
    if (user != NULL) {

        free(user);

    }
    if (user_nice != NULL) {

        free(user_nice);

    }

    if (idle != NULL) {

        free(idle);

    }
    if (msystem != NULL) {

        free(msystem);

    }
    if (percentage != NULL) {

        free(percentage);

    }

};
