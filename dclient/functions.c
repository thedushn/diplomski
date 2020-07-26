//
// Created by dushn on 23.7.20..
//

#include "functions.h"
#include"stdio.h"

#include <memory.h>

#include"sys/socket.h"




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

void connection(int socket, Cpu_usage *cpu_usage1, Network *network, Memory_usage *memory_usage, GArray *array_devices,
                GArray *array_int, GArray *array_tasks
) {


    int flag = MSG_WAITALL;
    Task task;
    Devices devices;
    Interrupts interrupts;
    ssize_t ret;
    __int32_t num = 0;


    ///memorija
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
    ///kraj memorije

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



    ///kraj cpu_usage

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



    /// kraj networka


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


        ret = (int) recv(socket, &devices, sizeof(Devices), flag);

        if (ret < 0) {
            printf("Error sending num_packets!\n\t");

            gtk_main_quit();

        }
        if (ret == 0) {

            printf("error sending data\n %d", (int) ret);
            printf("socket closed\n");
            gtk_main_quit();
        }


        g_array_append_val(array_devices, devices);

    }
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

    for (int i = 0; i < 10; i++) {
        ret = recv(socket, &interrupts, sizeof(Interrupts), flag);

        if (ret < 0) {
            printf("Error receving data!\n");

            gtk_main_quit();

        }
        if (ret == 0) {

            printf("error sending data\n %d", (int) ret);
            printf("socket closed\n");
            gtk_main_quit();
        }


        g_array_append_val(array_int, interrupts);


    }
    ret = test_recv(socket);
    if (ret < 0) {

        printf("error receing data\n %d", (int) ret);
        gtk_main_quit();
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        gtk_main_quit();
    }


    ///intterupts end


}

