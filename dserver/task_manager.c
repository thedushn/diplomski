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

void * send_task(void *socket){


    int result;
    ssize_t ret;
    Data data={0};

    T_Collection *tasks;
    T_Collection *temp_task;

   __int32_t task_num=0;

    result = get_task_list(&tasks, &task_num);
    if (result != 0) {

        printf("error in get_task_list\n");
        for(int k=0;k<task_num;k++){
            // save reference to first link
            temp_task = tasks;

            //mark next to first link as first
            tasks = tasks->next;

            //return the deleted link
            free(temp_task);


        }
        pthread_exit(NULL);
    }

    temp_task=tasks;
    for (int i = 0; i < task_num; i++) {
        memset(&data,0,sizeof(Data));

        data.size=TASK;
        data.unification.task=temp_task->task;
        pthread_mutex_lock(&mutex_send);
        ret = send((*(int*)socket), &data, sizeof(Data), 0);
        pthread_mutex_unlock(&mutex_send);


        if (ret < 0) {
            printf("Error sending data!\n\t");

            for(int k=0;k<task_num;k++){
                // save reference to first link
                temp_task = tasks;

                //mark next to first link as first
                tasks = tasks->next;

                //return the deleted link
                free(temp_task);


            }
           pthread_exit(NULL);

        }
        if (ret == 0) {

            printf("socket closed\n");
            for(int k=0;k<task_num;k++){
                // save reference to first link
                temp_task = tasks;

                //mark next to first link as first
                tasks = tasks->next;

                //return the deleted link
                free(temp_task);


            }
            pthread_exit(NULL);
        }

        temp_task=temp_task->next;


    }

    for(int k=0;k<task_num;k++){
        // save reference to first link
        temp_task = tasks;

        //mark next to first link as first
        tasks = tasks->next;

        //return the deleted link
        free(temp_task);


    }

    pthread_exit(&ret);
}

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
    diff->tm_hour = start.tm_hour - stop.tm_hour;

}


static inline long get_pagesize(void) {

    static long pagesize = 0;
    if (pagesize == 0) {
        pagesize = sysconf(_SC_PAGESIZE);
        if (pagesize == 0)
            pagesize = 4096;
    }

    return pagesize;
}



void * get_task_details(void *ptr) {

    Thread_task *thread_task=(Thread_task*)ptr;

    FILE *file;
    char filename[96];
    char buffer[1024];
    int result=0;


    snprintf(filename, 96, "/proc/%d/stat",thread_task->pid);

    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, 1024, file) == NULL) {
        printf("the file cant open %s\n",filename);
        result=1;
        thread_task->result=result;


        pthread_exit(NULL);
    }

    fclose(file);

    char *p1, *po, *p2;
    int i = 0;
    p1 = po = strchr(buffer, '(');
    p2 = strrchr(buffer, ')');
    while (po <= p2) {
        if (po > p1 && po < p2) {
            thread_task->task->name[i++] = *po;
            thread_task->task->name[i] = '\0';
        }
        *po = 'x';
        po++;
    }


    char dummy[256];
    int idummy;


    __uint64_t jiffies_user = 0, jiffies_system = 0;
    struct passwd *pw;
    struct stat sstat;


    sscanf(buffer,
           "%i %255s %1s %i %i %i %i %i %255s %255s %255s %255s %255s %" SCNu64 "   %" SCNu64 " %i %i %i %hi %i %i "
                   " %" SCNu64 " %" SCNu64 " %" SCNu64 " %255s %255s %255s %i %255s %255s %255s %255s %255s %255s %255s %255s %255s %255s %i %255s %255s",
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

    result= get_cpu_percent(jiffies_user, jiffies_system, thread_task->task);
    if (result == -1  ) {
        thread_task->result=result;

        pthread_exit(NULL);
    }


    stat(filename, &sstat);
    pw = getpwuid(sstat.st_uid);
    thread_task->task->uid = sstat.st_uid;
    strncpy(thread_task->task->uid_name, (pw != NULL) ? pw->pw_name : "nobody", sizeof(thread_task->task->uid_name));
    __uint64_t sec, hr, min, t;
    __uint64_t h, m, s;
    struct tm1 diff;


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


    thread_task->task->stime.tm_hour = 0;
    thread_task->task->stime.tm_min = 0;
    thread_task->task->stime.tm_sec = 0;
    thread_task->task->stime.tm_hour = (__uint32_t) h;
    thread_task->task->stime.tm_min = (__uint32_t) m;
    thread_task->task->stime.tm_sec = (__uint32_t) s;


    differenceBetweenTimePeriod(local_time, thread_task->task->stime, &diff);
    thread_task->task->duration.tm_hour = diff.tm_hour;
    thread_task->task->duration.tm_min = diff.tm_min;
    thread_task->task->duration.tm_sec = diff.tm_sec;
    thread_task->task->checked = false;

    thread_task->result=result;

    pthread_exit(NULL);

}
int get_task_list(T_Collection **array, __int32_t *task_num) {





    DIR *dir;
    struct dirent *d_file;
    char *directory = "/proc";
    uint32_t pid = 0;
    int thread_num=0;
    int result=0;
    char buffer[128];
    Thread_task *thread_task_main=NULL;
    Thread_task *tp=NULL;
    T_Collection  *task_temp=NULL;

    if ((dir = opendir(directory)) == NULL) {
        printf("error task dir %d\n", errno);
        return 1;
    }

    while ((d_file = readdir(dir)) != NULL) {

        if ((pid = (uint32_t) strtol(d_file->d_name, NULL, 0)) > 0) {
            tp=calloc(1,sizeof(Thread_task));
            task_temp=calloc(1,sizeof(T_Collection));
            if(task_temp==NULL||tp==NULL) {
                free(task_temp);
                free(tp);
                closedir(dir);
                printf("calloc error %d \n", errno);
                return 1;
            }

            (*task_num)++;
            thread_num++;
            tp->pid=pid;
            tp->task=&task_temp->task;


            if((result=pthread_create(&tp->pthread,NULL,get_task_details,tp))!=0){
                strerror_r(result,buffer,sizeof(buffer));
                fprintf(stderr,"pthread_create: error = %d (%s)\n",result,buffer);
                closedir(dir);
                (*task_num)--;
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
            task_temp->next=*array;
            *array=task_temp;



                tp->next=thread_task_main;

            if(thread_task_main!=NULL)
                thread_task_main->prev=tp;

            thread_task_main=tp;





        }


    }

    tp=thread_task_main;
    for(int i=0;i<thread_num;i++){

       if((result=pthread_join(tp->pthread,NULL))!=0){
           strerror_r(result,buffer,sizeof(buffer));
           fprintf(stderr,"error = %d (%s)\n",result,buffer);
           closedir(dir);
           for(int j=0;j<thread_num;j++){
               tp=thread_task_main;
               free(tp);
               thread_task_main=thread_task_main->next;

           }
           return -1;

       }

        if(tp->result!=0){
            printf("Thread pointer %d \n",tp->result);
        }
        tp=tp->next;

    }

    for(int i=0;i<thread_num;i++){
        tp=thread_task_main;
        if(tp->result!=0){
            printf(" Result %d task_pid %d \n",tp->result,tp->pid);

            task_temp=*array;
            int j=0;
            T_Collection *t_temp;
            while( j<(*task_num)){

                if((tp->pid-task_temp->task.pid)==tp->pid){

                    if(j==0){



                            *array=(*array)->next;
                        free(task_temp);

                        (*task_num)--;
                        break;
                    }else{


                        task_temp=*array;
                        for (int k = 0; k < j - 1; k++) {
                            task_temp = task_temp->next; //move one before

                        }

                        t_temp = task_temp->next;   //t_temp is the one to be removed

                        task_temp->next = t_temp->next;
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

        free(tp);
        thread_task_main=thread_task_main->next;

    }






    closedir(dir);
    return 0;

}


