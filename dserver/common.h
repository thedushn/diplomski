//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_COMMON_H
#define DIPLOMSKI_COMMON_H


#include <stdbool.h>
#include <time.h>
#include "pthread.h"



#define CPU_USAGE 1
#define NETWORK 2
#define MEMORY 3
#define TASK 4
#define DEVICES 5
#define INTERRUTPS 6
#define TEXT 7

pthread_mutex_t mutex_send;


struct __attribute__((__packed__))tm1 {
    __uint32_t tm_sec;            /* Seconds.	[0-60] (1 leap second) */
    __uint32_t tm_min;            /* Minutes.	[0-59] */
    __uint32_t tm_hour;            /* Hours.	[0-23] */
    __uint32_t tm_mday;            /* Day.		[1-31] */
    __uint32_t tm_mon;            /* Month.	[0-11] */
    __uint32_t tm_year;            /* Year	- 1900.  */
    __uint32_t tm_wday;            /* Day of week.	[0-6] */
    __uint32_t tm_yday;            /* Days in year.[0-365]	*/
    __uint32_t tm_isdst;            /* DST.		[-1/0/1]*/

};
typedef struct _Task Task;


struct __attribute__((__packed__)) _Task {
    bool checked;
    __int16_t prio;
    __uint32_t uid;
    __uint32_t pid;
    __uint32_t ppid;
    char cpu_user[16];
    char cpu_system[16];
    __uint64_t vsz;
    __uint64_t rss;
    __uint64_t start_time;
    struct tm1 stime;
    struct tm1 duration;
    char state[16];
    char name[256];
    char uid_name[256];


};
typedef struct _Network Network;
struct __attribute__((__packed__))_Network {

    __uint64_t received_bytes;
    __uint64_t transmited_bytes;


};
typedef struct _Cpu_usage Cpu_usage;
struct __attribute__((__packed__)) _Cpu_usage {


    char percentage0[16];
    char percentage1[16];
    char percentage2[16];
    char percentage3[16];


};


typedef struct _Memory_usage Memory_usage;

struct __attribute__((__packed__)) _Memory_usage {
    __uint64_t memory_used;
    __uint64_t memory_total;
    char swap_percentage[16];
    __uint64_t swap_total;
    __uint64_t swap_used;
    char memory_percentage[16];

};
typedef struct _Interrupts Interrupts;
struct __attribute__((__packed__))_Interrupts {

    char irq[64];
    char ime1[64];
    char ime2[64];
    char ime3[64];
    char ime4[64];
    __uint64_t CPU0;
    __uint64_t CPU1;
    __uint64_t CPU2;
    __uint64_t CPU3;

};
typedef struct _Commands Commands;
struct __attribute__((__packed__))_Commands {


    bool show;
    __uint32_t mem;
    char command[16];
    char task_id[256];
};

typedef struct _Devices Devices;
struct __attribute__((__packed__)) _Devices {


    bool checked;
    __uint64_t used;
    __uint64_t total;
    __uint64_t free;
    __uint64_t avail;
    __uint64_t fid;
    char name[64];
    char type[64];
    char directory[256];
};
typedef struct _Device_Collection D_Collection;
struct _Device_Collection{

    Devices devices;
    D_Collection * next;
};


typedef struct _Task_Collection T_Collection;
struct _Task_Collection{

    Task task;
    T_Collection * next;
};

typedef union _Unification Unification ;

union _Unification {

    Task task;
    Network network;
    Memory_usage memory_usage;
    Cpu_usage cpu_usage;
    Interrupts interrupts;
    Devices devices;
    char conformation[64];


};

typedef struct _Data Data;
struct __attribute__((__packed__)) _Data{

    int size;
    Unification unification;

};

#endif //DIPLOMSKI_COMMON_H
