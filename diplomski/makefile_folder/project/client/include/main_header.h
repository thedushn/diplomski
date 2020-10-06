//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_MAIN_HEADER_H
#define DIPLOMSKI_MAIN_HEADER_H

#include <gtk/gtk.h>
#include "common.h"

//!main header please work

typedef  struct  _NetMem_list NetMem_list;/*!structure for creating linked list for memory usage and network usage */

struct _NetMem_list{

    float       data[2];
    NetMem_list *next;

};

typedef struct _Cpu_List Cpu_list;/*!structure for creating linked list for cpu usage */
struct _Cpu_List {

    float     data[CPU_NUM];
    Cpu_list *next;
};

GtkWidget *window_graphs;   /*!>widget for creating the graph buttons window*/
GtkWidget *process_swindow; /*!>widget for creating the process window for editing the columns in the liststore for tasks*/
GtkWidget *device_swindow;  /*widget for creating the device window for editing the columns in the liststore for devices*/
GtkWidget *entry;           /*widget where we enter text to send to the server as a command*/

int newsockfd; /*socket for requesting and receiving data*/
int newsockfd1;/*socket for sending commands*/

guint t;        /*!time interval for when the client requests data again*/
guint refresh;  /*!if the function init_timeout is in a loop this value is bigger then 0*/
guint time_step;/*!the space between the two data inputs*/

__int32_t dev_num_old; /*!<number of devices*/
__int32_t task_num_old;/*!number of tasks*/
__int32_t list_num_size;/*!the size of the lists of cpu usage network usage and memory usage cant be bigger then LIST_SIZE*/

bool *cpu_status;/*array of bools that represent each cpu if a bool is  true the cpu usage for that cpu is drawn*/


bool device_all;/**<<bool used to check if the client wants all the devices shown */
bool record;
D_Collection *devices_old;/*!list to the devices that we keep on client */
T_Collection *tasks_old;/*!list to the tasks that we keep on client */
Interrupts *interrupts;/*!list to the interrupts  */
Cpu_list *cpu_list;/*!list to the cpu usage  */
NetMem_list *net_list;/*!list to the network usage  */
NetMem_list *mem_list;/*!list to the memory usage  */



#define LIST_SIZE 240 /*!the max size of list of cpu, network and memory usage*/



gboolean init_timeout();

void dec_refresh();

void inc_refresh();

void timeout_refresh();

int device_check(D_Collection *devices_new, int dev_num);

int task_check(T_Collection *tasks_new, int task_num);

void destroy_window(void);

void freeing_memory(void *array, __int32_t *array_size, int type);

void test_strtol(int val);

void set_record(GtkWidget *widget);



#endif //DIPLOMSKI_MAIN_HEADER_H
