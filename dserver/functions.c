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





/*
 * function test_send(): receives a packet and checks if its match a string
 * input : socket
 * output : returns a value equal or lower then zero if something is not correct else it return a value higher then 0
 * */

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


/*
 * function test_recv():  sends a conformation packet that has a specific string in it
 * input : socket
 * output : returns a value equal or lower then zero if something is not correct else it return a value higher then 0
 * */
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
/*
 * function send_prio_to_task():  sets tasks priority, they can span from -15 to 15
 * input : pointer to a string that contains the name of the task and a pointer to a string that contains the type of
 * priority we want to set
 * output : none.
 * */
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
/*
 * function send_signal_to_task():  sends signal to task, they can be KILL,STOP,CONTINUE,TERMINATE
 * input : pointer to a string that contains the name of the task and a pointer to a string that contains the type of
 * signal we want to send
 * output : none.
 * */
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
/*
 * function accept_command():  accepts commands from client
 * input : pointer to socket
 * output : if the socket gets closed or it breaks SIGPIPE the function exits with the value of ret
 * */
void *accept_command(void *socket) {

    int         sockfd = *(int *) socket;

    Commands    commands;
    char        buffer[1500];
    char        text[256];
    char        *text1;
    ssize_t     ret;
    int         g=0;

    while (1) {

        memset(buffer,0,sizeof(buffer));
        memset(&commands,0,sizeof(commands));
        memset(&text,0,sizeof(text));
        memset(&text1,0,sizeof(text1));

        ret = recv(sockfd, &buffer, sizeof(buffer), 0);
        if (ret < 0) {
            printf("error condition didn't get received\n");
            pthread_exit(&ret);
        }
        if (ret == 0) {
            printf("error condition did not get received\n");
            printf("ret %d\n", (int) ret);
            ret=-100;
            pthread_exit(&ret);
        }


        sscanf(buffer, "%s %s %s",text,commands.command,commands.task_id);
        if(strcmp(text,"COMMAND")!=0){

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

            if (system(text1) != 0){
                printf("command failed\n");
            }

        }

    }

};
/*
 * function sending():  gets request from client to send it data from the proc file system,it runs multiple threads to
 * accomplish this task, after it has sent all the data it sends a conformation message back to the client that all the
 * data has been sent
 * input : pointer to socket
 * output : if there is any error in the execution of the code the function waits for all of the threads to notice
 * that there is an error and lets them finish their execution,after it frees the memory and exits.
 * */

void *sending(void *socket) {

    ssize_t ret = 0;
    time_t time1;


    devices_show=false;/*bool that tells the server only to send Block devices*/


    thread_break = true;

    pthread_t  thr[6];
    ssize_t thread_ret[6]={0};
    pthread_attr_t attr;
    pthread_mutex_init(&mutex_jiff,NULL);
    pthread_mutex_init(&mutex_send,NULL);

    pthread_cond_init(&cpu_cond,NULL);
    int return_value;
    char buffer[128];




    while (1) {

        time1 = time(NULL);

        local_time = *localtime(&time1);

        test_send((*(int *) socket)); /*wait for client to ask for data*/

       pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);


        return_value=pthread_create(&thr[0], &attr, send_memory,socket);

        if (return_value != 0) {/*if the server fails to create the thread we tell the other threads to exit*/

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);

            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);


        }

        return_value=pthread_create(&thr[1], &attr, send_cpu,socket);

        if (return_value != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);


        }

        return_value=pthread_create(&thr[2], &attr, send_network,socket);

        if (return_value != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);


        }

        return_value=pthread_create(&thr[3], &attr, send_interrupts,socket);

        if (return_value != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);


        }

        return_value=pthread_create(&thr[4], &attr, send_devices,socket);

        if (return_value != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);

        }

        return_value=pthread_create(&thr[5], &attr, send_task,socket);

        if (return_value != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);


        }

        pthread_attr_destroy(&attr);
        for(int i=0;i<6;i++){
            void *status=NULL;

           if((return_value= pthread_join(thr[i], &status))){/*checking if threads had problems gathering data*/

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
            if (thread_ret[i] < 0) {/*if a thread encountered a problem*/
                pthread_mutex_lock(&mutex_send);
                thread_break = false;
                pthread_mutex_unlock(&mutex_send);
            }

        }


        if (thread_break == false) {

            ret=-100;
            break;

        }

        ret = test_recv((*(int *) socket));/*telling the client that we sent all the data*/
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

    pthread_exit(&ret);
}