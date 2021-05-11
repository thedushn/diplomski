//
// Created by dushn on 23.7.20..
//

#include "memory_usage.h"
#include "functions.h"
#include <inttypes.h>
#include <memory.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "testing.h"

/**
 * send_memory(): send data about the memory usage to client
 * @param socket to send data to
 * @return void
 * */
void * send_memory(void *socket){

   // int sockfd=(*(int*)socket);
  //  ssize_t *ret=NULL;
    Thread_pack thr_p=*(Thread_pack*)socket;

    Memory_usage  memory_usage={0};
    Data data={0};


   int s= pthread_mutex_lock(&mutex_send);
    if (s != 0)
        handle_error_en(s, "pthread_mutex_lock");

    while (thread_break == false) {/*if other threads have failed close this thread before it allocates any memory*/

       s= pthread_mutex_unlock(&mutex_send);
        if (s != 0)
            handle_error_en(s, "pthread_mutex_unlock");
        printf("inside the thread_break %s\n",__func__);
        fflush(stdout);
        pthread_exit(NULL);
    }
   s= pthread_mutex_unlock(&mutex_send);
    if (s != 0)
        handle_error_en(s, "pthread_mutex_unlock");




    thr_p.ret_val=(ssize_t)  get_memory_usage(&memory_usage);
    if (thr_p.ret_val < 0) {

        pthread_exit(&thr_p.ret_val);
    }
    //memory_write(&memory_usage);

    data.size=MEMORY;
    data.unification.memory_usage=memory_usage;

    pthread_mutex_lock(&mutex_send);
    thr_p.ret_val = send(thr_p.socket, &data, sizeof(Data), 0);
    pthread_mutex_unlock(&mutex_send);

    if ( thr_p.ret_val < 0) {
        printf("Error sending data\n return = %d %s\n", (int)  thr_p.ret_val,__FUNCTION__ );
        pthread_exit( &thr_p.ret_val);

    }
    if ( thr_p.ret_val == 0) {
        printf("Error sending data\n return = %d\n", (int)  thr_p.ret_val);
        printf("socket closed\n");
        thr_p.ret_val = -1;
        pthread_exit( &thr_p.ret_val);

    }


    pthread_exit( &thr_p.ret_val);
}

/**
 * get_memory_usage(); parsing data from proc/meminfo
 * @param memory_usage pointer to structure of data about memory usage that we want to send to the client
 * @return  non zero value if something didn't work correctly
 * */
int get_memory_usage(Memory_usage *memory_usage) {




    __uint64_t memory_total     = 0;
    __uint64_t memory_free      = 0;
    __uint64_t memory_cache     = 0;
    __uint64_t memory_buffers   = 0;
    __uint64_t swap_total       = 0;
    __uint64_t swap_free        = 0;
    __uint64_t swap_used        = 0;

    FILE *file;
    char buffer[1024];
    char *filename = "/proc/meminfo";
    int found = 0;

    float swap_percentage   = 0;
    float percentage        = 0;

   if(( file = fopen(filename, "r "))==NULL){
       printf("the file cant open %s\n",filename);
       return -1;
   }


    while (found < 6 && fgets(buffer, 1024, file) != NULL) {


        found += sscanf(buffer, "MemTotal:\t %" SCNu64 " kB",   &memory_total);
        found += sscanf(buffer, "MemFree:\t%" SCNu64 " kB",     &memory_free);
        found += sscanf(buffer, "Cached:\t%" SCNu64 " kB",      &memory_cache);
        found += sscanf(buffer, "Buffers:\t%" SCNu64 " kB",     &memory_buffers);
        found += sscanf(buffer, "SwapTotal:\t%" SCNu64 " kB",   &swap_total);
        found += sscanf(buffer, "SwapFree:\t%" SCNu64 " kB",    &swap_free);

    }
    fclose(file);

    //output is in  kB
    memory_total    *= 1024;
    memory_free     *= 1024;
    memory_cache    *= 1024;
    memory_buffers  *= 1024;
    swap_total      *= 1024;
    swap_free       *= 1024;

    memory_usage->memory_total = memory_total;
    memory_usage->swap_total = swap_total;


#ifdef __arm__

    __uint64_t memory_used=memory_total - memory_free;
#else

    __uint64_t memory_used = memory_total - memory_free - memory_cache - memory_buffers;
#endif


    swap_used = swap_total - swap_free;

    swap_percentage = (float) (swap_total != 0) ? (float)swap_used * 100 / (float) swap_total : 0;

    if (sprintf(memory_usage->swap_percentage, "%f", swap_percentage) < 0) {

        printf("conversion did not work %s \n", memory_usage->swap_percentage);
        strcpy(memory_usage->swap_percentage, "0");
    }
    memory_usage->swap_used     = swap_used;
    memory_usage->memory_used   = memory_used;


    percentage = (memory_total != 0) ?  ((float)memory_used * 100 /(float) memory_total) : 0;


    if (sprintf(memory_usage->memory_percentage, "%f", percentage) < 0) {

        printf("conversion did not work %s \n", memory_usage->memory_percentage);
        strcpy(memory_usage->swap_percentage, "0");
    }

     return 0;
}