//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_COMMON_H
#define DIPLOMSKI_COMMON_H



#include <stdbool.h>
#include <time.h>




/*!defines what type of data we are sending */
#define CPU_USAGE   1
#define NETWORK     2
#define MEMORY      3
#define TASK        4
#define DEVICES     5
#define INTERRUPTS  6
#define TEXT        7
#define CPU_PACK    8
#define INT_PACK    9



struct __attribute__((__packed__))tm1 {/*structure that contains information about time used for tasks*/
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
/**<<structure that contains task details */
typedef struct Task Task;

struct __attribute__((__packed__)) Task {
    bool       checked;
    __int16_t  prio;
    __uint32_t uid;
    __uint32_t pid;
    __uint32_t ppid;
    char       cpu_user[16];
    char       cpu_system[16];
    __uint64_t vsz;
    __uint64_t rss;
    __uint64_t start_time;
    struct tm1 stime;
    struct tm1 duration;
    char       state[16];
    char       name[256];
    char       uid_name[256];


};
typedef struct Network Network;/**structure that contains all the network usage*/
struct __attribute__((__packed__))Network {

    __uint64_t received_bytes;
    __uint64_t transmited_bytes;


};



typedef struct Memory_usage Memory_usage;/*!structure that contains information about memory usage */

struct __attribute__((__packed__)) Memory_usage {

    __uint64_t memory_used;
    __uint64_t memory_total;
    char       swap_percentage[16];
    __uint64_t swap_total;
    __uint64_t swap_used;
    char       memory_percentage[16];


};
typedef struct Interrupts_send Interrupts_send;/*structure that contains the information of a interrupt type*/
struct __attribute__((__packed__)) Interrupts_send {

    __int64_t   total;
    char       irq[64];
    char       name[256];


};


typedef struct Devices Devices;
struct __attribute__((__packed__))Devices {


    bool       checked;
    __uint64_t used;
    __uint64_t total;
    __uint64_t free;
    __uint64_t avail;
    __uint64_t fid;
    char       name[64];
    char       type[64];
    char       directory[256];
};



typedef struct Device_Collection D_Collection;/*!doubly linked list for devices*/
struct Device_Collection{

    Devices        devices;
    D_Collection * next;
    D_Collection * prev;
};
/*!doubly linked list for tasks*/
typedef struct Task_Collection T_Collection;
struct Task_Collection{

    Task           task;
    T_Collection * next;
    T_Collection * prev;
};



typedef union Unification Unification ; /*!union data structure that uses the same memory space for all elements*/

union Unification {

    Task            task;
    Network         network;
    Memory_usage    memory_usage;
    Interrupts_send interrupts_send;
    Devices         devices;
    char            conformation[64];
    char            data_pack [1024];


};

typedef struct Data Data;/*!the structure we use to send data*/
struct __attribute__((__packed__)) Data{

    __int16_t         size;
    Unification unification;

};


#endif //DIPLOMSKI_COMMON_H
