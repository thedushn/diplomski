//
// Created by dushn on 23.7.20..
//

#include "network_bandwith.h"
#include "functions.h"
#include <errno.h>
#include <inttypes.h>
#include <memory.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/socket.h>



#define BUFFER_SIZE 1024

static int number_bandwidth;

/*
 * function send_network(); send data about network usage to client
 * input : socket to send data to
 * output : void
 * */
void * send_network(void *socket){

    int sockfd=(*(int*)socket);
    int result;
    Data data={0};
    Network network={0};
    ssize_t ret;

    pthread_mutex_lock(&mutex_send);
    while (thread_break == false) {
        ret = -100;
        pthread_mutex_unlock(&mutex_send);
        pthread_exit(&ret);
    }
    pthread_mutex_unlock(&mutex_send);
    result = interface_name(&network);
    if (result != 0) {
        ret = -100;
        pthread_exit(&ret);
    }
    memset(&data,0,sizeof(Data));
    data.size=NETWORK;
    data.unification.network=network;
    pthread_mutex_lock(&mutex_send);
    ret = send(sockfd, &data, sizeof(Data), 0);
    pthread_mutex_unlock(&mutex_send);

    if (ret < 0) {
        printf("Error sending data!\n\t");

        pthread_exit(&ret);

    }
    if (ret == 0) {

        printf("socket closed\n");
        ret = -100;
        pthread_exit(&ret);
    }

    pthread_exit(&ret);
}
/*
 * function search_net(); searches the linked list to find a network devices and  save its old data and replace it with the
 * new data
 * input :char pointer that contains the name of the network device bool  pointer that we change to true if the network devices
 * is already in the list,
 * structure of data about network usage that we want to save in the list
 *
 * output : returns a structure about the  network devices network usage
 * */


struct Net_data search_net(char *key, bool *ima, struct Net_data new_data) {

    struct Net_data temp_data={0};
    struct DataItem_net *temp_item=hash_network;
    for (int i = 0; i < net_hash_size; i++) {

        if ((strcmp(temp_item->name, key)) == 0) {
            temp_item->check=true;
            *ima = true;



            if ( new_data.received_data < temp_item->net_data.received_data
                    ||new_data.received_data < temp_item->net_data.transfered_data) {

                temp_item->net_data.received_data=new_data.received_data;
                temp_item->net_data.transfered_data=new_data.transfered_data;
                temp_item->check=true;

                return temp_data;
            }

            temp_data.transfered_data= temp_item->net_data.transfered_data;
            temp_data.received_data= temp_item->net_data.received_data;

            temp_item->net_data.received_data=new_data.received_data;
            temp_item->net_data.transfered_data=new_data.transfered_data;
            temp_item->check=true;


            return temp_data;

        }

        temp_item=temp_item->next;


    }


    return temp_data;
}

/*
 * function get_rec_trans(): calculates the transmitted and received bytes of a network device and inputs it in a link list
 *
 * input : name of the network device, received bytes, pointer to received bytes that we calculate,transmitted bytes,
 * pointer to transmitted bytes that we calculate
 * output : returns a non zero value if something goes wrong
 * */
int get_rec_trans(char *name, __uint64_t received, __uint64_t *received_calculated, __uint64_t transmitted,
                  __uint64_t *transmitted_calculated) {

    struct Net_data net_data_new;
    struct Net_data net_data_old;

    bool ima = false;
    struct DataItem_net *temp = NULL;

    net_data_new.transfered_data=transmitted;
    net_data_new.received_data=received;



    net_data_old=search_net(name, &ima, net_data_new);


    if (ima == false) {
        temp=(struct DataItem_net *) calloc(1, sizeof(struct DataItem_net));
        if (temp == NULL) {
            free(temp);
            printf("calloc error %d \n", errno);
            return -1;
        }

        temp->net_data.received_data=received;
        temp->net_data.transfered_data=transmitted;
        strcpy(temp->name,name);
        temp->check=true;

        net_hash_size++;

        temp->next=hash_network;
        hash_network=temp;



    }


    if (net_data_old.received_data == 0) {

        *received_calculated = 0;

    }
    if (net_data_old.transfered_data == 0) {


        *transmitted_calculated = 0;

    } else {

        if (received < net_data_old.received_data || transmitted < net_data_old.transfered_data) {

            *received_calculated = 0;
            *transmitted_calculated = 0;
        } else {

            *received_calculated = (received - net_data_old.received_data);
            *transmitted_calculated = (transmitted - net_data_old.transfered_data);
        }


    }

    return 0;
}

/*
 * function interface_name(): finds all the network devices and then calculates their network usage and adds them all up
 *
 * input : pointer to Network structure;
 * output : returns a non zero value if something goes wrong
 * */
int interface_name(Network *network1) {


    char *v = "/sys/class/net/";
    struct dirent *pDirent;
    DIR  *pDir;
    FILE *file;
    char *filename = "/proc/net/dev";
    __uint64_t network_rc = 0;
    __uint64_t network_ts = 0;
    char name[64];

    char buffer[BUFFER_SIZE];
    char buffer3[64];
    memset(name, 0, sizeof(name));

    pDir = opendir(v);

    if (pDir == NULL) {

        printf("Cannot open directory '%s'\n", v);
        return 1;

    }


    while ((pDirent = readdir(pDir)) != NULL) {

        if (!strcmp(pDirent->d_name, ".") || !strcmp(pDirent->d_name, "..") || !strcmp(pDirent->d_name, "lo")) {


        } else {

            sscanf(pDirent->d_name, "%s", name);

            __uint64_t    received_bytes = 0;
            unsigned long received_packets = 0;
            unsigned long received_errors = 0;
            unsigned long received_drop = 0;
            unsigned long received_fifo = 0;
            unsigned long received_frame = 0;
            unsigned long received_compressed = 0;
            unsigned long received_multicast = 0;
            __uint64_t    transmit_bytes = 0;
            unsigned long transmit_packets = 0;
            unsigned long transmit_errors = 0;
            unsigned long transmit_drop = 0;
            unsigned long transmit_fifo = 0;
            unsigned long transmit_frame = 0;
            unsigned long transmit_compressed = 0;
            unsigned long transmit_multicast = 0;
            __uint64_t network_rc1 = 0;
            __uint64_t network_ts1 = 0;




            memset(buffer, 0, BUFFER_SIZE);
            memset(buffer3, 0, 64);
            strncpy(buffer3, name, 64);


            for (int g = 0; g < 64; g++) {

                if (buffer3[g] == '\0')
                    break;
                else
                    number_bandwidth++;
            }





            if ((file = fopen(filename, "r")) == NULL || fgets(buffer, BUFFER_SIZE, file) == NULL) {
                closedir(pDir);
                return 1;
            }


            while (fgets(buffer, BUFFER_SIZE, file) != NULL) {


                char *temp = buffer;

                while (*temp == ' ') {
                    temp = temp + 1;
                }


                if (strncmp(temp, buffer3, (size_t) number_bandwidth) == 0) {

                    break;
                }


            }

            fclose(file);
            char *network_data = NULL;
            network_data = strchr(buffer, ':');

            network_data++;

            sscanf(network_data, " %" SCNu64 " %lu  %lu %lu %lu %lu %lu %lu %" SCNu64 " %lu %lu %lu %lu %lu %lu %lu",
                   &received_bytes,
                   &received_packets,
                   &received_errors, &received_drop, &received_fifo,
                   &received_frame, &received_compressed,
                   &received_multicast,
                   &transmit_bytes,
                   &transmit_packets,
                   &transmit_errors,
                   &transmit_drop,
                   &transmit_fifo,
                   &transmit_frame,
                   &transmit_compressed,
                   &transmit_multicast);


            network_rc1 = 0;
            network_ts1 = 0;

            if (get_rec_trans(name, received_bytes, &network_rc1, transmit_bytes, &network_ts1) != 0) {
                closedir(pDir);
                return -1;
            }


            network_rc += network_rc1;
            network_ts += network_ts1;

            number_bandwidth = 0;


        }


    }


    network1->received_bytes   = network_rc;
    network1->transmited_bytes = network_ts;



    number_bandwidth = 0;
    closedir(pDir);

    check_for_old_net();


    return 0;

}

/*
 * function check_for_old_net(): finds all the network devices that where not checked and removes them from the list
 *
 * input : none.
 * output : none.
 * */
void check_for_old_net(){
    struct DataItem_net *temp=hash_network;
    for(int i=0;i<net_hash_size;i++){

           if(! temp->check){
               struct DataItem_net *proxy=hash_network;
               for(int j=0;j<i-1;j++){
                   proxy=proxy->next;
               }
               proxy->next=proxy->next->next;
               free(temp);
               temp= proxy;
               net_hash_size--;
           }
        temp->check=false;
        temp=temp->next;
    }
}