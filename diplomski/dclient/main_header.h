//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_MAIN_HEADER_H
#define DIPLOMSKI_MAIN_HEADER_H

#include <gtk/gtk.h>


GtkWidget *window2;
GtkWidget *process_swindow;
GtkWidget *dev_swindow;

int newsockfd;
int newsockfd1;

GArray *history[8];
GArray *task_array;
GArray *names_array;
GArray *interrupt_array_d;


void init_timeout();

void graph_refresh(GtkWidget *widget, gboolean);

void dec_refresh();

void inc_refresh();

void timeout_refresh();


#endif //DIPLOMSKI_MAIN_HEADER_H
