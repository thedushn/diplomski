//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_MAIN_HEADER_H
#define DIPLOMSKI_MAIN_HEADER_H

#include <gtk/gtk.h>
#include "common.h"
#include "drawing.h"
GtkWidget *window2;
GtkWidget *process_swindow;
GtkWidget *device_swindow;
GtkWidget *entry;

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
#define CPU_USAGE 1
#define NETWORK 2
#define MEMORY 3
#define TASK 4
#define DEVICES 5
#define INTERRUTPS 6
#define TEXT 7

 gboolean init_timeout();

void graph_refresh(GtkWidget *widget, gboolean);

void dec_refresh();

void inc_refresh();

void timeout_refresh();





#endif //DIPLOMSKI_MAIN_HEADER_H
