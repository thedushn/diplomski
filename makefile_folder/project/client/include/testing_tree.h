//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_TESTING_TREE_H
#define DIPLOMSKI_TESTING_TREE_H


#include "string.h"
#include <gtk/gtk.h>
#include <stdlib.h>

#include "common.h"

//GtkTreeStore *list_store1;

GtkTreeSelection *selection;


GtkWidget *treeview;
GtkWidget *treeview1;

GtkTreeStore *list_store;
GtkTreeStore *list_store1;


void remove_list_item(gint pid);

void remove_list_item_device(gchar *directory, gchar *name);

void refresh_list_item(Task *task_item);

void refresh_list_item_device(Devices *ref_temp);

void fill_list_item(Task *task_item, GtkTreeIter *iter);

void fill_list_item_device(Devices *f_temp, GtkTreeIter *iter);


void change_list_store_view_devices(GtkWidget *widget, gboolean visible);

void change_list_store_view_process(GtkWidget *widget, gboolean visible);

void add_new_list_item(Task *task_t);

void add_new_list_item_dev(Devices *d_temp);

void create_list_store(void);

gint compare_string_list_item(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column);

gint compare_int_list_item_size(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column);

gint compare_int_list_item_time(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column);

gint compare_int_list_item(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column);

void create_list_store_dev(void);

void test_strtol(int val);

enum {
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

enum {

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
