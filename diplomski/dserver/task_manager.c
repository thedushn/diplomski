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
#include "errno.h"

#include "functions.h"
#include "task_manager.h"
#include "cpu_usage.h"


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


int
get_task_details(int pid, Task *task) {
    FILE *file;
    char filename[96];
    char buffer[1024];
    float cpu_user = 0;
    float cpu_system = 0;


    snprintf(filename, 96, "/proc/%d/stat", pid);

    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, 1024, file) == NULL) {
        printf("nece da otvori fajl\n");
        return 1;
    }

    fclose(file);

    char *p1, *po, *p2;
    int i = 0;
    p1 = po = strchr(buffer, '(');
    p2 = strrchr(buffer, ')');
    while (po <= p2) {
        if (po > p1 && po < p2) {
            task->name[i++] = *po;
            task->name[i] = '\0';
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
           "%i %255s %1s %i %i %i %i %i %255s %255s %255s %255s %255s %" SCNu64 "   %" SCNu64 " %i %i %i %hi %i %i  %" SCNu64 " %" SCNu64 " %" SCNu64 " %255s %255s %255s %i %255s %255s %255s %255s %255s %255s %255s %255s %255s %255s %i %255s %255s",
           &task->pid,    // processid
           dummy,        // processname
           task->state,    // processstate
           &task->ppid,    // parentid
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
           &task->prio, // nice range from 19 to -19
           &idummy,    // hardcoded 0
           &idummy,    // itrealvalue time in jiffies to next SIGALRM send to this process
           &task->start_time,    // starttime jiffies the process started after system boot //clock ticks 100 ticks=1sec
           &task->vsz, // vsize in bytes
           &task->rss, // rss (number of pages in real memory)
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

    task->rss *= get_pagesize();
    int result = get_cpu_percent(task->pid, jiffies_user, &cpu_user, jiffies_system, &cpu_system);
    if (result == -1) {
        return result;
    }
    if (sprintf(task->cpu_user, "%f", cpu_user) < 0) {

        printf("nije uspelo convertovanje %s \n", task->cpu_user);
        return -1;
    }
    if (sprintf(task->cpu_system, "%f", cpu_system) < 0) {

        printf("nije uspelo convertovanje %s \n", task->cpu_system);
        return -1;
    }

    stat(filename, &sstat);
    pw = getpwuid(sstat.st_uid);
    task->uid = sstat.st_uid;
    strncpy(task->uid_name, (pw != NULL) ? pw->pw_name : "nobody", sizeof(task->uid_name));
    __uint64_t sec, hr, min, t;
    __uint64_t h, m, s;
    struct tm1 diff;


    sec = task->start_time / 100;
    hr = sec / 3600;
    t = sec % 3600;
    min = t / 60;
    sec = t % 60;


    h = 0;
    m = 0;
    s = sec + pocetno.tm_sec;
    if (s > 60) {
        m = s / 60;
        s = s % 60;
    }
    m = m + min + pocetno.tm_min;
    if (m > 60) {
        h = m / 60;
        m = m % 60;
    }
    h = h + pocetno.tm_hour + hr;


    task->stime.tm_hour = 0;
    task->stime.tm_min = 0;
    task->stime.tm_sec = 0;
    task->stime.tm_hour = (__uint32_t) h;
    task->stime.tm_min = (__uint32_t) m;
    task->stime.tm_sec = (__uint32_t) s;


    differenceBetweenTimePeriod(lokalno, task->stime, &diff);
    task->duration.tm_hour = diff.tm_hour;
    task->duration.tm_min = diff.tm_min;
    task->duration.tm_sec = diff.tm_sec;
    task->checked = false;

    return 0;
}

int
get_task_list(Task **array, int *niz) {

    Task *tasks_array;
    Task *temp = NULL;
    tasks_array = calloc(0, sizeof(Task));

    DIR *dir;
    struct dirent *d_file;
    char *directory = "/proc";
    int pid = 0;
    int g = 0;


    if ((dir = opendir(directory)) == NULL) {
        printf("error task dir %d\n", errno);
        return 1;
    }


    while ((d_file = readdir(dir)) != NULL) {

        if ((pid = (int) strtol(d_file->d_name, NULL, 0)) > 0) {


            g++;
            temp = realloc(tasks_array, g * sizeof(Task));

            if (temp != NULL) {
                tasks_array = temp;
            } else {
                free(tasks_array);
                closedir(dir);
                printf("relloc error %d \n", errno);
                return 1;
            }
            memset(&tasks_array[g - 1], 0, sizeof(Task));
            int result = get_task_details(pid, &tasks_array[g - 1]);
            if (result == -1) {
                free(tasks_array);
                closedir(dir);
                return -1;
            }
            if (result == 1) {

                g--;
            }


        }


    }


    *niz = g;
    *array = tasks_array;
    closedir(dir);
    return 0;

}
