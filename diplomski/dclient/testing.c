//
// Created by dushn on 23.9.20..
//

#include "testing.h"

#include "main_header.h"

#include <inttypes.h>
#include <memory.h>
#include <stdlib.h>
#include <errno.h>


int interrupts_write(I_Collection2 *array) {


    FILE *fp;
    char *filename = "interrupts.data";
    char *p;
    char buffer[256];
    memset(buffer,0,sizeof(buffer));
    strcat(buffer,p_dir);
    strcat(buffer,filename);
    ssize_t size;
    __uint64_t *ptr=NULL;
    if (array == NULL) {

        fprintf(stderr, "array is NULL failed\n");
        return 1;

    }

    if ((fp = fopen(buffer, "a+")) == NULL){
        printf("fopen failed \n");
        perror("Error");
        return 1;
    } //create a file if it doesnt exist




    time_t clk=time(NULL);
    p=ctime(&clk);
    size= strlen(p);
    p[size-1]=' ';
    fprintf(fp,"Time: %sDelay %d\n",p,confy.delay);

    while(array){
        fprintf(fp, "%s: ",array->interrupts.irq);
        ptr=array->interrupts.CPU;
        for(int i=0;i<cpu_num;i++){
            fprintf(fp,"%"PRIu64" ",*ptr++);
        }
        fprintf(fp,"%s\n",array->interrupts.name);
//        fprintf(fp, "%s %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %s %s %s %s \n",
//                array->interrupts.irq,
//                array->interrupts.CPU0,
//                array->interrupts.CPU1,
//                array->interrupts.CPU2,
//                array->interrupts.CPU3,
//                array->interrupts.ime1,
//                array->interrupts.ime2,
//                array->interrupts.ime3,
//                array->interrupts.ime4
//        );



        array=array->next;

    }



    fclose(fp);
    return 0;
}

int memory_write(Memory_usage *memory_usage){

    FILE *fp;
    char *filename = "memory.data";
    char *p;
    ssize_t size;
    char buffer[256];
    memset(buffer,0,sizeof(buffer));
    strcat(buffer,p_dir);
    strcat(buffer,filename);


    if (memory_usage == NULL) {

        fprintf(stderr, "array is NULL failed\n");
        return 1;

    }

    if ((fp = fopen(buffer, "a+")) == NULL){
        printf("fopen failed \n");
        perror("Error");
        return 1;
    } //create a file if it doesnt exist



    time_t clk=time(NULL);
    p=ctime(&clk);
    size= strlen(p);
    p[size-1]=' ';
    fprintf(fp,"Time: %sDelay %d\n",p,confy.delay);

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
    char buffer[256];
    memset(buffer,0,sizeof(buffer));
    strcat(buffer,p_dir);
    strcat(buffer,filename);




    if ((fp = fopen(buffer, "a+")) == NULL){
        printf("fopen failed \n");
        perror("Error");
        return 1;
    } //create a file if it doesnt exist


    time_t clk=time(NULL);
    p=ctime(&clk);
    size= strlen(p);
    p[size-1]=' ';
    fprintf(fp,"Time: %sDelay %d\n",p,confy.delay);
    for(int i=0;i<cpu_num;i++){
        fprintf(fp,"%s ",*cpu_usage.percentage);
        cpu_usage.percentage++;
    }
    fprintf(fp,"\n");
//    fprintf(fp, "%s %s %s %s \n",
//            cpu_usage.percentage[0],
//            cpu_usage.percentage[1],
//            cpu_usage.percentage[2],
//            cpu_usage.percentage[3]
//
//    );




    fclose(fp);
    return 0;

}
int cpu_read(Cpu_list **array) {


    FILE *fp;
    char *filename = "cpu.data";
   char buffer[1024];
   char *p;
    Cpu_list *temp=NULL;
    int ret=0;
    int time;
    if ((fp = fopen(filename, "r")) == NULL) //create a file if it doesnt exist
        return 1;




    while((fgets(buffer,1024,fp))!=NULL){

        if(strncmp(buffer,"Time:",5)==0){
            p=strchr(buffer,'D');
            ret= sscanf(p,"Delay %d",&time);
            if(fgets(buffer,1024,fp)==NULL){
                ret=-1;
                break;
            }
            if (buffer[0]=='\n'){
                if(fgets(buffer,1024,fp)==NULL){
                    ret=-1;
                    break;
                }
            }

        }
        temp = (Cpu_list *) calloc(1, sizeof(Cpu_list));
        if (temp == NULL) {
            printf("calloc error %d \n", errno);
            free(temp);
            ret=-1;
            break;
        }

        if(sscanf(buffer,"%f %f %f %f",&(temp)->data[0],&(temp)->data[1],&(temp)->data[2],&(temp)->data[3])==4){
            temp->next = (*array);
            (*array) = temp;
            list_num_size++;
        }
        else{
            printf("sscanf error  \n");
            free(temp);
            ret=-1;
            break;
        }





    }




    fclose(fp);
    return ret;

}
int task_write(T_Collection *array){
    FILE *fp;
    char *filename = "task.data";
    char *p;
    ssize_t size;

    char buffer[256];
    memset(buffer,0,sizeof(buffer));
    strcat(buffer,p_dir);
    strcat(buffer,filename);


    if (array == NULL) {

        fprintf(stderr, "array is NULL failed\n");
        return 1;

    }

    if ((fp = fopen(buffer, "a+")) == NULL){
        printf("fopen failed \n");
        perror("Error");
        return 1;
    } //create a file if it doesnt exist



    time_t clk=time(NULL);
    p=ctime(&clk);
    size= strlen(p);
    p[size-1]=' ';
    fprintf(fp,"Time: %sDelay %d\n",p,confy.delay);

    while(array!=NULL){
        fprintf(fp,
                "PID %i state %1s ppid %i prio %hi "
                        "vsz %" PRIu64 " rss %" PRIu64" "
                        "Name %s Uid_name %s uid %" PRIu32 " "
                        "CPU_system %s CPU_user %s duration: hour %" PRIu32 " "
                        "minute %" PRIu32 " second %" PRIu32 "\n",
                array->task.pid,    // processid
                array->task.state,    // processstate
                array->task.ppid,    // parentid
                array->task.prio, // nice range from 19 to -19

                array->task.vsz, // vsize in bytes
                array->task.rss, // rss (number of pages in real memory)
                array->task.name,
                array->task.uid_name,
                array->task.uid,
                array->task.cpu_system,
                array->task.cpu_user,
                array->task.duration.tm_hour,
                array->task.duration.tm_min,
                array->task.duration.tm_sec



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

    char buffer[256];
    memset(buffer,0,sizeof(buffer));
    strcat(buffer,p_dir);
    strcat(buffer,filename);


    if (array == NULL) {

        fprintf(stderr, "array is NULL failed\n");
        return 1;

    }

    if ((fp = fopen(buffer, "a+")) == NULL){
        printf("fopen failed \n");
        perror("Error");
        return 1;
    } //create a file if it doesnt exist

    time_t clk=time(NULL);
    p=ctime(&clk);
    size= strlen(p);
    p[size-1]=' ';
    fprintf(fp,"Time: %sDelay %d\n",p,confy.delay);

    while(array!=NULL){
        fprintf(fp,
                "Devices %s available %" PRIu64 "  free %" PRIu64 " type %s directory %s used %" PRIu64 ""
                        " total %" PRIu64 "\n",
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
int netw_write(uint64_t transmited, uint64_t received) {

    FILE *fp;
    char *filename = "network.data";
    char *p;
    ssize_t size;

    char buffer[256];
    memset(buffer,0,sizeof(buffer));
    strcat(buffer,p_dir);
    strcat(buffer,filename);


    if ((fp = fopen(buffer, "a+")) == NULL){
        printf("fopen failed \n");
        perror("Error");
        return 1;
    } //create a file if it doesnt exist


    time_t clk=time(NULL);
    p=ctime(&clk);
    size= strlen(p);
    p[size-1]=' ';
    fprintf(fp,"Time: %sDelay %d\n",p,confy.delay);

    fprintf(fp,"Transmited %"PRIu64"  Received %"PRIu64"\n",transmited,received);



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
int task_sort(T_Collection **array, int number) {

    Task temp;
    T_Collection *current=NULL;


    for(int i=0;i<number;i++){
        current =*array;



    while(current->next!=NULL){
        printf("PID before %d\n",current->task.pid);

        if(0<(int)(current->task.pid-current->next->task.pid))
        {
            temp=current->task;
            current->task       = current->next->task;
            current->next->task = temp;


        }
        printf("PID after_ %d\n",current->task.pid);
        current   =  current->next;
    }




    }




    return 0;
}