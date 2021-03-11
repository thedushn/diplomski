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
bool scan_numbers(__uint64_t *CPU, char *ptr, int *cpu_index){
    int  index=0;
    char num_buffer[64];
    memset(num_buffer,0,sizeof(num_buffer));

    {
        for(int i=(*cpu_index);i<cpu_num;i++) {
            while (*ptr == ' ') { //find the first number

                ptr++;
            }
            while (*ptr != ' ' && *ptr != '\n') {
                num_buffer[index++] = *ptr;
                ptr++;
            }
            if( *ptr == '\n' && index == 0){
                break;
            }
            index = 0;
            errno=0;
            CPU[i]= strtol(num_buffer, 0, 10 );
            if(errno !=0){
                test_strtol( CPU[i]) ;
            }

           // sscanf(num_buffer, " %" SCNu64 "", &CPU[i]);
            (*cpu_index)++;

            memset(num_buffer, 0, sizeof(num_buffer));





        }

        if((*cpu_index)==cpu_num){
            ((*cpu_index))=0;

            return true;
        }

    }

    return false;
}

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

}
/**
 * function command_sender(): prepares a text command to be sent and sends it to server
 * input: none
 * output:return non zero value if something is wrong
 * */
int command_sender(char *text) {

    int ret;
    char buffer[1500];
    strcpy(buffer, "COMMAND");
    strncat(buffer, " ", sizeof(buffer)-1);
    strncat(buffer, text, sizeof(buffer)-1);
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

    ssize_t     ret;
    Data       data;

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
}
/**
 * function test_send(): tests if the client can send TCP packets
 * input: socket
 * output:return non zero value if something is wrong
 * */
ssize_t test_recv(int socket) {



    ssize_t ret  ;
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
    char *p=NULL;
    int cpu_number=0;
    interrupt_num=0;
   // I_Collection *temp_i=NULL;
    I_Collection2 *temp_i2=NULL;
//    while(interrupts){
//        temp_i=interrupts;
//        interrupts=interrupts->next;
//        free(temp_i);
//        temp_i=NULL;
//
//    }
//    interrupts=NULL;
    while(interrupts2){
        temp_i2=interrupts2;
        if(temp_i2->interrupts.CPU){
            free(temp_i2->interrupts.CPU);
        }
        interrupts2=interrupts2->next;
        free(temp_i2);
        temp_i2=NULL;

    }
    interrupts2=NULL;

    T_Collection *temp_task_array   =NULL;
    D_Collection *temp_device_array =NULL;
   // I_Collection *temp_interrupts   =NULL;
    I_Collection2 *temp_interrupts2 =NULL;
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
                temp_interrupts2=calloc(1,sizeof(I_Collection2));
                if(temp_interrupts2==NULL){
                    free(temp_interrupts2);

                    printf("calloc error %d \n", errno);
                    return 1;
                }
                strcpy(temp_interrupts2->interrupts.name,data.unification.interrupts_send.name);
                strcpy(temp_interrupts2->interrupts.irq,data.unification.interrupts_send.irq);
                temp_interrupts2->interrupts.total=data.unification.interrupts_send.total;

                temp_interrupts2->next=interrupts2;
                interrupts2= temp_interrupts2;

                interrupts2->interrupts.CPU=calloc(cpu_num,sizeof(interrupts2->interrupts.CPU));
                if(interrupts2->interrupts.CPU==NULL){
                    free(interrupts2->interrupts.CPU);
                    free(temp_interrupts2);
                    printf("calloc error %d \n", errno);
                    return 1;
                }
                interrupt_num++;


//                temp_interrupts =calloc(1,sizeof(I_Collection));
//                if(temp_interrupts==NULL){
//                    free(temp_interrupts);
//
//                    printf("calloc error %d \n", errno);
//                    return 1;
//                }
//                temp_interrupts->interrupts=(Interrupts)(data.unification.interrupts);
//                temp_interrupts->next=interrupts;
//                interrupts=temp_interrupts;



                break;
            case CPU_PACK:
               p= data.unification.data_pack;
             //  printf("PACK %s\n",data.unification.data_pack);
                for(int i=0;i<cpu_num;i++){
                    int j=0;
                    while(*p==' '){
                        p++;
                    }
                    while(*p!=' ' && *p!='\n'){
                        cpu_usage->percentage[i][j++]=*p++;
                       // p++;

                    }
//                    strncpy(cpu_usage->percentage[i],p,5);
//                    p=p+5;
                  //  printf("%s ",cpu_usage->percentage[i]);
                }
              //  printf("\n");

                break;
            case INT_PACK:

                scan_numbers(interrupts2->interrupts.CPU,data.unification.data_pack,&cpu_number);

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
long receive_number_cpu(int socket) {



    ssize_t ret  ;
    Data data={0};
    long number;

    data.size=TEXT;

    memset(data.unification.conformation,0,sizeof(data.unification.conformation));


    ret = recv(socket, &data, sizeof(Data), MSG_WAITALL);


    if (ret < 0) {

        printf("error sending data\n %d", (int) ret);
        return ret;
    }
    if (ret == 0) {

        printf("error sending data\n %d", (int) ret);
        printf("socket closed\n");
        return ret;
    }
    number=(int)strtol(data.unification.conformation,NULL,10);
    if(errno!=0) {
        if ((errno == ERANGE && (number == LONG_MAX || number == LONG_MIN))
            || (errno != 0 && number == 0)) {
            perror("strtol");

           return -1;

        }
    }

    return number;

}