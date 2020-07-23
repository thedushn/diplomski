//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_NETWORK_BANDWITH_H
#define DIPLOMSKI_NETWORK_BANDWITH_H

#include "common.h"

struct DataItem_net {

    __uint64_t data;
    char key[64];


};

int interface_name(Network *network);

void get_rec_trans(char *name, __uint64_t received, __uint64_t *received_struct, __uint64_t transmitted,
                   __uint64_t *transmited_struct
);

void clean();

__uint64_t search_net(char *key, struct DataItem_net *hashArray, int hash_size, bool *ima, __uint64_t data);


#endif //DIPLOMSKI_NETWORK_BANDWITH_H
