//
// Created by dushn on 23.7.20..
//

#include "functions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include"sys/socket.h"


#include "interrupts.h"
#include "cpu_usage.h"
#include "memory_usage.h"
#include "task_manager.h"
#include "devices.h"
#include "network_bandwith.h"







ssize_t test_send(int socket) {

    ssize_t ret = 0;
    Data data;

    memset(&data, 0, sizeof(Data));

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
        size_t length = sizeof(Data);
        length -= ret;
        while (length > 0 || length < 0) {


            ret = (int) recv(socket, &data, length, 0);
            length -= ret;

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



    ssize_t ret ;

    Data data={0};
    data.size=TEXT;
    memset(data.unification.conformation,0,sizeof(data.unification.conformation));
    strcpy(data.unification.conformation,"everything came");

    ret = send(socket, &data, sizeof(Data), 0);


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
    char buffer[1500];
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

            if (system(text1) != 0)
                printf("command failed\n");
        }







    }

};


void *sending(void *socket) {

    ssize_t ret = 0;
    time_t time1;


    devices_show=false;


    thread_break = true;

    pthread_t  thr[6];
    ssize_t thread_ret[6]={0,1,2,3,4,5};
    pthread_attr_t attr;
    pthread_mutex_init(&mutex_jiff,NULL);
    pthread_mutex_init(&mutex_send,NULL);

    pthread_cond_init(&cpu_cond,NULL);
    int return_value;
    char buffer[128];




    while (1) {

        time1 = time(NULL);

        local_time = *localtime(&time1);

        test_send((*(int *) socket));

       pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);


        return_value=pthread_create(&thr[0], &attr, send_memory,socket);

        if (return_value != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);

            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);
            //   break;

        }

        return_value=pthread_create(&thr[1], &attr, send_cpu,socket);

        if (return_value != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);
            //   break;

        }

        return_value=pthread_create(&thr[2], &attr, send_network,socket);

        if (return_value != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);
            //   break;

        }

        return_value=pthread_create(&thr[3], &attr, send_interrupts,socket);

        if (return_value != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);
            //   break;

        }

        return_value=pthread_create(&thr[4], &attr, send_devices,socket);

        if (return_value != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);
            //   break;
        }

        return_value=pthread_create(&thr[5], &attr, send_task,socket);

        if (return_value != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);
            //   break;

        }

        pthread_attr_destroy(&attr);
        for(int i=0;i<6;i++){
            void *status=NULL;

           if((return_value= pthread_join(thr[i], &status))){

               strerror_r(return_value,buffer,sizeof(buffer));
               fprintf(stderr,"error = %d (%s)\n",return_value,buffer);
               pthread_mutex_lock(&mutex_send);
               thread_break = false;
               pthread_mutex_unlock(&mutex_send);

           }
            if(status==NULL){
                thread_ret[i]=0;
            }
            else
            {
                thread_ret[i]=(*(ssize_t *)status);
            }
            if (thread_ret[i] < 0) {
                pthread_mutex_lock(&mutex_send);
                thread_break = false;
                pthread_mutex_unlock(&mutex_send);
            }

        }


        if (thread_break == false) {

            break;
        }

        ret = test_recv((*(int *) socket));
        if (ret < 0) {

            printf("error receiving data\n %d", (int) ret);
            break;
        }
        if (ret == 0) {

            printf("socket closed\n");
            break;
        }




    }


    pthread_mutex_destroy(&mutex_send);
    pthread_mutex_destroy(&mutex_jiff);




   clean_interrupts();




    pthread_exit(NULL);
}