//
// Created by dushn on 23.7.20..
//

#include "functions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include"sys/socket.h"
#include"pthread.h"


#include "interrupts.h"
#include "cpu_usage.h"
#include "memory_usage.h"
#include "task_manager.h"
#include "devices.h"
#include "network_bandwith.h"






ssize_t test_send(int socket) {

    ssize_t ret = 0;
    Data data;
   memset(&data,0,sizeof(Data));
    ret = recv(socket, &data, sizeof(Data), 0);

    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        return ret;
    }
    if (ret == 0) {

        printf("socket closed\n");
        return ret;
    }
    if (ret < sizeof(Data)) {
        size_t velicina = sizeof(Data);
        velicina -= ret;
        while (velicina > 0 || velicina < 0) {


            ret = (int) recv(socket, &data, velicina, 0);
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
    if (strcmp(data.unification.conformation, "everything came") != 0) {

        printf("conformation didn't get received  \n");

        return -1;
    }

    return sizeof(Data);
};

ssize_t test_recv(int socket) {


    char buffer[64];
    ssize_t ret ;
    memset(buffer, 0, 64);
    strcpy(buffer, "everything came");
    Data data;
    data.size=TEXT;
    memset(data.unification.conformation,0,sizeof(data.unification.conformation));
    strcpy(data.unification.conformation,"everything came");
    ret = send(socket, &data, sizeof(Data), 0);
  //  ret = send(socket, buffer, 64, 0);

    if (ret < 0) {

        printf("error sending data\n %d", (int) ret);
        return ret;
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        return ret;
    }


    return ret;

}

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

    Commands commands;
    char buffer[1600];
    char text[256];
    char *text1;


    while (1) {
        memset(buffer,0,sizeof(buffer));
        memset(&commands,0,sizeof(commands));
        memset(&text,0,sizeof(text));
        memset(&text1,0,sizeof(text1));
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
     //   strncpy(text,buffer,7);
        sscanf(buffer, "%s %s %s",text,commands.command,commands.task_id);
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
                strcat(text1, " &");
            printf("text1 %s\n",text1);
            if (system(text1) != 0)
                printf("command failed\n");
        }







    }

};


void *sending(void *socket) {

    ssize_t ret = 0;
    time_t time1;


    devices_show=false;

    int sockfd = *(int *) socket;


    while (1) {





      //  pthread_t  threads[6];

        time1 = time(NULL);

        local_time = *localtime(&time1);
        ///memory
        send_memory(socket);


        ///cpu
        send_cpu(socket);



        ///network
         send_network(socket);


        ///devices
        send_devices(socket);




        /// tasks
        send_task(socket);





        ///interrupts
        send_interrupts(socket);





        ret = test_recv(sockfd);
        if (ret < 0) {

            printf("error receiving data\n %d", (int) ret);
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


    }





   clean_interrupts();




    return 0;
}