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

int newsockfd;
int newsockfd1;
int bjorg ;

gint dev_num_old;
gint dev_num;
int task_num;
int task_num_old;




Collection * collection;
D_Collection *devices_old;
T_Collection *tasks_old;
Interrupts * interrupts;

#define LIST_SIZE 240
#define COLLETION 1
#define DEV_COLLETION 2
#define TASK_COLLETION 3
#define TASK 4
#define CPU_S 5
#define MEMORY 6
#define  NETWORK 7;
#define  INTERRUPTS 8;

void init_timeout();

void graph_refresh(GtkWidget *widget, gboolean);

void dec_refresh();

void inc_refresh();

void timeout_refresh();

void freeing_memory(Unification *array);



#endif //DIPLOMSKI_MAIN_HEADER_H
