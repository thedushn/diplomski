//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_NETWORK_BANDWITH_H
#define DIPLOMSKI_NETWORK_BANDWITH_H

#include "common.h"
struct Net_data{

    __uint64_t received_data;
    __uint64_t transfered_data;
};
struct DataItem_net {

    struct Net_data net_data;
    char name[64];
    bool check;
    struct DataItem_net *next;

};

 struct DataItem_net *hash_network ;

int net_hash_size;
int interface_name(Network *network);

void get_rec_trans(char *name, __uint64_t received, __uint64_t *received_struct, __uint64_t transmitted,
                   __uint64_t *transmitted_struct
);



struct Net_data search_net(char *key, bool *ima, struct Net_data new_data);

void check_for_old_net();
void * send_network(void *socket);
#endif //DIPLOMSKI_NETWORK_BANDWITH_H
