//
// Created by dushn on 23.7.20..
//

#include "cpu_usage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/socket.h>


static __uint64_t jiffies_total_delta[5] = {0, 0, 0, 0, 0};

void send_cpu(void *socket){

    int sockfd=(*(int*)socket);
    Cpu_usage cpu_usage={0};
    Data data={0};
    __int32_t cpu_num;

    cpu_num = cpu_number();

   ssize_t ret=  cpu_percentage(cpu_num, &cpu_usage);
    if (ret < 0) {
        printf("Error getting cpu data!\n\t");
       exit(1);

    }
    memset(&data,0,sizeof(Data));
    data.size=CPU_USAGE;

    data.unification.cpu_usage=(Cpu_usage)cpu_usage;

    ret = send(sockfd, &data, sizeof(Data), 0);


    if (ret < 0) {
        printf("Error sending data!\n\t");
       exit(1);

    }
    if (ret == 0) {

        printf("socket closed\n");
        exit(1);
    }


}
int cpu_number() {

    int c = 1; //cpu number must be at least 1
    FILE *file;
    char *filename = "/proc/cpuinfo";
    char buffer[1024];
    char *buffer2 = "processor";

    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, 1024, file) == NULL)
        exit(1);
    while (fgets(buffer, 1024, file) != NULL) {
        if (strncmp(buffer2, buffer, strlen(buffer2)) == 0) {

            c++;
        }


    }
    fclose(file);


    return c;
}

int cpu_percentage(int cpu_count, Cpu_usage *cpu_usage) {


    float cpu_user[4] = {0, 0, 0, 0};
    float cpu_system[4] = {0, 0, 0, 0};
    __uint64_t user[4] = {0, 0, 0, 0};
    __uint64_t user_nice[4] = {0, 0, 0, 0};
    __uint64_t idle[4] = {0, 0, 0, 0};
    __uint64_t system[4] = {0, 0, 0, 0};
    float percentage[4] = {0, 0, 0, 0};



    static __uint64_t jiffies_system[4] = {0, 0, 0, 0};
    static __uint64_t jiffies_total[4] = {0, 0, 0, 0};
    static __uint64_t jiffies_user[4] = {0, 0, 0, 0};
    static __uint64_t jiffies_user_old[4] = {0, 0, 0, 0};
    static __uint64_t jiffies_system_old[4] = {0, 0, 0, 0};
    static __uint64_t jiffies_total_old[4] = {0, 0, 0, 0};

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
                   &user_nice[j], &system[j], &idle[j]
            );
        }



    }
    fclose(file);


    for (int i = 0; i < cpu_count; i++) {


        jiffies_user_old[i] = jiffies_user[i];
        jiffies_system_old[i] = jiffies_system[i];
        jiffies_total_old[i] = jiffies_total[i];

        jiffies_user[i] = user[i] + user_nice[i];
        jiffies_system[i] = system[i];
        jiffies_total[i] = jiffies_user[i] + jiffies_system[i] + idle[i];



        if (jiffies_total[i] > jiffies_total_old[i]) {


            jiffies_total_delta[i] = jiffies_total[i] - jiffies_total_old[i];

            cpu_user[i] = (float)(jiffies_user[i] - jiffies_user_old[i])  / (float) (jiffies_total_delta[i]);
            cpu_system[i] =(float) (jiffies_system[i] - jiffies_system_old[i]) / (float) (jiffies_total_delta[i]);

        }

        percentage[i] = (cpu_user[i] + cpu_system[i]) * 100;



    }


    if (sprintf(cpu_usage->percentage0, "%f", percentage[0]) < 0) {

        printf("converting didn't work %s \n", cpu_usage->percentage0);
        return -1;
    }
    if (sprintf(cpu_usage->percentage1, "%f", percentage[1]) < 0) {

        printf("converting didn't work %s \n", cpu_usage->percentage1);
        return -1;
    }
    if (sprintf(cpu_usage->percentage2, "%f", percentage[2]) < 0) {

        printf("converting didn't work %s \n", cpu_usage->percentage2);
        return -1;
    }
    if (sprintf(cpu_usage->percentage3, "%f", percentage[3]) < 0) {

        printf("converting didn't work %s \n", cpu_usage->percentage3);
        return -1;
    }


    jiffies_total_delta[4] =
            jiffies_total_delta[0] + jiffies_total_delta[1] + jiffies_total_delta[2] + jiffies_total_delta[3];

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


    if (jiffies_user < old.cpu_user || jiffies_system < old.cpu_system) {

        return 1;
    }

    if (jiffies_total_delta[4] > 0) {

        cpu_user = (float) (( jiffies_user) - (old.cpu_user))* 100 /
                    (float) (jiffies_total_delta[4]);
        cpu_system =(float) ((jiffies_system - old.cpu_system) * 100) / (float) jiffies_total_delta[4];

    } else {

        cpu_user = cpu_system = 0;

    }

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


