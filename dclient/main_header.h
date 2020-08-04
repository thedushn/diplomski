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
GArray *task_array;
GArray *names_array;


Collection * collection;
D_Collection *devices;
D_Collection *devices_old;
Interrupts * interrupts;
#define LIST_SIZE 240

void init_timeout();

void graph_refresh(GtkWidget *widget, gboolean);

void dec_refresh();

void inc_refresh();

void timeout_refresh();

void freeing_memory(Unification array);



#endif //DIPLOMSKI_MAIN_HEADER_H
