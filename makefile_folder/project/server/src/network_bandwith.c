//
// Created by dushn on 23.7.20..
//

#include "network_bandwith.h"
#include <errno.h>
#include <inttypes.h>
#include <memory.h>
#include <stdlib.h>
#include <printf.h>
#include <dirent.h>
#include <stdio.h>


#define BUFFER_SIZE 1024
#define BUFFER_SIZE2 64
static struct DataItem_net *hash_network_rec = NULL;
static struct DataItem_net *hash_network_trans = NULL;
static int broj;


__uint64_t search_net(char *key, struct DataItem_net *hashArray, int hash_size, bool *ima, __uint64_t data
) {


    for (int hashIndex = 0; hashIndex < hash_size; hashIndex++) {

        if ((strcmp(hashArray[hashIndex].key, key)) == 0) {

            *ima = true;

            __uint64_t temp = hashArray[hashIndex].data;

            if (temp > data) {


                hashArray[hashIndex].data = data;
                return 0;
            }
            hashArray[hashIndex].data = data;

            return temp;

        }


    }


    return 0;
}


void get_rec_trans(char *name, __uint64_t received, __uint64_t *received_struct, __uint64_t transmitted,
                   __uint64_t *transmited_struct) {
    __uint64_t received_bytes_old = 0, transmit_bytes_old = 0;
    static int hash_size = 0;
    bool ima = false;
    struct DataItem_net *temp = NULL;
    struct DataItem_net *temp1 = NULL;

    if (hash_network_rec == NULL) {

        hash_network_rec = (struct DataItem_net *) calloc(1, sizeof(struct DataItem_net));
        hash_network_trans = (struct DataItem_net *) calloc(1, sizeof(struct DataItem_net));

        memset(hash_network_trans, 0, sizeof(struct DataItem_net));
        memset(hash_network_rec, 0, sizeof(struct DataItem_net));
        hash_size++;

    }

    received_bytes_old = search_net(name, hash_network_rec, hash_size, &ima, received);
    transmit_bytes_old = search_net(name, hash_network_trans, hash_size, &ima, transmitted);

    if (ima == false) {


        temp = realloc(hash_network_rec, (hash_size + 1) * sizeof(struct DataItem_net));
        temp1 = realloc(hash_network_trans, (hash_size + 1) * sizeof(struct DataItem_net));

        hash_size++;

        if (temp != NULL) {

            hash_network_rec = temp;


        } else {

            free(hash_network_rec);
            free(temp);
            printf("relloc error %d", errno);


        }

        if (temp1 != NULL) {
            hash_network_trans = temp1;


        } else {
            free(hash_network_trans);
            free(temp1);
            printf("relloc error %d", errno);
        }


        memset(&hash_network_rec[hash_size - 1], 0, sizeof(struct DataItem_net));
        memset(&hash_network_trans[hash_size - 1], 0, sizeof(struct DataItem_net));

        hash_network_rec[hash_size - 1].data = received;
        hash_network_trans[hash_size - 1].data = transmitted;

        for (int i = 0; i < sizeof(hash_network_rec->key); i++) {

            hash_network_rec[hash_size - 1].key[i] = name[i];
            hash_network_trans[hash_size - 1].key[i] = name[i];


        }


    }


    if (received_bytes_old == 0) {

        *received_struct = 0;

    }
    if (transmit_bytes_old == 0) {


        *transmited_struct = 0;

    } else {

        if (received < received_bytes_old || transmitted < transmit_bytes_old) {

            *received_struct = 0;
            *transmited_struct = 0;
        } else {

            *received_struct = (received - received_bytes_old);
            *transmited_struct = (transmitted - transmit_bytes_old);
        }


    }


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
            char buffer[BUFFER_SIZE];
            char buffer3[BUFFER_SIZE2];


            memset(buffer, 0, BUFFER_SIZE);
            memset(buffer3, 0, BUFFER_SIZE2);
            strncpy(buffer3, name, BUFFER_SIZE2);


            for (int g = 0; g < BUFFER_SIZE2; g++) {

                if (buffer3[g] == '\0')
                    break;
                else
                    broj++;
            }


            char *filename = "/proc/net/dev";


            if ((file = fopen(filename, "r")) == NULL || fgets(buffer, 1024, file) == NULL) {
                closedir(pDir);
                return 1;
            }


            while (fgets(buffer, 1024, file) != NULL) {


                char *temp = buffer;

                while (*temp == ' ') {
                    temp = temp + 1;
                }


                if (strncmp(temp, buffer3, (size_t) broj) == 0) {

                    break;
                }


            }


            char *network_data = NULL;
            network_data = strchr(buffer, ':');

            network_data = network_data + 1;

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

            get_rec_trans(name, received_bytes, &network_rc1, transmit_bytes, &network_ts1);


            network_rc += network_rc1;
            network_ts += network_ts1;

            broj = 0;
            fclose(file);


        }


    }


    network1->received_bytes = network_rc;
    network1->transmited_bytes = network_ts;

    broj = 0;
    closedir(pDir);

    return 0;

}

void clean() {

    if (hash_network_trans != NULL) {
        free(hash_network_trans);
    }

    if (hash_network_rec != NULL) {
        free(hash_network_rec);
    }

}