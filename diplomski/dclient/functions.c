//
// Created by dushn on 23.7.20..
//

#include "functions.h"

#include <memory.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>

#include"sys/socket.h"
#include "main_header.h"
/**
 * function device_task_commands(): sends command to server about what type of devices it wants to see
 * input:pointer to signal and to task id
 * output:none.
 * */
void device_task_commands(char *signal, char *task_id) {

    int ret;
    char buffer[1500];

    memset(buffer, 0, sizeof(buffer));




    if (signal != NULL && task_id != NULL) {
        sprintf(buffer, "%d %s %s", device_all, signal, task_id);

    } else {
        sprintf(buffer, "%d", device_all);
    }

    ret = (int) send(newsockfd1, &buffer, sizeof(buffer), 0);
    if (ret < 0) {

        printf("command did not get sent \n");
        gtk_main_quit();


    }
    if (ret == 0) {

        printf("command did not get sent \n");
        printf("socket closed\n");
        gtk_main_quit();


    }

}
/**
 * function connection(): establishes a connection with the server
 * input: port number and IP address
 * output:return non zero value if something is wrong
 * */
int connection(char *argv1, char *argv2) {


    struct addrinfo hints, *servinfo, *p;
    int rv;
    int socketfd = 0;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv2, argv1, &hints, &servinfo)) != 0) {
        fprintf(stderr, "get_addr_info: %s\n", gai_strerror(rv));
        return -2;
    }

// loop through all the results and connect to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((socketfd = socket(p->ai_family, p->ai_socktype,
                               p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        if (connect(socketfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("connect");
            close(socketfd);
            continue;
        }

        break; // if we get here, we must have connected successfully
    }

    if (p == NULL) {
        // looped off the end of the list with no connection
        free(servinfo);
        fprintf(stderr, "failed to connect\n");
        return -2;

    }


    if (socketfd < 0) {
        printf("Error creating socket!\n");
        return -2;
    }


    free(servinfo);
    return socketfd;
}
/**
 * function input_command(): takes what we have typed in the entry widget and sends it to the server
 * input: none
 * output:none
 * */
void input_command() {


    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    command_sender((char *) text);

};
/**
 * function command_sender(): prepares a text command to be sent and sends it to server
 * input: none
 * output:return non zero value if something is wrong
 * */
int command_sender(char *text) {

    int ret;
    char buffer[1500];
    strcpy(buffer, "COMMAND");
    strncat(buffer, " ", sizeof(buffer));
    strncat(buffer, text, sizeof(buffer));
    ret = (int) send(newsockfd1, &buffer, sizeof(buffer), MSG_WAITALL);
    if (ret < 0) {

        printf("command did not get sent \n");
        if (gtk_main_level() > 0)
            gtk_main_quit();


    }
    if (ret == 0) {

        printf("command did not get sent \n");
        printf("socket closed\n");
        if (gtk_main_level() > 0)
            gtk_main_quit();


    }
    return ret;
}
/**
 * function test_send(): tests if the server can send TCP packets
 * input: socket
 * output:return non zero value if something is wrong
 * */
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
/**
 * function test_send(): tests if the client can send TCP packets
 * input: socket
 * output:return non zero value if something is wrong
 * */
ssize_t test_recv(int socket) {



    ssize_t ret =0;
    Data data={0};


    data.size=TEXT;

    memset(data.unification.conformation,0,sizeof(data.unification.conformation));
    strcpy(data.unification.conformation,"everything came");

    ret = send(socket, &data, sizeof(Data), MSG_WAITALL);


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
/**
 * function data_transfer(): receives TCP packets from the server and handles them depending on the type of file they are
 * input: socket,pointer cpu usage structure, pointer to network usage structure, double pointer to a Task doubly linked
 * list,double pointer to a Device doubly linked list, tasks number and devices number
 * output:return non zero value if something is wrong
 * */
int
data_transfer(int socket, Cpu_usage *cpu_usage, Network *network, Memory_usage *memory_usage, T_Collection **task_array,
              D_Collection **devices_array, __int32_t *task_num, __int32_t *dev_num) {


    int flag = MSG_WAITALL;
    ssize_t ret;


    T_Collection *temp_task_array=NULL;
    D_Collection *temp_device_array=NULL;


    Interrupts *temp=interrupts;
    while (1){
        Data data;
        memset(&data,0,sizeof(Data));

        ret = recv(socket, &data, sizeof(Data), flag);

        if (ret < 0) {

            printf("error receiving data\n %d", (int) ret);
            return (int)ret;
        }
        if (ret == 0) {

            printf("error receiving data\n %d ", (int) ret);
            printf("socket closed\n");
            return 1;
        }

        switch (data.size){

            case CPU_USAGE:

                *cpu_usage=(Cpu_usage)data.unification.cpu_usage;


                break;

            case NETWORK:

                *network=(Network)data.unification.network;
                break;

            case MEMORY:

                *memory_usage=(Memory_usage)data.unification.memory_usage;
                break;

            case TASK:
                temp_task_array =calloc(1,sizeof(T_Collection));
                if(temp_task_array==NULL){
                    free(temp_task_array);

                    printf("calloc error %d \n", errno);
                    return 1;
                }
                temp_task_array->task=(Task)(data.unification.task);
                temp_task_array->next=*task_array;
                *task_array=temp_task_array;

                (*task_num)++;
                break;

            case DEVICES:
                temp_device_array =calloc(1,sizeof(D_Collection));
                if(temp_device_array==NULL){
                    free(temp_device_array);

                    printf("calloc error %d \n", errno);
                    return 1;
                }
                temp_device_array->devices=(Devices)(data.unification.devices);
                temp_device_array->next=*devices_array;
                *devices_array=temp_device_array;

                (*dev_num)++;


                break;

            case INTERRUPTS:

                *temp=(Interrupts)(data.unification.interrupts);
                temp++;
                break;

            case TEXT:

                if (strcmp(data.unification.conformation, "everything came") != 0) {

                    printf("conformation didn't get received  \n");

                    return -1;
                }


                    return 0;

            default:
                return -1;


        }

    }





}
