//
// Created by dushn on 23.7.20..
//

#include "functions.h"

#include <memory.h>
#include <stdlib.h>
#include <errno.h>

#include"sys/socket.h"
#include "main_header.h"

void input_command() {


    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    g_print("Command %s  \n", text);
    command_sender((char *) text);

};

int command_sender(char *text) {
    int ret;
    char buffer[1500];
    strcpy(buffer, "COMMAND");
    strncat(buffer, " ", sizeof(buffer));
    strncat(buffer, text, sizeof(buffer));
    ret = (int) send(newsockfd1, &buffer, sizeof(buffer), 0);
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

int
data_transfer(int socket, Cpu_usage_list **cpu_usage_list, Network *network, Memory_usage *memory_usage,
              T_Collection **task_array, D_Collection **devices_array, __int32_t *task_num, __int32_t *dev_num) {


    int flag = MSG_WAITALL;
    ssize_t ret;


    T_Collection *temp_task_array=NULL;
    D_Collection *temp_device_array=NULL;
    Cpu_usage_list *temp_cpu_array=NULL;
    Cpu_usage_list *temp_cpu_p=NULL;

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

            printf("error receiving data\n %d", (int) ret);
            printf("socket closed\n");
            return 1;
        }

        switch (data.size){

            case CPU_USAGE:

                if(*cpu_usage_list!=NULL) {


                    temp_cpu_array = *cpu_usage_list;

                    while (temp_cpu_array->next != NULL) {
                        temp_cpu_array = temp_cpu_array->next;
                    }
                }
                temp_cpu_p =calloc(1,sizeof(Cpu_usage_list));
                if(temp_cpu_p==NULL){
                    free(temp_cpu_p);

                    printf("calloc error %d \n", errno);
                    return 1;
                }
                temp_cpu_p->cpu_usage=(Cpu_usage)data.unification.cpu_usage;

                if(*cpu_usage_list==NULL){

                    *cpu_usage_list=temp_cpu_p;
                }
                else {
                    temp_cpu_array->next=temp_cpu_p;
                }


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

                    return 0;

            default:
                return -1;


        }

    }





}
