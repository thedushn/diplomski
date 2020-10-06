//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_TESTING_TREE_H
#define DIPLOMSKI_TESTING_TREE_H


#include "string.h"
#include <gtk/gtk.h>
#include <stdlib.h>

#include "common.h"



GtkTreeSelection *selection; /*so we know which task we clicked*/


GtkWidget *treeview_tasks; /*!treeview for tasks*/
GtkWidget *treeview_devices;/*!treeview for devices*/

GtkTreeStore *list_tasks; /*liststore for tasks*/
GtkTreeStore *list_devices; /*liststore for devices*/


int task_check(T_Collection *tasks_new, int task_num);

int device_check(D_Collection *devices_new, int dev_num);

void remove_task_item(gint pid);

void remove_list_item_device(gchar *directory, gchar *name);

void refresh_list_item(Task *task_item);

void refresh_list_item_device(Devices *ref_temp);

int fill_task_item(Task *task_item, GtkTreeIter *iter);

int fill_device_item(Devices *f_temp, GtkTreeIter *iter);


void change_list_store_view_devices(GtkWidget *widget);

void change_list_store_view_process(GtkWidget *widget);

int add_new_task(Task *task_t);

int add_new_dev(Devices *devices);

void create_list_store_task(void);

gint compare_string_list_item(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column);

gint compare_int_list_item_size(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column);

gint compare_int_list_item_time(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column);

gint compare_int_list_item(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column);

void create_list_store_dev(void);



enum {/* task column id for sorting and for setting visibility*/
    COL_TASK = 0,
    COL_PID,
    COL_RSS,
    COL_CPU,
    COL_PRIO,
    COL_VSZ,
    COL_PPID,
    COL_STATE,
    COL_UNAME,
    COL_DUR,
    NUM_COLS
};

enum {/*device column id for sorting and for setting visibility*/

    COL_DEV = 0,
    COL_DIR,
    COL_TYPE,
    COL_TOTAL,
    COL_AVAILABLE,
    COL_USED,
    COL_FREE,
    NUM_COLS_DEV


};
#endif //DIPLOMSKI_TESTING_TREE_H
