//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_MAIN_HEADER_H
#define DIPLOMSKI_MAIN_HEADER_H

#include <gtk/gtk.h>
#include "common.h"

GtkWidget *window2;
GtkWidget *process_swindow;
GtkWidget *device_swindow;
GtkWidget *entry;

int newsockfd;
int newsockfd1;
int bjorg ;
guint t;

gint dev_num_old;
gint dev_num;
int task_num;
int task_num_old;

typedef struct _Time_Managment Time_Managment;

struct _Time_Managment {

    double time;
    Time_Managment *next;
};


typedef struct _Collection Collection;
struct _Collection {

    float data[8];
    Collection *next;
};



D_Collection *devices_old;
T_Collection *tasks_old;
Interrupts * interrupts;
Collection *collection;
Time_Managment *my_time;

#define LIST_SIZE 240
#define CPU_USAGE 1
#define NETWORK 2
#define MEMORY 3
#define TASK 4
#define DEVICES 5
#define INTERRUPTS 6
#define TEXT 7

 gboolean init_timeout();

void graph_refresh(GtkWidget *widget, gboolean);

void dec_refresh();

void inc_refresh();

void timeout_refresh();

void device_check(D_Collection *devices_new);

void task_check(T_Collection *tasks_new);






#endif //DIPLOMSKI_MAIN_HEADER_H
