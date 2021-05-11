//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_NETWORK_BANDWITH_H
#define DIPLOMSKI_NETWORK_BANDWITH_H

#include "common.h"

struct DataItem_net { /*structure for saving network devices sent and received bytes*/

    Network             net_data;
    char                name[64];
    bool                check;
    struct DataItem_net *next;

};

struct DataItem_net *hash_network; /*poiter to link list of network devices*/

int net_hash_size; /*number of network devices*/

int interface_name(Network *network);

int get_rec_trans(char *name, __uint64_t received, __uint64_t *received_calculated, __uint64_t transmitted,
                  __uint64_t *transmitted_calculated
);



Network search_net(char *key, bool *ima, Network new_data);

void check_for_old_net();

void * send_network(void *socket);
#endif //DIPLOMSKI_NETWORK_BANDWITH_H
