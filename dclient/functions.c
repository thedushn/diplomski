//
// Created by dushn on 23.7.20..
//

#include "functions.h"

#include <memory.h>
#include <stdlib.h>
#include <errno.h>

#include"sys/socket.h"
#include "main_header.h"

void printerino() {

    printf("we are in you pressed enter\n");
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    g_print("Command %s  \n", text);
    command_sender((char *) text);

};

int command_sender(char *text) {
    int ret;
    char buffer[1600];
    strcpy(buffer, "COMMAND");
    strncat(buffer, " ", sizeof(buffer));
    strncat(buffer, text, sizeof(buffer));
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
    return ret;
}

ssize_t test_recv(int socket) {


    char buffer[64];
    ssize_t ret ;
    memset(buffer, 0, 64);
    strcpy(buffer, "stiglo sve");
    ret = send(socket, buffer, 64, 0);

    if (ret < 0) {

        printf("error sending data\n %d", (int) ret);
        return ret;
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        return ret;
    }


    return 64;

}

int
data_transfer(int socket, Cpu_usage *cpu_usage1, Network *network, Memory_usage *memory_usage, Task **array_tasks,
              Devices **array_devices) {


    int flag = MSG_WAITALL;
    Task *task_a;
    Devices *dev_a;

    ssize_t ret;
    __int32_t num = 0;


    ///memory
    ret = recv(socket, memory_usage, sizeof(Memory_usage), flag);
    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        return (int)ret;
    }
    if (ret == 0) {

        printf("error receiving data\n %d", (int) ret);
        printf("socket closed\n");
        return 1;
    }


    ret = test_recv(socket);
    if (ret < 0) {

        printf("error sending data\n %d", (int) ret);
        return (int)ret;
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        return 1;
    }
    ///end of memory

    ///cpu_usage
    ret = recv(socket, cpu_usage1, sizeof(Cpu_usage), flag);
    if (ret < 0) {
        printf("Error receiving data!\n");
        return (int)ret;

    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        return 1;
    }

    ret = test_recv(socket);
    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        return (int)ret;
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        return 1;
    }



    ///end cpu_usage

    ///network
    ret = recv(socket, network, sizeof(Network), flag);
    if (ret < 0) {
        printf("Error receiving num_packets!\n\t");

        return (int)ret;

    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        return 1;
    }

    ret = test_recv(socket);
    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        return (int)ret;
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        return 1;
    }



    /// end network


    ///devices
    ret = (int) recv(socket, &num, sizeof(__int32_t), flag);
    if (ret < 0) {
        printf("Error sending num_packets!\n\t");
        return (int)ret;

    }
    ret = test_recv(socket);
    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        return (int)ret;
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        return 1;
    }

    dev_a=calloc((size_t)num, sizeof(Devices));
    if(dev_a==NULL){
        free(dev_a);

        printf("calloc error %d \n", errno);
        return 1;


    }


    Devices *temp_dev=dev_a;
    for (int i = 0; i < num; i++) {

        ret = (int) recv(socket, temp_dev, sizeof(Devices), flag);

        if (ret < 0) {
            printf("Error sending num_packets!\n\t");

            return (int) ret;

        }
        if (ret == 0) {

            printf("error sending data\n %d", (int) ret);
            printf("socket closed\n");
            return 1;
        }
        temp_dev++;


    }
    *array_devices=dev_a;
    dev_num=num;



    ///end of devices

    /// tasks
    ret = (int) recv(socket, &num, sizeof(__int32_t), flag);
    if (ret < 0) {
        printf("Error sending num_packets!\n\t");

        return (int)ret;

    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        return 1;
    }

    ret = test_recv(socket);
    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        return (int)ret;
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        return 1;
    }
    task_a=calloc((size_t)num, sizeof(Task));
    if(task_a==NULL){
        free(task_a);

        printf("calloc error %d \n", errno);
        return 1;


    }
    Task *temp_task=task_a;
    for (int i = 0; i < num; i++) {


        ret = recv(socket, task_a, sizeof(Task), flag);
        if (ret < 0) {
            printf("Error sending num_packets!\n\t");

            return (int)ret;

        }
        if (ret == 0) {

            printf("error sending data\n %d", (int) ret);
            printf("socket closed\n");
            return 1;
        }

        task_a++;


    }
    task_a=temp_task;
    *array_tasks=task_a;
    task_num=num;


    /// interrupts
    Interrupts *temp=interrupts;
    for (int i = 0; i < 10; i++) {

        ret = recv(socket, interrupts, sizeof(Interrupts), flag);

        if (ret < 0) {
            printf("Error receving data!\n");

            return (int)ret;

        }
        if (ret == 0) {

            printf("error sending data\n %d", (int) ret);
            printf("socket closed\n");
            return 1;
        }

      //  *interrupts=interrupts2;

        interrupts++;

    }
    interrupts=temp;
    ret = test_recv(socket);
    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        return (int)ret;
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        return 1;
    }


    ///interrupts end

    return 0;
}

