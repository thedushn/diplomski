//
// Created by dushn on 23.7.20..
//

#include "task_manager.h"
#include <inttypes.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/socket.h>
#include "errno.h"

#include "functions.h"
#include "cpu_usage.h"
#include "testing.h"

/*
 * function send_task(): send data about all the tasks to client
 * input : socket to send data to
 * output : void
 * */
void * send_task(void *socket){


    Thread_pack thr_p=*(Thread_pack*)socket;
    //ssize_t         *ret=NULL;
    Data            data={0};

    T_Collection    *tasks=NULL;
    T_Collection    *temp_task;

   __int32_t        task_num=0;
    printf("inside the %s\n",__func__);
    fflush(stdout);
    pthread_mutex_lock(&mutex_send);
    while (thread_break == false) {/*if other threads have failed close this thread before it allocates any memory*/

        pthread_mutex_unlock(&mutex_send);
        printf("inside the thread_break %s\n",__func__);
        fflush(stdout);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&mutex_send);

    printf("inside the %s\n",__func__);
    fflush(stdout);


    if (get_task_list(&tasks, &task_num) != 0) {

        printf("error in get_task_list\n");
        for(int k=0;k<task_num;k++){

            temp_task   = tasks;
            tasks       = tasks->next;
            free(temp_task);


        }
        thr_p.ret_val = -1;

        pthread_exit(&thr_p.ret_val);
    }

    temp_task=tasks;
    for (int i = 0; i < task_num; i++) {
        memset(&data,0,sizeof(Data));

        data.size=TASK;
        data.unification.task=temp_task->task;

      //  task_write(&temp_task->task);
        pthread_mutex_lock(&mutex_send);
        thr_p.ret_val = send(thr_p.socket, &data, sizeof(Data), 0);
        pthread_mutex_unlock(&mutex_send);

        if (thr_p.ret_val <= 0) {
            printf("Error sending data\n return = %d %s\n", (int) thr_p.ret_val,__FUNCTION__ );
            for(int k=0;k<task_num;k++){

                temp_task   = tasks;
                tasks       = tasks->next;
                free(temp_task);

            }
            thr_p.ret_val--;
            pthread_exit(&thr_p.ret_val);
        }


        temp_task=temp_task->next;


    }

    for(int k=0;k<task_num;k++){

        temp_task   = tasks;
        tasks       = tasks->next;
        free(temp_task);
    }

    pthread_exit(&thr_p.ret_val);
}
/*
 * function differenceBetweenTimePeriod(): calculates the age of the task
 * input : time the kernel started ,time the task started, and a pointer to the age of the task
 * output : none
 * */
void differenceBetweenTimePeriod(struct tm start, struct tm1 stop, struct tm1 *diff) {

    if (stop.tm_sec > start.tm_sec) {
        --start.tm_min;
        start.tm_sec += 60;
    }

    diff->tm_sec = start.tm_sec - stop.tm_sec;
    if (stop.tm_min > start.tm_min) {
        --start.tm_hour;
        start.tm_min += 60;
    }

    diff->tm_min = start.tm_min - stop.tm_min;
    if (stop.tm_hour > start.tm_hour) {
        --start.tm_mday;
        start.tm_hour += 1;
    }
    diff->tm_hour = start.tm_hour - stop.tm_hour;

}

/*
 * function get_pagesize(): function that calculates the real memory of a task
 * input : none
 * output : pagesize
 * */
static inline long get_pagesize(void) {

    static long pagesize = 0;
    if (pagesize == 0) {
        pagesize = sysconf(_SC_PAGESIZE);
        if (pagesize == 0)
            pagesize = 4096;
    }

    return pagesize;
}


/*
 * function get_task_details(): parses the data about a task to a pointer
 * input : pointer to a structure that contains a pointer to a task and thread and the result said thread
 * output : none
 * */
void * get_task_details(void *ptr) {

    Thread_task *thread_task=(Thread_task*)ptr;

    FILE *file;
    char filename [96];
    char buffer   [1024];
    char dummy    [256];
    int result ;
    int i      = 0;
    int idummy;
    __uint64_t sec, hr, min, t;
    __uint64_t h, m, s;
    struct tm1 diff;
    struct passwd *pw;
    struct stat sstat;


    char *p1, *po, *p2;



    snprintf(filename, 96, "/proc/%d/stat",thread_task->pid);

    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, 1024, file) == NULL) {
        printf("the file cant open %s\n",filename);
        result=1;
        thread_task->result=result; /*if the file cant open it means the task has just been made or its just been removed*/


        pthread_exit(NULL);
    }

    fclose(file);


    p1 = po = strchr(buffer, '(');/*removing '(' and ')' from the tasks name*/
    p2 = strrchr(buffer, ')');
    while (po <= p2) {
        if (po > p1 && po < p2) {
            thread_task->task->name[i++] = *po;
            thread_task->task->name[i] = '\0';
        }
        *po = 'x';
        po++;
    }





    __uint64_t jiffies_user = 0, jiffies_system = 0;



    sscanf(buffer,
           "%i %255s %1s %i %i %i %i %i %255s %255s %255s %255s %255s %" SCNu64 "   %" SCNu64 " %i %i %i %hi %i %i "
                   " %" SCNu64 " %" SCNu64 " %" SCNu64 " %255s %255s %255s %i %255s %255s %255s %255s %255s %255s "
                   "%255s %255s %255s %255s %i %255s %255s",
           &thread_task->task->pid,    // processid
           dummy,        // processname
           thread_task->task->state,    // processstate
           &thread_task->task->ppid,    // parentid
           &idummy,    // processs groupid

           &idummy,    // session id
           &idummy,    // tty id
           &idummy,    // tpgid the process group ID of the process running on tty of the process
           dummy,        // flags
           dummy,        // minflt minor faults the process has maid

           dummy,        // cminflt
           dummy,        // majflt
           dummy,        // cmajflt
           &jiffies_user,    // utime the number of jiffies that this process has scheduled in user mode
           &jiffies_system,// stime " system mode

           &idummy,    // cutime " waited for children in user mode
           &idummy,    // cstime " system mode
           &idummy,    // priority (nice value + fifteen)
           &thread_task->task->prio, // nice range from 19 to -19
           &idummy,    // hardcoded 0
           &idummy,    // itrealvalue time in jiffies to next SIGALRM send to this process
           &thread_task->task->start_time,    // starttime jiffies the process started after system boot //clock ticks 100 ticks=1sec
           &thread_task->task->vsz, // vsize in bytes
           &thread_task->task->rss, // rss (number of pages in real memory)
           dummy,        // rlim limit in bytes for rss

           dummy,        // startcode
           dummy,        // endcode
           &idummy,    // startstack
           dummy,        // kstkesp value of esp (stack pointer)
           dummy,        // kstkeip value of EIP (instruction pointer)

           dummy,        // signal. bitmap of pending signals
           dummy,        // blocked: bitmap of blocked signals
           dummy,        // sigignore: bitmap of ignored signals
           dummy,        // sigcatch: bitmap of catched signals
           dummy,        // wchan

           dummy,        // nswap
           dummy,        // cnswap
           dummy,        // exit_signal
           &idummy,    // CPU number last executed on
           dummy,

           dummy
    );

    thread_task->task->rss *= get_pagesize();


    if ((result=get_cpu_percent(jiffies_user, jiffies_system, thread_task->task)) == -1  ) {/*get the cpu usage of the task*/
        thread_task->result=result;

        pthread_exit(NULL);
    }


    if((result=stat(filename, &sstat))!=0){
        thread_task->result=result;

        pthread_exit(NULL);
    }
    if((pw = getpwuid(sstat.st_uid))==NULL){
        thread_task->result=-1;

        pthread_exit(NULL);
    }
    thread_task->task->uid = sstat.st_uid;
    strncpy(thread_task->task->uid_name, (pw != NULL) ? pw->pw_name : "nobody", sizeof(thread_task->task->uid_name));


    sec =thread_task->task->start_time / 100;
    hr = sec / 3600;
    t = sec % 3600;
    min = t / 60;
    sec = t % 60;


    h = 0;
    m = 0;
    s = sec + begin_time.tm_sec;
    if (s > 60) {
        m = s / 60;
        s = s % 60;
    }
    m = m + min + begin_time.tm_min;
    if (m > 60) {
        h = m / 60;
        m = m % 60;
    }
    h = h + begin_time.tm_hour + hr;


    thread_task->task->stime.tm_hour    = 0;
    thread_task->task->stime.tm_min     = 0;
    thread_task->task->stime.tm_sec     = 0;
    thread_task->task->stime.tm_hour    = (__uint32_t) h;
    thread_task->task->stime.tm_min     = (__uint32_t) m;
    thread_task->task->stime.tm_sec     = (__uint32_t) s;


    differenceBetweenTimePeriod(local_time, thread_task->task->stime, &diff);/*calculate the time the task has been alive*/

    thread_task->task->duration.tm_hour = diff.tm_hour;
    thread_task->task->duration.tm_min  = diff.tm_min;
    thread_task->task->duration.tm_sec  = diff.tm_sec;
    thread_task->task->checked          = false;

    thread_task->result=result;

    pthread_exit(NULL);

}
/*
 * function get_task_list(): gets all the task details and adds them to a linked list by using threads
 * input : double pointer to an array of tasks, pointer to the task number
 * output : a non zero value equals a failure
 * */
int get_task_list(T_Collection **array, __int32_t *task_num) {


    DIR             *dir;
    struct dirent   *d_file;
    char            *directory = "/proc";
    uint32_t        pid;
    int             thread_num  = 0;
    int             result;
    char            buffer[128]={0};
    Thread_task     *thread_task_main=NULL;
    Thread_task     *tp=NULL;
    T_Collection    *task_temp=NULL;



    if ((dir = opendir(directory)) == NULL) {
        printf("error task dir %d\n", errno);
        return -1;
    }

    while ((d_file = readdir(dir)) != NULL) { /*going through all the directories that have a number as a name*/

        if ((pid = (uint32_t) strtol(d_file->d_name, NULL, 0)) > 0) {
            tp=calloc(1,sizeof(Thread_task));
            task_temp=calloc(1,sizeof(T_Collection));
            if(task_temp==NULL||tp==NULL) {
                free(task_temp);
                free(tp);
                closedir(dir);
                for(int i=0;i<thread_num;i++){

                    tp=thread_task_main;
                    thread_task_main=thread_task_main->next;
                    free(tp);

                }
                printf("calloc error %d \n", errno);
                return -1;
            }


            thread_num++;
            tp->pid=pid;
            tp->task=&task_temp->task;/*passing the address to the structure to be filled*/


            if((result=pthread_create(&tp->pthread,NULL,get_task_details,(void*)tp))!=0){/*creating a thread to get details*/

                strerror_r(result,buffer,sizeof(buffer));
                fprintf(stderr,"error = %d (%s)\n",result,buffer);
                closedir(dir);
                free(task_temp);
                free(tp);
                thread_num--;

                for(int i=0;i<thread_num;i++){

                    tp=thread_task_main;
                    thread_task_main=thread_task_main->next;
                    free(tp);

                }
                return -1;

            }

            (*task_num)++;  /*adding a task to the double linked list*/
            task_temp->next = *array;

            if ((*array) != NULL) {
                (*array)->prev = task_temp;
            }


            *array           = task_temp;
            tp->next         = thread_task_main;
            thread_task_main = tp;



        }


    }

    tp=thread_task_main;
    for(int i=0;i<thread_num;i++){

       if((result=pthread_join(tp->pthread,NULL))!=0){/*if an error happened  free allocated memory*/
           strerror_r(result,buffer,sizeof(buffer));
           fprintf(stderr,"error = %d (%s)\n",result,buffer);
           closedir(dir);
           for(int j=0;j<thread_num;j++){

               tp               = thread_task_main;
               thread_task_main = thread_task_main->next;
               free(tp);

           }
           return -1;

       }


        tp=tp->next;

    }

    while(0<thread_num){/*going though all the return values we got back */

        tp=thread_task_main;
        if(tp->result!=0){ /*checking if we encountered errors*/

            if(tp->result==-1){/*if there was an memory allocation error*/
                for(int i=0;i<thread_num;i++){

                    tp               = thread_task_main;
                    thread_task_main = thread_task_main->next;
                    free(tp);

                }
                closedir(dir);
                return -1;


            }

            int j=0;

            task_temp = *array;

            T_Collection *t_temp;
            while( j<(*task_num)){


                if((tp->pid-task_temp->task.pid)==tp->pid){ /*if the error was an file opening issue*/


                    if(j==0){/*check if its at the begging of the list*/
                        t_temp = task_temp;
                        if (task_temp->next != NULL) {
                            task_temp = task_temp->next;
                            task_temp->prev = NULL;
                        }


                        *array = task_temp;

                        free(t_temp);

                        (*task_num)--;
                        break;
                    }else{


                        t_temp = task_temp;
                        task_temp->next->prev = task_temp->prev; /*linking prev element and the next together*/
                        task_temp->prev->next = task_temp->next;
                        free(t_temp);


                        (*task_num)--;
                        break;
                    }

                }else{

                    j++;
                    task_temp=task_temp->next;

                }

            }

        }



        thread_task_main=thread_task_main->next;/*free element of the list we no longer need*/
        free(tp);
        thread_num--;


    }

    test=false;
    closedir(dir);
    return 0;

}


