//
// Created by dushn on 23.7.20..
//



#include"stdio.h"
#include"stdlib.h"


#include"string.h"

#include"pthread.h"

#include "functions.h"
#include "cpu_usage.h"

#include "network_bandwith.h"
#include "time_managment.h"



#define BUFFER_SIZE 1024



/*
 * function main(): creates a TPC socket and waits for a connection of the client,if that was successful it gathers
 * information about when the kernel started working and creates threads to send information to the client and receive
 * commands from him, after the client wants to stop getting information about the servers host, the server closes the
 * sockets and free the allocated memory from the gathering of the data
 * input  : port number
 * output : returns a non zero value if something goes wrong
 * */

int main(int argc, char *argv[]) {

    hash_size     = 0;
    task_details  = NULL;
    hash_network  = NULL;
    net_hash_size = 0;
    cpu_Number=0;


    printf("%ld\n",sysconf(_SC_NPROCESSORS_ONLN)) ;





    int socket_send     = 0;
    int socket_command  = 0;  // listen on sock_fd, new connection on socket_send
    int ret                ;

    char buffer[BUFFER_SIZE];


    struct DataItem     *temp;
    struct DataItem_net *temp_net;

    if (argc < 2) {

        printf("no port provided");
        return -1;
    }



   // cpu_Number=500;
    cpu_Number=cpu_number();




    connection(argv[1],&socket_send,&socket_command);

    cpu_Number=cpu_number();
    send_cpu_num(socket_send,cpu_Number);

    if(cpu_data_allocation()){
        close(socket_send);
        close(socket_command);
        return -1;
    }




   if(uptime()){
       close(socket_send);
       close(socket_command);
       free_cpu_memory();
       return -1;
   }
    thr_p_main[0].socket=socket_send;

    pthread_mutex_init(&mutex_send,NULL);


    thread_break = true;
    if ((ret=pthread_create(&thr_p_main[0].thread_id, NULL, &sending, &thr_p_main[0])) != 0) {/*creating thread for sending data to client*/

        printf("ERROR: Return Code from pthread_create() is %d\n", ret);

        close(socket_command);
        close(socket_send);
        free_cpu_memory();
        return -1;

    }
    /*creating thread for receiving commands from client*/

    if ((ret = pthread_create(&thr_p_main[1].thread_id, NULL, accept_command, &socket_command)) != 0) {

        printf("ERROR: Return Code from pthread_create() is %d\n", ret);

        close(socket_command);
        close(socket_send);
        free_cpu_memory();
        return -1;

    }





    if((ret= pthread_join(thr_p_main[0].thread_id, NULL))){

        strerror_r(ret,buffer,sizeof(buffer));
        fprintf(stderr,"error = %d (%s)\n",ret,buffer);
        pthread_mutex_lock(&mutex_send);
        thread_break = false;
        pthread_mutex_unlock(&mutex_send);

    }


    if((ret= pthread_join(thr_p_main[1].thread_id, NULL ))){

        strerror_r(ret,buffer,sizeof(buffer));
        fprintf(stderr,"error = %d (%s)\n",ret,buffer);
        pthread_mutex_lock(&mutex_send);
        thread_break = false;
        pthread_mutex_unlock(&mutex_send);
    }

    pthread_mutex_destroy(&mutex_send);




//    for(int k=0;k<hash_size;k++){ /*freeing the list of task details*/
    while(task_details){
        temp         = task_details;
        task_details = task_details->next;
        free(temp);

    }

    for(int k=0;k<net_hash_size;k++){/*freeing the list of network details*/

        temp_net     = hash_network;
        hash_network = hash_network->next;
        free(temp_net);

    }

  //  clean_interrupts2();
    close(socket_command);
    close(socket_send);
    free_cpu_memory();
    printf("we exited");
    fflush(stdout);

    return 0;
}
