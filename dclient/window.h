//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_WINDOW_H
#define DIPLOMSKI_WINDOW_H

#include "main_header.h"
#include "common.h"


GtkWidget *graph1;/*graph for cpu usage*/
GtkWidget *graph2;/*graph for network usage*/
GtkWidget *graph3;/*graph for memory usage*/
GtkWidget *graph4;/*graph for interrupts */






void cpu_change(Cpu_usage cpu_usage);

void memory_change(Memory_usage *memory_usage);

void swap_change(Memory_usage *memory_usage);

void network_change(Network *network);

GtkWidget *main_window(GtkWidget *des_swindow, GtkWidget *proc_swindow);





#endif //DIPLOMSKI_WINDOW_H
