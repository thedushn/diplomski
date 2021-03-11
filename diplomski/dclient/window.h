//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_WINDOW_H
#define DIPLOMSKI_WINDOW_H

#include "main_header.h"
#include "common.h"


GtkWidget *graph1;/*graph for cpu usage*/
GtkWidget *graph_net;/*graph for network usage*/
GtkWidget *graph_mem;/*graph for memory usage*/
GtkWidget *graph_inttrp;/*graph for interrupts */
GtkWidget *graph_write;/*graph for interrupts */

GtkWidget *viewport;
GtkAdjustment *adj;
GtkWidget *interrupts_swindow;



void cpu_change(Cpu_usage cpu_usage);

void memory_change(Memory_usage *memory_usage);

void swap_change(Memory_usage *memory_usage);

void network_change(Network *network);

GtkWidget *main_window(GtkWidget *des_swindow, GtkWidget *proc_swindow);





#endif //DIPLOMSKI_WINDOW_H
