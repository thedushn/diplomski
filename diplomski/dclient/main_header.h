//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_MAIN_HEADER_H
#define DIPLOMSKI_MAIN_HEADER_H

#include <gtk/gtk.h>
#include "common.h"
#include "semaphore.h"
#include "error.h"
//!main header please work

typedef  struct  NetMem_list NetMem_list;/*!structure for creating linked list for memory usage and network usage */

struct NetMem_list{

    float       data[2];
    NetMem_list *next;

};

typedef struct Cpu_List Cpu_list;/*!structure for creating linked list for cpu usage */
struct Cpu_List {

    float     *data;
    Cpu_list *next;
};

typedef struct Mega_Data Mega_Data;
struct Mega_Data{
    float           *cpu_stats;
    float           *mem_stats;
    float           *net_stats;
    T_Collection    *task_list;
    D_Collection    *device_list;
    I_Collection2   *interrupts_list;
    Mega_Data *next;
    Mega_Data *prev;
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

I_Collection2 *interrupts2;/*!list to the interrupts  */
Cpu_list *cpu_list;/*!list to the cpu usage  */
NetMem_list *net_list;/*!list to the network usage  */
NetMem_list *mem_list;/*!list to the memory usage  */
Mega_Data *m_data;
Mega_Data *mDataHead;
char p_dir[256];
long cpu_num;
long interrupt_num;
sem_t semt;//!<semaphore for letting the init_timeout function finish before we change the time interval
#define LIST_SIZE 240 /*!the max size of list of cpu, network and memory usage*/

GtkApplication *gtkApplication;//!< application

gboolean init_timeout();

void decRefresh();

void incRefresh();

void timeout_refresh();

int device_check(D_Collection *devices_new, int dev_num);

int task_check(T_Collection *tasks_new, int task_num);

void destroy_window(void);

void freeingMemory(void *array, __int32_t *array_size, int type);

void test_strtol(long val);

void set_record(GtkWidget *widget);

void pause_app(GtkWidget *button);

void activate (GtkApplication *app,gpointer  user_data);
void  free_mega_data(Mega_Data **m_ptr);

Mega_Data *allocate_stats();



#endif //DIPLOMSKI_MAIN_HEADER_H
