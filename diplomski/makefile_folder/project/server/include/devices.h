//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_DEVICES_H
#define DIPLOMSKI_DEVICES_H

#include "common.h"

bool devices_show ; /*bool used to check if client wants Block devices or all of the mounted devices*/


void input_device_stats(Devices *devices);

void * send_devices(void *socket);

int mount_list(D_Collection **array, __int32_t *dev_num, bool mount);

#endif //DIPLOMSKI_DEVICES_H
