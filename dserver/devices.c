//
// Created by dushn on 23.7.20..
//

#include "devices.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <memory.h>
#include <sys/socket.h>

void * send_devices(void *socket){

    int sockfd=(*(int*)socket);
    int result;
    ssize_t ret;

    D_Collection *devices_c;
    D_Collection *temp_dev;
    __int32_t device_num = 0;

    Data data={0};

    result = mount_list(&devices_c, &device_num, devices_show);
    if (result != 0) {
        printf("error in mount_list\n");
        for(int k=0;k<device_num;k++){
            // save reference to first link
            temp_dev = devices_c;

            //mark next to first link as first
            devices_c = devices_c->next;

            //return the deleted link
            free(temp_dev);

        }
        pthread_exit(NULL);
    }


    temp_dev=devices_c;
    for (int i = 0; i < device_num; i++) {
        temp_dev->devices.checked=false;
        memset(&data,0,sizeof(Data));
        data.size=DEVICES;
        data.unification.devices=(Devices)temp_dev->devices;
        pthread_mutex_lock(&mutex_send);
        ret = send(sockfd, &data, sizeof(Data), 0);
        pthread_mutex_unlock(&mutex_send);



        if (ret < 0) {
            printf("Error sending data!\n\t");
            for(int k=0;k<device_num;k++){
                // save reference to first link
                temp_dev = devices_c;

                //mark next to first link as first
                devices_c = devices_c->next;

                //return the deleted link
                free(temp_dev);

            }
            pthread_exit(NULL);

        }
        if (ret == 0) {

            printf("socket closed\n");
            for(int k=0;k<device_num;k++){
                // save reference to first link
                temp_dev = devices_c;

                //mark next to first link as first
                devices_c = devices_c->next;

                //return the deleted link
                free(temp_dev);

            }
           pthread_exit(NULL);
        }
        temp_dev=temp_dev->next;


    }
    for(int k=0;k<device_num;k++){
        // save reference to first link
        temp_dev = devices_c;

        //mark next to first link as first
        devices_c = devices_c->next;

        //return the deleted link
        free(temp_dev);

    }


    pthread_exit(NULL);

}

void testing_files(Devices *devices) {

    struct statvfs info;

    statvfs(devices->directory, &info);


    devices->fid = info.f_flag;
    devices->total = (__uint64_t) info.f_blocks * (__uint64_t) info.f_bsize;
    devices->used = (((__uint64_t) info.f_blocks - (__uint64_t) info.f_bfree)) * (__uint64_t) info.f_bsize;
    devices->avail = (__uint64_t) (info.f_bavail * info.f_bsize);
    devices->free = (__uint64_t) (info.f_bfree * info.f_bsize);


}



int mount_list(D_Collection **array, __int32_t *dev_num, bool mount) {

    D_Collection *temp_dev;




    char *filename = "/proc/mounts";

    char buffer[BUFFER_SIZE];
    FILE *file;


    file = fopen(filename, "r");
    if (file == NULL) {
        printf("open failed, errno = %d\n", errno);
        printf("open failed,  %s\n", filename);
        return -1;
    }
    fseek(file, 0, SEEK_SET);

    if (mount == true) {
        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {

          temp_dev=  calloc(1, sizeof(D_Collection));
            if(temp_dev==NULL){
                free(temp_dev);

                printf("calloc error %d \n", errno);
                return 1;


            }

            sscanf(buffer, "%63s %255s %63s", temp_dev->devices.name,temp_dev->devices.directory,temp_dev->devices.type);

            testing_files(&temp_dev->devices);
            (*dev_num)++;
            temp_dev->next=*array;
            *array=temp_dev;


        }
    } else {
        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            Devices proxy={0};



            sscanf(buffer, "%s %s %s", proxy.name,proxy.directory,proxy.type);



            struct stat filestat={0};





            lstat(proxy.name, &filestat);

            switch (filestat.st_mode & S_IFMT) {

                case S_IFBLK: {


                    testing_files(&proxy);
                    temp_dev=  (D_Collection *)calloc(1, sizeof(D_Collection));
                    if(temp_dev==NULL){
                        free(temp_dev);

                        printf("calloc error %d \n", errno);
                        return 1;


                    }

                    (*dev_num)++;
                    temp_dev->devices=proxy;
                    temp_dev->next=*array;
                    *array=temp_dev;



                    break;

                }
                default:

                    break;
            }




        }
    }

    fclose(file);




    return 0;
}