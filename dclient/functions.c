//
// Created by dushn on 23.7.20..
//

#include "functions.h"

#include <memory.h>
#include <stdlib.h>
#include <errno.h>

#include"sys/socket.h"
#include "main_header.h"



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

void data_transfer(int socket, Cpu_usage *cpu_usage1, Network *network, Memory_usage *memory_usage,
                   GArray *array_devices,
                    GArray *array_tasks
) {


    int flag = MSG_WAITALL;
    Task task;

    ssize_t ret;
    __int32_t num = 0;


    ///memory
    ret = recv(socket, memory_usage, sizeof(Memory_usage), flag);
    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        gtk_main_quit();
    }
    if (ret == 0) {

        printf("error receiving data\n %d", (int) ret);
        printf("socket closed\n");
        gtk_main_quit();
    }


    ret = test_recv(socket);
    if (ret < 0) {

        printf("error sending data\n %d", (int) ret);
        gtk_main_quit();
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        gtk_main_quit();
    }
    ///end of memory

    ///cpu_usage
    ret = recv(socket, cpu_usage1, sizeof(Cpu_usage), flag);
    if (ret < 0) {
        printf("Error receiving data!\n");
        gtk_main_quit();

    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        gtk_main_quit();
    }

    ret = test_recv(socket);
    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        gtk_main_quit();
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        gtk_main_quit();
    }



    ///end cpu_usage

    ///network
    ret = recv(socket, network, sizeof(Network), flag);
    if (ret < 0) {
        printf("Error receiving num_packets!\n\t");

        gtk_main_quit();

    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        gtk_main_quit();
    }

    ret = test_recv(socket);
    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        gtk_main_quit();
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        gtk_main_quit();
    }



    /// end network


    ///devices
    ret = (int) recv(socket, &num, sizeof(__int32_t), flag);
    if (ret < 0) {
        printf("Error sending num_packets!\n\t");
        gtk_main_quit();

    }
    ret = test_recv(socket);
    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        gtk_main_quit();
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        gtk_main_quit();
    }

    for (int i = 0; i < num; i++) {
        D_Collection *temp_d=calloc(1,sizeof(D_Collection));
        if(temp_d==NULL){

            printf("calloc error %d \n", errno);
            free(temp_d);
            gtk_main_quit();
        }


        ret = (int) recv(socket, &temp_d->devices, sizeof(Devices), flag);

        if (ret < 0) {
            printf("Error sending num_packets!\n\t");

            gtk_main_quit();

        }
        if (ret == 0) {

            printf("error sending data\n %d", (int) ret);
            printf("socket closed\n");
            gtk_main_quit();
        }
        temp_d->next=devices;
        devices=temp_d;


        g_array_append_val(array_devices, devices);

    }

    dev_num=num;
    ///end of devices

    /// tasks
    ret = (int) recv(socket, &num, sizeof(__int32_t), flag);
    if (ret < 0) {
        printf("Error sending num_packets!\n\t");

        gtk_main_quit();

    }

    ret = test_recv(socket);
    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        gtk_main_quit();
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        gtk_main_quit();
    }

    for (int i = 0; i < num; i++) {


        ret = recv(socket, &task, sizeof(Task), flag);
        if (ret < 0) {
            printf("Error sending num_packets!\n\t");

            gtk_main_quit();

        }
        if (ret == 0) {

            printf("error sending data\n %d", (int) ret);
            printf("socket closed\n");
            gtk_main_quit();
        }


        g_array_append_val(array_tasks, task);

    }


    /// interrupts
    Interrupts *temp=interrupts;
    for (int i = 0; i < 10; i++) {
      //  ret = recv(socket, &interrupts2, sizeof(Interrupts), flag);
        ret = recv(socket, interrupts, sizeof(Interrupts), flag);

        if (ret < 0) {
            printf("Error receving data!\n");

            gtk_main_quit();

        }
        if (ret == 0) {

            printf("error sending data\n %d", (int) ret);
            printf("socket closed\n");
            gtk_main_quit();
        }

      //  *interrupts=interrupts2;

        interrupts++;

    }
    interrupts=temp;
    ret = test_recv(socket);
    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        gtk_main_quit();
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        gtk_main_quit();
    }


    ///interrupts end


}

