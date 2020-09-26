//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_MAIN_HEADER_H
#define DIPLOMSKI_MAIN_HEADER_H

#include <gtk/gtk.h>
#include "common.h"



typedef  struct  _NetMem_list NetMem_list;

struct _NetMem_list{

    float data[2];
    NetMem_list *next;

};

typedef struct _Cpu_List Cpu_list;
struct _Cpu_List {

    float data[CPU_NUM];
    Cpu_list *next;
};

GtkWidget *window2;
GtkWidget *process_swindow;
GtkWidget *device_swindow;
GtkWidget *entry;

int newsockfd;
int newsockfd1;

guint t;
guint refresh;
guint time_step;

__int32_t dev_num_old;
__int32_t task_num_old;
__int32_t bjorg;

bool *cpu_status;

gboolean show_before;
bool device_all;

D_Collection *devices_old;
T_Collection *tasks_old;
Interrupts *interrupts;
Cpu_list *cpu_list;
NetMem_list *net_list;
NetMem_list *mem_list;



#define LIST_SIZE 240



gboolean init_timeout();

void dec_refresh();

void inc_refresh();

void timeout_refresh();

int device_check(D_Collection *devices_new, int dev_num);

int task_check(T_Collection *tasks_new, int task_num);

void destroy_window(void);

void freeing_memory(void *array, __int32_t *array_size, int type);





#endif //DIPLOMSKI_MAIN_HEADER_H
