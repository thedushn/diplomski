//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_WINDOW_H
#define DIPLOMSKI_WINDOW_H

#include "main_header.h"
#include "common.h"


GtkWidget *graphCpu;/*graph for cpu usage*/
GtkWidget *graphNet;/*graph for network usage*/
GtkWidget *graphMem;/*graph for memory usage*/
GtkWidget *graph_inttrp;/*graph for interrupts */
GtkWidget *graph_write;/*graph for interrupts */
GtkWidget *cpuGraphs;
GtkWidget *viewport;
GtkWidget *viewportcpu;
GtkAdjustment *adj;
GtkWidget *interrupts_swindow;
GtkWidget *cpu_swindow;



GtkWidget *label_rec;   /*GtkWidget where we input received data*/
GtkWidget *label_trans; /*GtkWidget where we input transmitted data*/
GtkWidget *label_cpu0;  /*GtkWidget where we input cpu data*/
GtkWidget *label_mem;   /*GtkWidget where we input memory data*/
GtkWidget *label_swap;  /*GtkWidget where we input swap memory data*/

GtkWidget* CPU_WINDOW;

void cpu_change(Cpu_usage *cpu_usage);

void memoryChange(Memory_usage *memoryUsage);

void swap_change(Memory_usage *memoryUsage);

void network_change(Network *network);

GtkWidget *main_window(GtkWidget *des_swindow, GtkWidget *proc_swindow);

GtkWidget *cpuWindow(int cpuNumber, GtkWidget **graphList);



#endif //DIPLOMSKI_WINDOW_H
