//
// Created by dushn on 23.7.20..
//

#include "cpu_usage.h"
#include "functions.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/socket.h>


#define BUFFER_SIZE 1024

pthread_cond_t cpu_cond= PTHREAD_COND_INITIALIZER; /*cond_variable that checks if the jiffies_total_delta is being used */


//__uint64_t jiffies_total_delta[CPU_NUM + 1]; /*total cpu jiffies that occurred */
//__uint64_t jiffies_system[CPU_NUM];
//__uint64_t jiffies_total[CPU_NUM];
//__uint64_t jiffies_user[CPU_NUM];
//__uint64_t jiffies_user_old[CPU_NUM];
//__uint64_t jiffies_system_old[CPU_NUM];
//__uint64_t jiffies_total_old[CPU_NUM];

__uint64_t *jiffies_total_delta; /*total cpu jiffies that occurred */
__uint64_t *jiffies_system;
__uint64_t *jiffies_total;
__uint64_t *jiffies_user;
__uint64_t *jiffies_user_old;
__uint64_t *jiffies_system_old;
__uint64_t *jiffies_total_old;
__uint64_t *user;
__uint64_t *user_nice;
__uint64_t *idle;
__uint64_t *msystem;
float      *cpu_user;
float      *cpu_system;
float      *percentage;

int cpu_number (){

    int c=0;
    FILE *fp;
    char *filename = "/proc/cpuinfo";
    char *name = "cpu cores";
    char buffer[1024];
    if ((fp = fopen (filename, "r")) == NULL || fgets (buffer, 1024, fp) == NULL)
       return -1;

    while (fgets (buffer, 1024, fp) != NULL) {
        if (strncmp(buffer,name,strlen(name))==0)
        {
            sscanf(buffer, "cpu cores\t: %d", &c);
            break;
        }



    }
    fclose(fp);


    return c;
}
int cpu_data_allocation(){

    if((jiffies_total_delta=(__uint64_t*)calloc((size_t)(int)(cpu_Number+1),sizeof(__uint64_t)))==NULL){
        free(jiffies_total_delta);
        printf("calloc error %d \n", errno);
        return -1;
    }
    if((jiffies_system=(__uint64_t*)calloc((size_t)(cpu_Number),sizeof(__uint64_t)))==NULL){
        free(jiffies_system);
        printf("calloc error %d \n", errno);
        return -1;
    }
    if((jiffies_total=(__uint64_t*)calloc((size_t)(cpu_Number),sizeof(__uint64_t)))==NULL){
        free(jiffies_total);
        printf("calloc error %d \n", errno);
        return -1;
    }
    if((jiffies_user=(__uint64_t*)calloc((size_t)(cpu_Number),sizeof(__uint64_t)))==NULL){
        free(jiffies_user);
        printf("calloc error %d \n", errno);
        return -1;
    }

    if((jiffies_user_old=(__uint64_t*)calloc((size_t)(cpu_Number),sizeof(__uint64_t)))==NULL){
        free(jiffies_user_old);
        printf("calloc error %d \n", errno);
        return -1;
    }
    if((jiffies_system_old=(__uint64_t*)calloc((size_t)(cpu_Number),sizeof(__uint64_t)))==NULL){
        free(jiffies_system_old);
        printf("calloc error %d \n", errno);
        return -1;
    }
    if((jiffies_total_old=(__uint64_t*)calloc((size_t)(cpu_Number),sizeof(__uint64_t)))==NULL){
        free(jiffies_total_old);
        printf("calloc error %d \n", errno);
        return -1;
    }
    if((user=(__uint64_t*)calloc((size_t)(cpu_Number),sizeof(__uint64_t)))==NULL){
        free(user);
        printf("calloc error %d \n", errno);
        return -1;
    }
    if((user_nice=(__uint64_t*)calloc((size_t)(cpu_Number),sizeof(__uint64_t)))==NULL){
        free(user_nice);
        printf("calloc error %d \n", errno);
        return -1;
    }
    if((idle=(__uint64_t*)calloc((size_t)(cpu_Number),sizeof(__uint64_t)))==NULL){
        free(idle);
        printf("calloc error %d \n", errno);
        return -1;
    }
    if((msystem=(__uint64_t*)calloc((size_t)(cpu_Number),sizeof(__uint64_t)))==NULL){
        free(msystem);
        printf("calloc error %d \n", errno);
        return -1;
    }
    if((cpu_user=(float*)calloc((size_t)(cpu_Number),sizeof(float)))==NULL){
        free(cpu_user);
        printf("calloc error %d \n", errno);
        return -1;
    }
    if((cpu_system=(float*)calloc((size_t)(cpu_Number),sizeof(float)))==NULL){
        free(cpu_system);
        printf("calloc error %d \n", errno);
        return -1;
    }
    if((percentage=(float*)calloc((size_t)(cpu_Number),sizeof(float)))==NULL){
        free(percentage);
        printf("calloc error %d \n", errno);
        return -1;
    }

    return 0;
}
void free_cpu_memory(){

    if(jiffies_total_delta){
        free(jiffies_total_delta);
    }
    if(jiffies_system){
        free(jiffies_system);
    }
    if(jiffies_total){
        free(jiffies_total);
    }
    if(jiffies_user){
        free(jiffies_user);
    }
    if(jiffies_user_old){
        free(jiffies_user_old);
    }
    if(jiffies_system_old){
        free(jiffies_system_old);
    }
    if(jiffies_total_old){
        free(jiffies_total_old);
    }
    if(user){
        free(user);
    }
    if(user_nice){
        free(user_nice);
    }
    if(idle){
        free(idle);
    }
    if(msystem){
        free(msystem);
    }
    if(cpu_user){
        free(cpu_user);
    }
    if(cpu_system){
        free(cpu_system);
    }
    if(percentage){
        free(percentage);
    }

}
/*
 * function send_cpu():send data about cpu usage to client
 * input : socket to send data to
 * output : returns a value less then zero if something did not work
 * */
void *send_cpu(void *socket) {


   // ssize_t   *ret=NULL;
    Thread_pack thr_p=*(Thread_pack*)socket;
 //   Cpu_usage cpu_usage = {0};
    Data      data;

    pthread_mutex_lock(&mutex_send);
    while (thread_break == false) { /*if other threads have failed close this thread before it allocates any memory*/
        pthread_cond_signal(&cpu_cond);
        pthread_mutex_unlock(&mutex_send);
        printf("inside the thread_break %s\n",__func__);
        fflush(stdout);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&mutex_send);


    memset(&data, 0, sizeof(Data));
    if ((thr_p.ret_val = cpu_percentage(data.unification.data_pack)) < 0) {
        printf("Error getting cpu data!\n\t");
        pthread_exit(&thr_p.ret_val);

    }


   // memset(&data, 0, sizeof(Data));
    data.size = CPU_PACK;
 //   printf("%s\n",data.unification.data_pack);
  //  data.unification.cpu_usage = cpu_usage;

    pthread_mutex_lock(&mutex_send);

    thr_p.ret_val = send(thr_p.socket, &data, sizeof(Data), 0);
    pthread_mutex_unlock(&mutex_send);

    if (thr_p.ret_val < 0) {
        printf("Error sending data\n return = %d\n", (int) thr_p.ret_val);
        printf("Error sending data\n return = %d %s\n", (int)  thr_p.ret_val,__FUNCTION__ );


        pthread_exit(&thr_p.ret_val);

    }
    if (thr_p.ret_val == 0) {
        printf("Error sending data\n return = %d\n", (int) thr_p.ret_val);
        printf("socket closed\n");


        thr_p.ret_val = -1;
        pthread_exit(&thr_p.ret_val);
    }

    pthread_exit(&thr_p.ret_val);
}

/*
 * function cpu_percentage(): parsing data from /proc/stat
 * input : pointer to structure of data about cpu usage that we want to send to the client
 * output : returns a non zero value if something didn't work correctly
 * */
int cpu_percentage(char *array) {


    FILE *file;
    char *filename = "/proc/stat";
    char buffer[BUFFER_SIZE];

    char *p=NULL;

    char dummy[4];


    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, BUFFER_SIZE, file) == NULL){

        printf("error opening file\n");
        return -1;
    }



    for (int j = 0; j < cpu_Number; j++) {
        if(fgets(buffer, BUFFER_SIZE, file) != NULL){

            sscanf(buffer, "%s %" SCNu64 " %" SCNu64 " %" SCNu64 " %" SCNu64 "", dummy, &user[j],
                   &user_nice[j], &msystem[j], &idle[j]
            );

        }



    }
    fclose(file);

    pthread_mutex_lock(&mutex_cpu);
    test=false;
    for (int i = 0; i < cpu_Number; i++) {


        jiffies_user_old[i]     = jiffies_user[i];
        jiffies_system_old[i]   = jiffies_system[i];
        jiffies_total_old[i]    = jiffies_total[i];

        jiffies_user[i]         = user[i] + user_nice[i];
        jiffies_system[i]       = msystem[i];

        jiffies_total[i]        = jiffies_user[i] + jiffies_system[i] + idle[i];



        if (jiffies_total[i] > jiffies_total_old[i]) {


            jiffies_total_delta[i] = jiffies_total[i] - jiffies_total_old[i];

            cpu_user[i]   = (float)(jiffies_user[i] - jiffies_user_old[i])  / (float) (jiffies_total_delta[i]);
            cpu_system[i] = (float)(jiffies_system[i] - jiffies_system_old[i]) / (float) (jiffies_total_delta[i]);

        }

        percentage[i] = (cpu_user[i] + cpu_system[i]) * 100;



    }
    jiffies_total_delta[cpu_Number]=0;
    for(int i=0;i<cpu_Number;i++){
        jiffies_total_delta[cpu_Number]+=jiffies_total_delta[i];
    }
  //  jiffies_total_delta[cpu_Number] = jiffies_total_delta[0]+jiffies_total_delta[1]+jiffies_total_delta[2]+ jiffies_total_delta[3];



        test=true;
    printf("inside the %s\n",__func__);
    fflush(stdout);
   int s= pthread_cond_signal(&cpu_cond);
    if(s!=0)
        handle_error_en(s, "pthread_cond_signal");
    printf("signal cpu_cond %s\n",__func__);
    fflush(stdout);
   s= pthread_mutex_unlock(&mutex_cpu);
    if(s!=0)
        handle_error_en(s, "unlock");
    printf("unlock cpu_cond %s\n",__func__);
    fflush(stdout);
    memset(buffer,0,sizeof(buffer));
    p=buffer;
    for (int i = 0; i < cpu_Number; i++) {
    int mov;



        if ((mov=sprintf(p, "%3.2f ", percentage[i])) < 0) {

            printf("converting didn't work %s \n", p);


            return -1;
        }
        p=p+mov;
      //  *p++=' ';

    }

    strcpy(array,buffer);
  //  printf("buffer %s\n",buffer);

    return 0;
}
/*
 * function search(): searches the linked list to find a tasks cpu usage data save the old data and replace it with the
 * new cpu usage data
 * input : bool  pointer that we change to  true if the task is already in the list,
 * structure of data about cpu usage that we want to save in the list and the pointer to the task we are checking if it
 * exist in the list
 * output : returns a structure about a tasks cpu usage data
 * */

struct Cpu_data search(bool *ima, struct Cpu_data new, Task *task) {


        struct DataItem *temp=task_details;
        for(int i=0;i<hash_size;i++){

               if(temp->pid==task->pid) {
                   *ima =true;

                      if( strcmp(task->name,temp->name)!=0){//if the pid was already taken
                          strcpy(temp->name,task->name);
                          temp->cpu_user=new.cpu_user;
                          temp->cpu_system=new.cpu_system;
                          return new;
                      }
                   struct Cpu_data old;
                   old.cpu_system=temp->cpu_system;
                   old.cpu_user=temp->cpu_user;

                   temp->cpu_user=new.cpu_user;
                   temp->cpu_system=new.cpu_system;
                   return old;
               }
            temp=temp->next;
        }


    return new;
}


/*
 * function get_cpu_percent(): calculates the cpu percentage that the task is doing
 *
 * input : jiffies_user_local, jiffies_system_local, and pointer to a structure Task to which we parse the cpu percent
 * output : returns a non zero value if something goes wrong
 * */
int get_cpu_percent(__uint64_t jiffies_user_local, __uint64_t jiffies_system_local, Task *task) {

    struct Cpu_data      old;
    struct Cpu_data      new;
    struct DataItem      *temp=NULL;
    float                user_cpu;
    float                system_cpu;
    bool                 exists;
    //TODO add a mutex to keep track of the list of data items
    pthread_mutex_lock(&mutex_jiff);
    static int i=0;
    exists = false;
    new.cpu_system=jiffies_system_local;
    new.cpu_user=jiffies_user_local;


    old= search(&exists, new, task);


    if (exists == false) {/*if the task doesnt exist in the list*/

        temp=(struct DataItem *) calloc(1, sizeof(struct DataItem));

        if (temp == NULL) {
            free(temp);
            printf("calloc error %d \n", errno);
            pthread_mutex_unlock(&mutex_jiff);
            return -1;
        }


        temp->cpu_user  = jiffies_user_local;
        temp->cpu_system= jiffies_system_local;
        temp->pid       = task->pid;

        strcpy(temp->name,task->name);

        temp->next      = task_details;
        task_details    = temp;



        hash_size++;



    }



    printf("inside the %s one %d \n",__func__,i);
    fflush(stdout);
    while(test==false){
        printf("inside the cpu_cond %s %d \n",__func__,i);
     int  s= pthread_cond_wait(&cpu_cond,&mutex_cpu);
        if(s!=0)
            handle_error_en(s, "pthread_cond_wait");
    } /*if the cpu usage thread didn't acquire data about jiffies */


    printf("inside the %s two %d \n",__func__,i);
    fflush(stdout);
    ++i;
        if (jiffies_total_delta[cpu_Number] > 0) {


            user_cpu   = (float) ((jiffies_user_local) - (old.cpu_user)) * 100 / (float) (jiffies_total_delta[cpu_Number]);
            system_cpu = (float) ((jiffies_system_local - old.cpu_system) * 100) / (float) jiffies_total_delta[cpu_Number];

        } else {

            user_cpu = system_cpu = 0;

        }

    pthread_mutex_unlock(&mutex_jiff);


    if (sprintf(task->cpu_user, "%f", user_cpu) < 0) {

        printf("conversion didn't work %s \n", task->cpu_user);
        return -1;
    }
    if (sprintf(task->cpu_system, "%f", system_cpu) < 0) {

        printf("conversion didn't work %s \n", task->cpu_system);
        return -1;
    }

    return 0;
}

