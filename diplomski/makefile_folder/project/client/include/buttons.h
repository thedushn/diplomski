//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_BUTTONS_H
#define DIPLOMSKI_BUTTONS_H


#include <gtk/gtk.h>
#include <stdbool.h>




GtkWidget *task_popup;


GtkWidget *button_inc;
GtkWidget *button_dec;
GtkWidget *button_proc;

GtkWidget *button_dev;
GtkWidget *button_graph;
GtkWidget *button_graph0;
GtkWidget *button_graph1;
GtkWidget *button_graph2;
GtkWidget *button_graph3;
GtkWidget *button_device_devices;
GtkWidget *button_device_directory;
GtkWidget *button_device_type;
GtkWidget *button_device_avail;
GtkWidget *button_device_used;
GtkWidget *button_device_all;
GtkWidget *button_device_free;
GtkWidget *button_device_total;
GtkWidget *button_process_task;
GtkWidget *button_process_user;
GtkWidget *button_process_pid;
GtkWidget *button_process_ppid;
GtkWidget *button_process_state;
GtkWidget *button_process_vm_size;
GtkWidget *button_process_rss;
GtkWidget *button_process_cpu;
GtkWidget *button_process_prio;
GtkWidget *button_process_duration;

static gboolean CPU0_line = TRUE;
static gboolean CPU1_line = TRUE;
static gboolean CPU2_line = TRUE;
static gboolean CPU3_line = TRUE;

gboolean on_treeview1_button_press_event(GtkButton *button, GdkEventButton *event);

GtkWidget *create_taskpopup(void);

void handle_task_menu(GtkWidget *widget, char *signal);

void handle_task_prio(GtkWidget *widget, char *signal);

void button_clicked_view_process(GtkWidget *widget);

void graph_button_clicked(GtkWidget *widget);

void close_window();

void show_hide(GtkWidget *button, GtkWidget *window);

void dev_button_clicked2(GtkWidget *widget);



void device_window();

void process_window();

void process_clicked(GtkWidget *widget);

void device_clicked(GtkWidget *widget);

void graph_clicked(GtkWidget *widget);

void start_stop(int show, char *signal, char *task_id);

void show_all(GtkWidget *widget);


#endif //DIPLOMSKI_BUTTONS_H
