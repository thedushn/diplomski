//
// Created by dushn on 23.7.20..
//

#include "functions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include"sys/socket.h"
#include"pthread.h"


#include "interrupts.h"
#include "cpu_usage.h"
#include "memory_usage.h"
#include "task_manager.h"
#include "devices.h"
#include "network_bandwith.h"




bool devices_show = false;

ssize_t test_send(int socket) {

    ssize_t ret = 0;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    ret = recv(socket, buffer, sizeof(buffer), 0);

    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        return ret;
    }
    if (ret == 0) {

        printf("socket closed\n");
        return ret;
    }
    if (ret < 64) {
        size_t velicina = 64;
        velicina -= ret;
        while (velicina > 0 || velicina < 0) {


            ret = (int) recv(socket, buffer, velicina, 0);
            velicina -= ret;

            if (ret < 0) {

                printf("error receiving data\n %d", (int) ret);
                return ret;
            }
            if (ret == 0) {

                printf("socket closed\n");
                return ret;
            }

        }
    }
    if (strcmp(buffer, "stiglo sve") != 0) {

        printf("conformation didnt get received  \n");

        return -1;
    }

    return 64;
};

void send_prio_to_task(char *task_id, char *signal) {
    int prio = 0;
    if (strcmp(signal, "VERY_LOW") == 0) {
        prio = 15;

    }
    if (strcmp(signal, "LOW") == 0) {
        prio = 5;

    }
    if (strcmp(signal, "NORMAL") == 0) {
        prio = 0;

    }
    if (strcmp(signal, "VERY_HIGH") == 0) {
        prio = -15;

    }
    if (strcmp(signal, "HIGH") == 0) {
        prio = -5;

    }


    char str[4];

    sprintf(str, "%d", prio);
    char command[64] = "renice -n ";
    strncat(command, str, sizeof command);
    strncat(command, " -p ", sizeof command);
    strncat(command, task_id, sizeof command);
    if (system(command) != 0) {

        printf("command failed\n");
    }


}

void send_signal_to_task(char *task_id, char *signal) {

    if (task_id != NULL && signal != NULL) {
        char command[64] = "kill -";
        strncat(command, signal, sizeof(command) );
        strncat(command, " ", sizeof command);
        strncat(command, task_id, sizeof command);

        if (system(command) != 0)
            printf("command failed\n");
    }
}

void *accept_c(void *socket) {

    int sockfd = *(int *) socket;

    Commands commands={0};
    char buffer[1600];
    char text[256];
    char *text1;
    while (1) {
        memset(buffer,0,sizeof(buffer));
        ssize_t ret = recv(sockfd, &buffer, sizeof(buffer), 0);
        if (ret < 0) {
            printf("error condition didn't get received\n");


            pthread_exit(&ret);
        }
        if (ret == 0) {
            printf("error condition did not get received\n");
            printf("ret %d\n", (int) ret);

            pthread_exit(&ret);
        }
        printf("%d\n",(int)ret);
        printf("%s\n",buffer);
        strncpy(text,buffer,7);

        if(strcmp(text,"COMMAND")!=0){
            int g=0;
           g=atoi(text);
           if(g==1){
               devices_show=true;
           }else{
               devices_show=false;
           }
            if (strcmp(commands.task_id, "") != 0 && strcmp(commands.command, "") != 0) {
                if (strcmp(commands.command, "STOP") == 0 ||
                    strcmp(commands.command, "CONT") == 0 ||
                    strcmp(commands.command, "KILL") == 0 ||
                    strcmp(commands.command, "TERM") == 0) {
                    send_signal_to_task(commands.task_id, commands.command);
                } else {

                    send_prio_to_task(commands.task_id, commands.command);
                }

            }
        }

        else {
            text1=strchr(buffer,' ');

            printf("text1 %s\n",text1);
            if (system(text1) != 0)
                printf("command failed\n");
        }







    }

};


void *sending(void *socket) {

    ssize_t ret = 0;


    time_t time1;

    D_Collection *devices_c;
    T_Collection *tasks;


    T_Collection *temp_task;
    D_Collection *temp_dev;

    Interrupts *interrupts = NULL;
    Interrupts *interrupts_main = NULL;
    Interrupts *interrupts_send = NULL;
    __int32_t task_num;
    __int32_t device_num;

    int sockfd = *(int *) socket;
    int result = 0;

    while (1) {


        Memory_usage memory_usage = {0};
        Cpu_usage cpu_usage = {{0}};
        Network network = {0};
        memset(&network, 0, sizeof(Network));


        time1 = time(NULL);

        local_time = *localtime(&time1);
        ///memory
        get_memory_usage(&memory_usage);


        ret = send(sockfd, &memory_usage, sizeof(Memory_usage), 0);

        if (ret < 0) {
            printf("Error sending data!\n\t");
            break;

        }
        if (ret == 0) {

            printf("socket closed\n");
            break;
        }

        ret = test_send(sockfd);
        if (ret < 0) {
            printf("Error receiving  num_packets!\n\t");
            break;

        }
        if (ret == 0) {

            printf("socket closed\n");
            break;
        }



        ///memory_end
        ///cpu
        __int32_t cpu_num = cpu_number();
      ret=  cpu_percentage(cpu_num, &cpu_usage);
        if (ret < 0) {
            printf("Error getting cpu data!\n\t");
            break;

        }
        ret = send(sockfd, &cpu_usage, sizeof(Cpu_usage), 0);

        if (ret < 0) {
            printf("Error sending data!\n\t");
            break;

        }
        if (ret == 0) {

            printf("socket closed\n");
            break;
        }
        ret = test_send(sockfd);
        if (ret < 0) {

            printf("error receiving data\n %d", (int) ret);
            break;
        }
        if (ret == 0) {

            printf("socket closed\n");
            break;
        }

        ///cpu end

        ///network
        result = interface_name(&network);
        if (result != 0) {

            break;
        }
        ret = send(sockfd, &network, sizeof(Network), 0);

        if (ret < 0) {
            printf("Error sending data!\n\t");
            break;

        }
        if (ret == 0) {
            printf("Error sending data!\n\t");
            printf("socket closed\n");
            break;

        }
        ret = test_send(sockfd);
        if (ret < 0) {

            printf("error receiving data\n %d", (int) ret);
            break;
        }
        if (ret == 0) {

            printf("socket closed\n");
            break;
        }




        ///devices
       device_num = 0;

        result = mount_list(&devices_c, &device_num, devices_show);
        if (result != 0) {
            printf("error in mount_list\n");
            break;
        }


        ret = send(sockfd, &device_num, sizeof(__int32_t), 0);
        if (ret < 0) {
            printf("Error sending num_packets!\n\t");

            break;
        }
        if (ret == 0) {

            printf("socket closed\n");
            break;
        }
        ret = test_send(sockfd);
        if (ret < 0) {

            printf("error receiving data\n %d", (int) ret);
            break;
        }
        if (ret == 0) {

            printf("socket closed\n");
            break;
        }
            temp_dev=devices_c;
        for (int i = 0; i < device_num; i++) {
            temp_dev->devices.checked=false;

            ret = (int) send(sockfd, &temp_dev->devices , sizeof(Devices), 0);

            if (ret < 0) {
                printf("Error sending data!\n\t");
                break;

            }
            if (ret == 0) {

                printf("socket closed\n");
                break;
            }
            temp_dev=temp_dev->next;


        }



        /// tasks
         task_num=0;

        result = get_task_list(&tasks, &task_num);
        if (result != 0) {

            printf("error in get_task_list\n");
            break;
        }

        ret = send(sockfd, &task_num, sizeof(__int32_t), 0);
        if (ret < 0) {
            printf("Error sending num_packets!\n\t");

            break;

        }
        if (ret == 0) {

            printf("socket closed\n");
            break;
        }
        ret = test_send(sockfd);
        if (ret < 0) {

            printf("error receiving data\n %d", (int) ret);
            break;
        }
        if (ret == 0) {

            printf("socket closed\n");
            break;
        }
        temp_task=tasks;
        for (int i = 0; i < task_num; i++) {


            ret = send(sockfd, &temp_task->task, sizeof(Task), 0);

            if (ret < 0) {
                printf("Error sending data!\n\t");
                break;

            }
            if (ret == 0) {

                printf("socket closed\n");
                break;
            }
            temp_task=temp_task->next;


        }







        ///interrupts
        __int32_t h = 0;

        result = interrupt_usage2(&interrupts, &h);
        if (result != 0) {

            break;
        }

        if (interrupts_main == NULL) {

            interrupts_main = calloc((size_t) h, sizeof(Interrupts));
            if(interrupts_main==NULL){

                printf("calloc error %d \n", errno);
                free(interrupts_main);


            }
            for (int r = 0; r < h; r++) {

                interrupts_main[r] = interrupts[r];
            }


        }



        sort2(interrupts, interrupts_main, &interrupts_send, h);


        sort(interrupts_send, h);


        for (int r = h - 10; r < h; r++) {


            ret = send(sockfd, &interrupts_send[r], sizeof(Interrupts), 0);


            if (ret < 0) {
                printf("Error sending data!\n\t");
                break;

            }
            if (ret == 0) {

                printf("socket closed\n");
                break;
            }


        }
        ret = test_send(sockfd);
        if (ret < 0) {

            printf("error receiving data\n %d", (int) ret);
            break;
        }
        if (ret == 0) {

            printf("socket closed\n");
            break;
        }







        free(interrupts);
        free(interrupts_send);



        interrupts = NULL;
        interrupts_send = NULL;


        for(int k=0;k<device_num;k++){
            // save reference to first link
            temp_dev = devices_c;

            //mark next to first link as first
            devices_c = devices_c->next;

            //return the deleted link
            free(temp_dev);

        }

        for(int k=0;k<task_num;k++){
            // save reference to first link
            temp_task = tasks;

            //mark next to first link as first
            tasks = tasks->next;

            //return the deleted link
            free(temp_task);


        }
        task_num=0;
    }







    if (interrupts != NULL) {

        free(interrupts);
    }
    if (interrupts_main != NULL) {

        free(interrupts_main);
    }
    if (interrupts_send != NULL) {

        free(interrupts_send);
    }


    return 0;
}