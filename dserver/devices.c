//
// Created by dushn on 23.7.20..
//

#include "devices.h"
#include "functions.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <memory.h>
#include <sys/socket.h>

/*
 * function send_devices(): send data about the devices  to client
 * input  : socket to send data to
 * output : returns a value less then zero if something did not work
 * */
void * send_devices(void *socket){

   // int     sockfd=(*(int*)socket);
    //ssize_t *ret=NULL;
    Thread_pack thr_p=*(Thread_pack*)socket;
    D_Collection *devices_c=NULL;
    D_Collection *temp_dev;
    __int32_t device_num = 0;


    Data data={0};
    pthread_mutex_lock(&mutex_send);
    while (thread_break == false) { /*if other threads have failed close this thread before it allocates any memory*/

        pthread_mutex_unlock(&mutex_send);
        printf("inside the thread_break %s\n",__func__);
        fflush(stdout);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&mutex_send);





    if ( (thr_p.ret_val=mount_list(&devices_c, &device_num, devices_show))) {
        printf("error in mount_list\n");
        for(int k=0;k<device_num;k++){

            temp_dev  = devices_c;
            devices_c = devices_c->next;
            free(temp_dev);

        }
        thr_p.ret_val = -1;
        pthread_exit(&thr_p.ret_val);
    }


    temp_dev=devices_c;
    for (int i = 0; i < device_num; i++) {/*send all the data about devices to client*/
        temp_dev->devices.checked=false;
        memset(&data,0,sizeof(Data));
        data.size=DEVICES;
        data.unification.devices=(Devices)temp_dev->devices;
        pthread_mutex_lock(&mutex_send);
        thr_p.ret_val = send(thr_p.socket, &data, sizeof(Data), 0);
        pthread_mutex_unlock(&mutex_send);



        if (thr_p.ret_val< 0) { /*if the socket broke SIGPIPE error free allocated memory*/

            printf("Error sending data\n return = %d\n", (int)thr_p.ret_val);
            for(int k=0;k<device_num;k++){

                temp_dev = devices_c;
                devices_c = devices_c->next;
                free(temp_dev);

            }
            pthread_exit(&thr_p.ret_val);

        }
        if (thr_p.ret_val == 0) {
            printf("Error sending data\n return = %d\n", (int) thr_p.ret_val);
            printf("socket closed\n");
            for(int k=0;k<device_num;k++){

                temp_dev = devices_c;
                devices_c = devices_c->next;
                free(temp_dev);

            }
            thr_p.ret_val = -1;
            pthread_exit(&thr_p.ret_val);
        }


        temp_dev=temp_dev->next;


    }
    for(int k=0;k<device_num;k++){ /*done with device list time to free memory*/

        temp_dev = devices_c;


        devices_c = devices_c->next;


        free(temp_dev);

    }


    pthread_exit(&thr_p.ret_val);

}
/*
 * function input_device_stats(): uses the function statvfs to get stats about  mounted device
 * input : pointer to Devices structure;
 * output : none.
 * */
void input_device_stats(Devices *devices) {

    struct statvfs info;

    statvfs(devices->directory, &info);


    devices->fid    = info.f_flag;
    devices->total  = (__uint64_t) info.f_blocks * (__uint64_t) info.f_bsize;
    devices->used   = (((__uint64_t) info.f_blocks - (__uint64_t) info.f_bfree)) * (__uint64_t) info.f_bsize;
    devices->avail  = (__uint64_t) (info.f_bavail * info.f_bsize);
    devices->free   = (__uint64_t) (info.f_bfree * info.f_bsize);


}

/*
 * function mount_list(): creates a list of mounted devices based on their type
 * input : Double pointer to a structure that contains stats about a device, pointer to the number of devices
 * and a bool that specifies which type of devices we want in our list
 * output : returns a non zero value if something goes wrong
 * */

int mount_list(D_Collection **array, __int32_t *dev_num, bool mount) {

    D_Collection *temp_dev;




    char *filename = "/proc/mounts";

    char buffer[1024];
    FILE *file;


    file = fopen(filename, "r");
    if (file == NULL) {
        printf("open failed, errno = %d\n", errno);
        printf("open failed,  %s\n", filename);
        return -1;
    }
    fseek(file, 0, SEEK_SET);

    if (mount == true) {/*checking if the client wants all the mounted files */
        while (fgets(buffer, 1024, file) != NULL) {

          temp_dev=  calloc(1, sizeof(D_Collection));
            if(temp_dev==NULL){
                free(temp_dev);

                printf("calloc error %d \n", errno);
                return -1;


            }

            sscanf(buffer, "%63s %255s %63s", temp_dev->devices.name,temp_dev->devices.directory,temp_dev->devices.type);

            input_device_stats(&temp_dev->devices);
            (*dev_num)++;
            temp_dev->next=*array;
            *array=temp_dev;


        }
    } else {
        while (fgets(buffer, 1024, file) != NULL) { /*if the client only wants Block devices */
            Devices proxy={0};



            sscanf(buffer, "%s %s %s", proxy.name,proxy.directory,proxy.type);

            struct stat filestat={0};


            lstat(proxy.name, &filestat);

            switch (filestat.st_mode & S_IFMT) {

                case S_IFBLK: {  /*checking to see if its a block device*/


                    input_device_stats(&proxy);
                    temp_dev=  (D_Collection *)calloc(1, sizeof(D_Collection));
                    if(temp_dev==NULL){
                        free(temp_dev);

                        printf("calloc error %d \n", errno);
                        return -1;


                    }
                    /*adding the the device to the list*/
                    (*dev_num)++;
                    temp_dev->devices=proxy;
                    temp_dev->next=*array;
                    *array=temp_dev;

                    break;

                }
                default: /*if its not a Block device we skip it*/

                    break;
            }

        }
    }

    fclose(file);


    return 0;
}