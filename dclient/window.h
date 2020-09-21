//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_WINDOW_H
#define DIPLOMSKI_WINDOW_H

#include "main_header.h"
#include "common.h"


GtkWidget *graph1;
GtkWidget *graph2;
GtkWidget *graph3;
GtkWidget *graph4;




void quit_activated();

void cpu_change(Cpu_usage_list *cpu_usage_list);

void memory_change(Memory_usage *memory_usage);

void swap_change(Memory_usage *memory_usage);

void network_change_rc(Network *network);

GtkWidget *main_window(GtkWidget *des_swindow, GtkWidget *proc_swindow);





#endif //DIPLOMSKI_WINDOW_H
