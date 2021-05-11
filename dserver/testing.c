//
// Created by dushn on 23.9.20..
//

#include <stdio.h>
#include <inttypes.h>
#include <memory.h>
#include "testing.h"

#define BUFFER_SIZE 104

 int memory_write(Memory_usage *memory_usage){

     FILE *fp;
      char *filename = "memory_server.data";



     if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
         return 1;
     time_t clk=time(NULL);
     fprintf(fp,"%s\n",ctime(&clk));
     fprintf(fp,"total memory %" PRIu64 " memory_used %" PRIu64 " memory_free %" PRIu64 " swap_total %" PRIu64 "   swap_used %" PRIu64 "\n",
             memory_usage->memory_total,
             memory_usage->memory_used,
             ( memory_usage->memory_total- memory_usage->memory_used),
             memory_usage->swap_total,
             memory_usage->swap_used);


     fclose(fp);
     return 0;

 }

int task_write(Task *task){
    FILE *fp;
    char *filename = "task.data";

    if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
        return 1;


    fprintf(fp,
           "PID %i state %1s ppid %i  prio  %hi  "
                   " vsz %" SCNu64 " rss %" SCNu64"\n",
           task->pid,    // processid
           task->state,    // processstate
           task->ppid,    // parentid
           task->prio, // nice range from 19 to -19

           task->vsz, // vsize in bytes
           task->rss // rss (number of pages in real memory)

    );

    fclose(fp);


    return 0;
}
int netw_write(Network network){

    FILE *fp;
    char *filename = "net.data";

    if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
        return 1;


    time_t clk=time(NULL);
    fprintf(fp,"Time: %s\n",ctime(&clk));

    fprintf(fp,"Transmited  %" SCNu64 " Received %" SCNu64"\n",network.transmited_bytes,network.received_bytes);



    fclose(fp);


    return 0;

}