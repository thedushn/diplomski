//
// Created by dushn on 23.7.20..
//

#include "devices.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

#define  BUFFER_SIZE 1024

void testing_files2(Devices *devices) {

    struct statvfs info;

    statvfs(devices->directory, &info);


    devices->fid = info.f_flag;
    devices->total = (__uint64_t) info.f_blocks * (__uint64_t) info.f_bsize;
    devices->used = (((__uint64_t) info.f_blocks - (__uint64_t) info.f_bfree)) * (__uint64_t) info.f_bsize;
    devices->avail = (__uint64_t) (info.f_bavail * info.f_bsize);
    devices->free = (__uint64_t) (info.f_bfree * info.f_bsize);


}


int device2(Devices **array, bool show, __int32_t *niz2) {

    Devices *devices = NULL;


    __int32_t niz = 0;

    int ret = mount_list3(&devices, show, &niz);

    if (ret == -1) {
        return ret;
    }

    *niz2 = niz;
    *array = devices;
    return ret;

}

int mount_list3(Devices **array, bool mount, __int32_t *number) {


    Devices *devices2 = NULL;
    Devices *temp;
    devices2 = calloc(1, sizeof(Devices));
    __int32_t niz = 0;
    char *filename = "/proc/mounts";

    char buffer[BUFFER_SIZE];
    FILE *file;


    file = fopen(filename, "r");
    if (file == NULL) {
        printf("open failed, errno = %d\n", errno);
        return -1;
    }
    fseek(file, 0, SEEK_SET);

    if (mount == true) {
        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {


            sscanf(buffer, "%63s %255s %63s", devices2[niz].name, devices2[niz].directory, devices2[niz].type);

            testing_files2(&devices2[niz]);
            niz++;
            temp = realloc(devices2, (niz + 1) * sizeof(Devices));

            if (temp != NULL) {
                devices2 = temp;
            } else {
                free(devices2);
                return -1;
            }

        }
    } else {
        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {

            sscanf(buffer, "%s %s %s", devices2[niz].name, devices2[niz].directory, devices2[niz].type);


            struct stat filestat;

            memset(&filestat, 0, sizeof(filestat));
            lstat(devices2[niz].name, &filestat);

            switch (filestat.st_mode & S_IFMT) {

                case S_IFBLK: {


                    testing_files2(&devices2[niz]);
                    niz++;
                    temp = realloc(devices2, (niz + 1) * sizeof(Devices));

                    if (temp != NULL) {
                        devices2 = temp;
                    } else {
                        free(devices2);
                        return -1;

                    }


                    break;

                }
                default:
                    memset(devices2[niz].name, 0, sizeof(devices2[niz].name));
                    memset(devices2[niz].directory, 0, sizeof(devices2[niz].directory));
                    memset(devices2[niz].type, 0, sizeof(devices2[niz].type));
                    break;
            }


        }
    }

    fclose(file);

    *array = devices2;
    *number = niz;


    return 0;
}