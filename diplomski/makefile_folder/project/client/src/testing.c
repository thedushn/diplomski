//
// Created by dushn on 23.9.20..
//

#include "testing.h"

#include "main_header.h"

#include <inttypes.h>
#include <memory.h>
#include <stdlib.h>



int interrupts_write(Interrupts *array) {


    FILE *fp;
    char *filename = "interrupts.data";
    char *p;
    ssize_t size;

    if (array == NULL) {

        fprintf(stderr, "array is NULL failed\n");
        return 1;

    }
    if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
        return 1;


    time_t clk=time(NULL);
    p=ctime(&clk);
    size= strlen(p);
    p[size-1]=' ';
    fprintf(fp,"Time: %sDelay %d\n",p,t);

    for (int i = 0; i < 10; i++) {

        fprintf(fp, " \n %3s %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %s %s %s %s \n", array->irq, array->CPU0,
                array->CPU1, array->CPU2, array->CPU3, array->ime1, array->ime2, array->ime3, array->ime4);



        array++;

    }



    fclose(fp);
    return 0;
}

int memory_write(Memory_usage *memory_usage){

    FILE *fp;
    char *filename = "memory.data";
    char *p;
    ssize_t size;

    if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
        return 1;

    time_t clk=time(NULL);
    p=ctime(&clk);
    size= strlen(p);
    p[size-1]=' ';
    fprintf(fp,"Time: %sDelay %d\n",p,t);

    fprintf(fp,"total memory %" PRIu64 " memory_used %" PRIu64 " memory_free %" PRIu64 " swap_total %" PRIu64 "   "
                    "swap_used %" PRIu64 "\n",
            memory_usage->memory_total,
            memory_usage->memory_used,
            ( memory_usage->memory_total- memory_usage->memory_used),
            memory_usage->swap_total,
            memory_usage->swap_used);


    fclose(fp);
    return 0;

}
int cpu_write(Cpu_usage cpu_usage) {


    FILE *fp;
    char *filename = "cpu.data";
    char *p;
    ssize_t size;


    if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
        return 1;


    time_t clk=time(NULL);
    p=ctime(&clk);
    size= strlen(p);
    p[size-1]=' ';
    fprintf(fp,"Time: %sDelay %d\n",p,t);

    fprintf(fp, "%s %s %s %s \n",
            cpu_usage.percentage[0],
            cpu_usage.percentage[1],
            cpu_usage.percentage[2],
            cpu_usage.percentage[3]

    );




    fclose(fp);
    return 0;

}
int task_write(T_Collection *array){
    FILE *fp;
    char *filename = "task.data";
    char *p;
    ssize_t size;

    if (array == NULL) {

        fprintf(stderr, "array is NULL failed\n");
        return 1;

    }

    if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
        return 1;

    time_t clk=time(NULL);
    p=ctime(&clk);
    size= strlen(p);
    p[size-1]=' ';
    fprintf(fp,"Time: %sDelay %d\n",p,t);

    while(array!=NULL){
        fprintf(fp,
                "PID %i state %1s ppid %i  prio  %hi  "
                        " vsz %" SCNu64 " rss %" SCNu64"\n",
                array->task.pid,    // processid
                array->task.state,    // processstate
                array->task.ppid,    // parentid
                array->task.prio, // nice range from 19 to -19

                array->task.vsz, // vsize in bytes
                array->task.rss // rss (number of pages in real memory)

        );
        array=array->next;
    }


    fclose(fp);


    return 0;
}
int device_write(D_Collection *array){
    FILE *fp;
    char *filename = "device.data";
    char *p;
    ssize_t size;

    if (array == NULL) {

        fprintf(stderr, "array is NULL failed\n");
        return 1;

    }

    if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
        return 1;

    time_t clk=time(NULL);
    p=ctime(&clk);
    size= strlen(p);
    p[size-1]=' ';
    fprintf(fp,"Time: %sDelay %d\n",p,t);

    while(array!=NULL){
        fprintf(fp,
                "Devices %s available %" SCNu64 "  free %" SCNu64 " type %s directory %s used %" SCNu64 ""
                        " total %" SCNu64 "\n",
                array->devices.name,
                array->devices.avail,
                array->devices.free,
                array->devices.type,
                array->devices.directory,
                array->devices.used,
                array->devices.total




        );
        array=array->next;
    }


    fclose(fp);


    return 0;
}
int netw_write(float transmited,float received) {

    FILE *fp;
    char *filename = "network.data";
    char *p;
    ssize_t size;

    if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
        return 1;


    time_t clk=time(NULL);
    p=ctime(&clk);
    size= strlen(p);
    p[size-1]=' ';
    fprintf(fp,"Time: %sDelay %d\n",p,t);

    fprintf(fp,"Transmited %f  Received %f\n",transmited,received);



    fclose(fp);


    return 0;

}
int netw_calculate(float *transmited, float *received){

    FILE *fp;
    char *filename = "network.data";
    char buffer[1024];
    float  temp_tr;
    float  temp_rec;

    if ((fp = fopen(filename, "r")) == NULL) //open file
        return 1;


    while((fgets(buffer,1024,fp))!=NULL){

        if(strncmp(buffer,"Time:",5)==0){
            fgets(buffer,1024,fp);
            if (buffer[0]=='\n'){
                fgets(buffer,1024,fp);
            }

        }
        sscanf(buffer,"Transmited %f  Received %f",&temp_tr,&temp_rec);
        (*transmited)+=temp_tr;
        (*received)+=temp_rec;
        temp_tr=0;
        temp_rec=0;

    }




    fclose(fp);


    return 0;

}
int ifstat_calculate(float *received_kb, float *transmitted_kb) {

    FILE *fp;
    char *filename = "net_ifstat.data";
    char buffer[1024];
    float  temp_tr;
    float  temp_rec;

    int i=0;

    if ((fp = fopen(filename, "r")) == NULL) //open file
        return 1;


    while((fgets(buffer,1024,fp))!=NULL){


           while(buffer[i]==' '){

               i++;
           }
        if(buffer[i]=='K'){

            fgets(buffer,1024,fp);

           while(sscanf(buffer,"%f %f",&temp_tr,&temp_rec)==2){
              if (fgets(buffer,1024,fp)==NULL){
                  break;
              }
               ( *received_kb)+= temp_rec;
               (*transmitted_kb)+= temp_tr;
               temp_rec=0;
               temp_tr=0;
           }
        }
        i=0;


    }




    fclose(fp);


    return 0;

}