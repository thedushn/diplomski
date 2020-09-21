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


#define BUFFER_SIZE2 64
#define BUFFER_SIZE 1024

static int number_bandwidth;

void * send_network(void *socket){
    int sockfd=(*(int*)socket);
    int result;
    Data data={0};
    Network network={0};
    ssize_t ret;

    result = interface_name(&network);
    if (result != 0) {

        pthread_exit(NULL);
    }
    memset(&data,0,sizeof(Data));
    data.size=NETWORK;
    data.unification.network=network;
    pthread_mutex_lock(&mutex_send);
    ret = send(sockfd, &data, sizeof(Data), 0);
    pthread_mutex_unlock(&mutex_send);

    if (ret < 0) {
        printf("Error sending data!\n\t");
        pthread_mutex_unlock(&mutex_send);
        pthread_exit(NULL);

    }
    if (ret == 0) {

        printf("socket closed\n");
        pthread_mutex_unlock(&mutex_send);
        pthread_exit(NULL);
    }
//    pthread_mutex_lock(&mutex_send);
//    if( test_send(sockfd)<=0){
//
//        pthread_mutex_unlock(&mutex_send);
//        pthread_exit(NULL);
//    }
//    pthread_mutex_unlock(&mutex_send);
    pthread_exit(NULL);
}

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


int get_rec_trans(char *name, __uint64_t received, __uint64_t *received_struct, __uint64_t transmitted,
                  __uint64_t *transmitted_struct) {
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

        *received_struct = 0;

    }
    if (net_data_old.transfered_data == 0) {


        *transmitted_struct = 0;

    } else {

        if (received < net_data_old.received_data || transmitted < net_data_old.transfered_data) {

            *received_struct = 0;
            *transmitted_struct = 0;
        } else {

            *received_struct = (received - net_data_old.received_data);
            *transmitted_struct = (transmitted - net_data_old.transfered_data);
        }


    }

    return 0;
}

int interface_name(Network *network1) {


    char *v = "/sys/class/net/";
    struct dirent *pDirent;
    DIR *pDir;

    __uint64_t network_rc = 0;
    __uint64_t network_ts = 0;
    char name[BUFFER_SIZE2];
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

            __uint64_t received_bytes = 0;
            unsigned long received_packets = 0;
            unsigned long received_errors = 0;
            unsigned long received_drop = 0;
            unsigned long received_fifo = 0;
            unsigned long received_frame = 0;
            unsigned long received_compressed = 0;
            unsigned long received_multicast = 0;
            __uint64_t transmit_bytes = 0;
            unsigned long transmit_packets = 0;
            unsigned long transmit_errors = 0;
            unsigned long transmit_drop = 0;
            unsigned long transmit_fifo = 0;
            unsigned long transmit_frame = 0;
            unsigned long transmit_compressed = 0;
            unsigned long transmit_multicast = 0;
            __uint64_t network_rc1 = 0;
            __uint64_t network_ts1 = 0;

            FILE *file;
            char buffer[1024];
            char buffer3[BUFFER_SIZE2];


            memset(buffer, 0, BUFFER_SIZE);
            memset(buffer3, 0, BUFFER_SIZE2);
            strncpy(buffer3, name, BUFFER_SIZE2);


            for (int g = 0; g < BUFFER_SIZE2; g++) {

                if (buffer3[g] == '\0')
                    break;
                else
                    number_bandwidth++;
            }


            char *filename = "/proc/net/dev";


            if ((file = fopen(filename, "r")) == NULL || fgets(buffer, BUFFER_SIZE, file) == NULL) {
                closedir(pDir);
                return 1;
            }


            while (fgets(buffer, 1024, file) != NULL) {


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


    network1->received_bytes = network_rc;
    network1->transmited_bytes = network_ts;



    number_bandwidth = 0;
    closedir(pDir);

    check_for_old_net();


    return 0;

}

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