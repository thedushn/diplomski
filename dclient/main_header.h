//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_MAIN_HEADER_H
#define DIPLOMSKI_MAIN_HEADER_H

#include <gtk/gtk.h>
#include "common.h"
#include "semaphore.h"
#include "error.h"

typedef struct Cpu_usage Cpu_usage;
struct __attribute__((__packed__)) Cpu_usage {/**structure that contains cpu usage of all the cpus */

    char (*percentage)[16];

};

typedef struct Interrupt_Collection2 I_Collection2;
struct Interrupt_Collection2{

    Interrupts_send     interrupts;
    __uint64_t          * CPU;
    I_Collection2       * next;
    I_Collection2       * prev;
};
typedef struct ConfigS ConfigS;
struct ConfigS{
    unsigned int width;
    unsigned int height;
    unsigned int delay;
    bool record;
    bool fileS;
    char font[256];
    char name[256];
    unsigned int fontS;


};
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
    __uint64_t      delay;
    __uint64_t      gtk_delay;
    __uint64_t      network[2];
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
GtkWidget *device_swindow;  /*!>widget for creating the device window for editing the columns in the liststore for devices*/
GtkWidget *entry;           /*!>widget where we enter text to send to the server as a command*/

int newsockfd; /*socket for requesting and receiving data*/
int newsockfd1;/*socket for sending commands*/

//guint t;        /*!time interval for when the client requests data again*/
guint refresh;  /*!if the function init_timeout is in a loop this value is bigger then 0*/
guint time_step;/*!the space between the two data inputs*/

__int32_t dev_num_old; /*!<number of devices*/
__int32_t task_num_old;/*!number of tasks*/
__int32_t list_num_size;/*!the size of the lists of cpu usage network usage and memory usage cant be bigger then LIST_SIZE*/

bool *cpu_status;/*array of bools that represent each cpu if a bool is  true the cpu usage for that cpu is drawn*/


//bool device_all;/**<<bool used to check if the client wants all the devices shown */
//bool record;
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
int interrupt_num;
ConfigS confy;
#define LIST_SIZE 240 /*!the max size of list of cpu, network and memory usage*/
PangoFontDescription *fontdesc;
int read_config();
void check_config();
GtkApplication *gtkApplication;//!< application

/**
 * init_timeout(): sends a request to server and then waits for data,after it got all the data it inputs it in
 * the right places and checks if the list_num_size is bigger then the LIST_SIZE if that is the case it removes the
 * oldest element of the list and adds the newest to the begging.After the data has been properly handled it displays it
 * in the lists and draws the new data on the graph.We check if the function is running in an infinite loop,if not we
 * set it to run in regular intervals that we have set.
 *
 * @return  returns TRUE if we want to continue or FALSE if we want to stop
 * */
gboolean init_timeout();
/**
 * decRefresh(): decrease the time that we want the client to request data from server

 * @return void
 * */
void decRefresh();
/**
 * incRefresh(): increments the time that we want the client to request data from server
 *
 * @return void
 * */
void incRefresh();
/**
 * timeout_refresh(): reruns the function init_timeout and tells the previous version to stop
 *
 * @return void
 * */
void timeout_refresh();

int device_check(D_Collection *devices_new, D_Collection **dev_old);

int task_check(T_Collection *tasks_new, T_Collection **task_old);
/**
 * free_one_mega_data() frees the allocated memory of one element of a list of Mega_Data structs
 * @param m_ptr
 * @return void
 * */
void  free_one_mega_data(Mega_Data *m_ptr);
/**
 * destroy_window() destroys window
 * @return void
 *
 * */
void destroy_window(void);

/**
 * freeingMemory(): frees different types of memory
 * @param array
 * @param array_size
 * @param type of the array.
 * @return  void.
 * */
void freeingMemory(void *array, __int32_t *array_size, int type);
/**
 * test_strtol() tests if strtol didnt convert properly
 * @param val
 * @return void
 *
 * */

int test_strtol(__uint64_t val);
/**
 *set_record(): sets the record flag to true or false depending on if the button is clicked or not
 * @param widget
 * @return void
 * */
void set_record(GtkWidget *widget);
/**
 * pause_app(): stops or restarts the client from to requesting data from server
 * @param button if the button is toggled it pauses the data gathering
 * @return void
 * */
void pause_app(GtkWidget *button);
/**
 * activate() creates the application , the main window ,
 * the cpu window, the graphs, the labels and menu ,and lists
 * @param app
 * @param user_data default
 * @return void
 * */
void activate (GtkApplication *app,gpointer  user_data);
/**
 * free_mega_data() frees the allocated memory for a doubly linked list of Mega_Data structs
 * @param m_ptr
 * @return void
 * */
void  free_mega_data(Mega_Data **m_ptr);


/**
 * allocate_stats() allocates memory used for storing data gather from server
 *
 * @returns pointer to struct  Mega_Data
 * */
void allocate_stats(Mega_Data **n_ptr);

int test_strtof(double val);

int input_port_number(char *string);

int input_ip(char *string);

int validate_ip4dotted(char *string);

#endif //DIPLOMSKI_MAIN_HEADER_H
