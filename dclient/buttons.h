//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_BUTTONS_H
#define DIPLOMSKI_BUTTONS_H


#include <gtk/gtk.h>



GtkWidget *dev_window;/*window that contains the list of devices */
GtkWidget *proc_window;/*window that contains the list of tasks */

GtkWidget *task_popup;

GtkWidget *button_inc;/*button that when pressed increments the time delay by 250 */
GtkWidget *button_dec;/*button that when pressed decreases the time delay by 250 */

GtkWidget *button_proc;/*button that when pressed shows or hides window that contains the list of tasks */
GtkWidget *button_dev;/*button that when pressed shows or hides window that contains the list of devices */
GtkWidget *button_graph;/*button that when pressed shows buttons that control the graph that displays the cpu usage */

/*buttons that when clicked show or hide columns in the window that contains the list of devices */
GtkWidget *button_device_devices;
GtkWidget *button_device_directory;
GtkWidget *button_device_type;
GtkWidget *button_device_avail;
GtkWidget *button_device_used;
GtkWidget *button_device_all;
GtkWidget *button_device_free;
GtkWidget *button_device_total;

/*buttons that when clicked show or hide a  specific column in the window that contains the list of tasks */
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

GtkWidget *cpu_buttons;/*array of buttons that when true let the graph display cpu usage */

gboolean on_treeview_tasks_button_press_event(GtkButton *button, GdkEventButton *event);

GtkWidget *create_taskpopup(void);

void handle_task_menu(GtkWidget *widget, char *signal);

void handle_task_prio(GtkWidget *widget, char *signal);

void graph_button_clicked(GtkWidget *widget);

void close_window_toggled();

void close_window(GtkWidget *widget);

void show_hide(GtkWidget *button, GtkWidget *window);

void device_window();

void process_window();

void graph_clicked(GtkWidget *widget);

void show_all(GtkWidget *widget);






#endif //DIPLOMSKI_BUTTONS_H
