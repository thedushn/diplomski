//
// Created by dushn on 23.7.20..
//

#include "testing_tree.h"
#include <errno.h>


#include "buttons.h"
#include "main_header.h"

int device_check(D_Collection *devices_new, int dev_num) {

    __int32_t i, j;

    D_Collection *rem_old = devices_old;
    D_Collection *rem_new = devices_new;
    //refreshing data
    for (i = 0; i < dev_num_old; i++)
    {
        Devices *tmp = &devices_old->devices;

        tmp->checked = FALSE;

        for (j = 0; j < dev_num; j++) {
            Devices *new_tmp = &devices_new->devices;

            if (strcmp(new_tmp->directory, tmp->directory) == 0
                && strcmp(new_tmp->name, tmp->name) == 0
                && strcmp(new_tmp->type, tmp->type) == 0
                && new_tmp->fid == tmp->fid
                && new_tmp->total == tmp->total
                    )  //comparing elements of the array
            {


                if ((gint) tmp->avail != (gint) new_tmp->avail //if there is a difference
                    || tmp->used != new_tmp->used ||

                    (unsigned int) tmp->used != (unsigned int) new_tmp->used ||
                    (unsigned int) tmp->free != (unsigned int) new_tmp->free ||
                    (unsigned int) tmp->total != (unsigned int) new_tmp->total) {
                    tmp->avail = new_tmp->avail;

                    tmp->used = new_tmp->used;
                    tmp->total = new_tmp->total;
                    tmp->free = new_tmp->free;

                    refresh_list_item_device(tmp);

                }
                tmp->checked = TRUE; //
                new_tmp->checked = TRUE;

                break;


            } else
                tmp->checked = FALSE;

            devices_new = devices_new->next;
        }
        devices_new = rem_new;
        devices_old = devices_old->next;

    }

    devices_old = rem_old;

    devices_new = rem_new;
    //  check for unchecked old-devices for deleting
    i = 0;
    rem_old = devices_old;
    while (i < dev_num_old) {
        if (devices_old == NULL) {
            break;
        }
        Devices *tmp = &devices_old->devices;
        D_Collection *dtemp;

        if (!tmp->checked)//element of the array that does not exist in the new array anymore
        {
            remove_list_item_device(tmp->directory, tmp->name);
            if (i == 0) {
                dtemp = devices_old;
                if (devices_old->next != NULL) {
                    devices_old = devices_old->next;
                    devices_old->prev=NULL;
                }


                free(dtemp);
                rem_old = devices_old; //setting the first node
            } else {

                dtemp=devices_old; // the one that we don't want anymore
                devices_old->next->prev=devices_old->prev; //remember the last prev
                devices_old->prev->next=devices_old->next;
                free(dtemp);
                devices_old = rem_old;
            }


            dev_num_old--;


        } else {


            i++;
            devices_old = devices_old->next;


        }

    }
    devices_old = rem_old;


    //  check for unchecked new devices for inserting



    for (i = 0; i < dev_num; i++) {
        D_Collection *new_tmp;

        if (!devices_new->devices.checked) {
            new_tmp = calloc(1, sizeof(D_Collection));
            if (new_tmp == NULL) {

                printf("calloc error %d \n", errno);
                free(new_tmp);


                return -1;
            }
            ///doubly linked
            new_tmp->devices = devices_new->devices;
            new_tmp->next = devices_old;
            new_tmp->prev = NULL;
            if(devices_old!=NULL)
                devices_old->prev=new_tmp;
            devices_old = new_tmp;
            ///doubly linked

            add_new_list_item_dev(&devices_old->devices);
            dev_num_old++;
        }

        devices_new = devices_new->next;
    }


    /*devices */
    return 0;

}

int task_check(T_Collection *tasks_new, int task_num) {
    __int32_t i, j;
    /*tasks */
    T_Collection *rem_task_old = tasks_old;
    T_Collection *rem_task_new = tasks_new;

    for (i = 0; i < task_num_old; i++) {

        Task *tmp = &tasks_old->task;

        tmp->checked = FALSE;

        for (j = 0; j < task_num; j++) {
            Task *new_tmp = &tasks_new->task;


            float cpu_user_tmp;
            float cpu_system_tmp;
            float cpu_user_tmp_new;
            float cpu_system_tmp_new;

            cpu_system_tmp = (float) strtod(tmp->cpu_system, NULL);

            cpu_system_tmp_new = (float) strtod(new_tmp->cpu_system, NULL);

            cpu_user_tmp = (float) strtod(tmp->cpu_user, NULL);

            cpu_user_tmp_new = (float) strtod(new_tmp->cpu_system, NULL);


            if (new_tmp->pid == tmp->pid) {

                if ((gint) tmp->ppid != (gint) new_tmp->ppid || strcmp(tmp->state, new_tmp->state) != 0
                    || cpu_system_tmp != cpu_system_tmp_new
                    || cpu_user_tmp != cpu_user_tmp_new
                    || (unsigned int) tmp->rss != (unsigned int) new_tmp->rss
                    || (unsigned int) tmp->prio != (unsigned int) new_tmp->prio
                    || tmp->duration.tm_hour != new_tmp->duration.tm_hour
                    || tmp->duration.tm_min != new_tmp->duration.tm_min
                    || tmp->duration.tm_sec != new_tmp->duration.tm_sec
                        ) {
                    tmp->ppid = new_tmp->ppid;
                    strcpy(tmp->state, new_tmp->state);

                    memset(tmp->cpu_system, 0, sizeof(tmp->cpu_system));
                    memset(tmp->cpu_user, 0, sizeof(tmp->cpu_user));
                    sprintf(tmp->cpu_user, "%f", cpu_user_tmp_new);
                    sprintf(tmp->cpu_system, "%f", cpu_system_tmp_new);

                    memset(tmp->cpu_system, 0, sizeof(tmp->cpu_system));
                    memset(tmp->cpu_user, 0, sizeof(tmp->cpu_user));
                    strcpy(tmp->cpu_user, new_tmp->cpu_user);
                    strcpy(tmp->cpu_system, new_tmp->cpu_system);


                    tmp->rss = new_tmp->rss;
                    tmp->prio = new_tmp->prio;
                    tmp->duration.tm_hour = new_tmp->duration.tm_hour;
                    tmp->duration.tm_min = new_tmp->duration.tm_min;
                    tmp->duration.tm_sec = new_tmp->duration.tm_sec;

                    refresh_list_item(tmp);
                }

                tmp->checked = TRUE;

                new_tmp->checked = TRUE;


                break;

            } else
                tmp->checked = FALSE;

            tasks_new=tasks_new->next;

        }
        tasks_new = rem_task_new;
        tasks_old = tasks_old->next;

    }

    tasks_old = rem_task_old;

    tasks_new = rem_task_new;
    //  check for unchecked old-tasks for deleting
    i = 0;
    rem_task_old = tasks_old;
    while (i < task_num_old) {

        Task *tmp = &tasks_old->task;
        T_Collection *t_temp;
        if (!tmp->checked) {

            remove_list_item((gint) tmp->pid);
            if (i == 0) {
                t_temp = tasks_old;
                if (tasks_old->next != NULL) {
                    tasks_old = tasks_old->next;
                    tasks_old->prev=NULL;
                }




                free(t_temp);
                rem_task_old = tasks_old;//becomes new head
            } else {

                t_temp=tasks_old; // the one that we don't want anymore
                tasks_old->next->prev=tasks_old->prev; //remember the last prev
                tasks_old->prev->next=tasks_old->next;
                free(t_temp);
                tasks_old = rem_task_old;


            }


            task_num_old--;
        } else {
            i++;

            tasks_old = tasks_old->next;

        }

    }

    tasks_old = rem_task_old;

    //  check for unchecked new tasks for inserting



    for (i = 0; i < task_num; i++) {
        T_Collection *new_tmp;


        if (!tasks_new->task.checked) {
            new_tmp = calloc(1, sizeof(T_Collection));
            if (new_tmp == NULL) {

                printf("calloc error %d \n", errno);
                free(new_tmp);


                return -1;

            }

            new_tmp->task = tasks_new->task;
            new_tmp->next = tasks_old;
            new_tmp->prev = NULL;
            if(tasks_old!=NULL){
                tasks_old->prev=new_tmp;
            }

            tasks_old = new_tmp;


            add_new_list_item(&tasks_old->task);
            task_num_old++;
        }

        tasks_new=tasks_new->next;

    }

    return 0;
}

void create_list_store(void) {
    GtkCellRenderer *cell_renderer;
    GtkTreeViewColumn *column;

    list_store = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                    G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    cell_renderer = gtk_cell_renderer_text_new();


    column = gtk_tree_view_column_new_with_attributes(("Tasks"), cell_renderer, "text", COL_TASK, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_TASK);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COL_TASK, compare_string_list_item,
                                    (void *) COL_TASK, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(("USERS"), cell_renderer, "text", COL_UNAME, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_UNAME);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COL_UNAME, compare_string_list_item,
                                    (void *) COL_UNAME, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(("PID"), cell_renderer, "text", COL_PID, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_PID);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COL_PID, compare_int_list_item, (void *) COL_PID,
                                    NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(("PPID"), cell_renderer, "text", COL_PPID, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_PPID);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COL_PPID, compare_int_list_item, (void *) COL_PPID,
                                    NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(("State"), cell_renderer, "text", COL_STATE, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_STATE);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COL_STATE, compare_string_list_item,
                                    (void *) COL_STATE, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(("VM-Size"), cell_renderer, "text", COL_VSZ, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_VSZ);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COL_VSZ, compare_int_list_item_size,
                                    (void *) COL_VSZ, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(("RSS"), cell_renderer, "text", COL_RSS, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_RSS);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COL_RSS, compare_int_list_item_size,
                                    (void *) COL_RSS, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);


    column = gtk_tree_view_column_new_with_attributes(("CPU%"), cell_renderer, "text", COL_CPU, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_CPU);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COL_CPU, compare_int_list_item, (void *) COL_CPU,
                                    NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(("Prio"), cell_renderer, "text", COL_PRIO, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_PRIO);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COL_PRIO, compare_int_list_item, (void *) COL_PRIO,
                                    NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(("Duration"), cell_renderer, "text", COL_DUR, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_DUR);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COL_DUR, compare_int_list_item_time,
                                    (void *) COL_DUR, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);


}

void create_list_store_dev(void) {
    GtkCellRenderer *cell_renderer;
    GtkTreeViewColumn *column;

    list_store1 = gtk_tree_store_new(NUM_COLS_DEV, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                     G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    cell_renderer = gtk_cell_renderer_text_new();


    column = gtk_tree_view_column_new_with_attributes(("Devices"), cell_renderer, "text", COL_DEV, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_DEV);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store1), COL_DEV, compare_string_list_item, (void *) COL_DEV,
                                    NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), column);

    column = gtk_tree_view_column_new_with_attributes(("Available"), cell_renderer, "text", COL_AVAILABLE, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_AVAILABLE);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store1), COL_AVAILABLE, compare_int_list_item_size,
                                    (void *) COL_AVAILABLE, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), column);

    column = gtk_tree_view_column_new_with_attributes(("Used"), cell_renderer, "text", COL_USED, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_USED);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store1), COL_USED, compare_int_list_item_size,
                                    (void *) COL_USED, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), column);

    column = gtk_tree_view_column_new_with_attributes(("Type"), cell_renderer, "text", COL_TYPE, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_TYPE);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store1), COL_TYPE, compare_string_list_item,
                                    (void *) COL_TYPE, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), column);

    column = gtk_tree_view_column_new_with_attributes(("Directory"), cell_renderer, "text", COL_DIR, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_DIR);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store1), COL_DIR, compare_string_list_item, (void *) COL_DIR,
                                    NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), column);

    column = gtk_tree_view_column_new_with_attributes(("Total"), cell_renderer, "text", COL_TOTAL, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_TOTAL);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store1), COL_TOTAL, compare_int_list_item_size,
                                    (void *) COL_TOTAL, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), column);

    column = gtk_tree_view_column_new_with_attributes(("Free"), cell_renderer, "text", COL_FREE, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COL_FREE);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store1), COL_FREE, compare_int_list_item_size,
                                    (void *) COL_FREE, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), column);


}

void add_new_list_item(Task *task_t) {
    GtkTreeIter iter;

    gtk_tree_store_append(GTK_TREE_STORE(list_store), &iter, NULL);

    fill_list_item(task_t, &iter);
}

void add_new_list_item_dev(Devices *d_temp) {
    GtkTreeIter iter;

    gtk_tree_store_append(GTK_TREE_STORE(list_store1), &iter, NULL);

    fill_list_item_device(d_temp, &iter);
}

void change_list_store_view_devices(GtkWidget *widget, gboolean visible) {

    if (button_device_devices == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview1), 0), visible);
    } else if (button_device_avail == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview1), 1), visible);
    } else if (button_device_used == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview1), 2), visible);
    } else if (button_device_type == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview1), 3), visible);
    } else if (button_device_directory == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview1), 4), visible);
    } else if (button_device_total == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview1), 5), visible);
    } else {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview1), 6), visible);
    }

}

void change_list_store_view_process(GtkWidget *widget, gboolean visible) {

    if (button_process_task == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0), visible);
    } else if (button_process_user == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 1), visible);
    } else if (button_process_pid == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 2), visible);
    } else if (button_process_ppid == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 3), visible);
    } else if (button_process_state == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 4), visible);
    } else if (button_process_vm_size == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 5), visible);
    } else if (button_process_rss == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 6), visible);
    } else if (button_process_cpu == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 7), visible);
    } else if (button_process_prio == widget) {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 8), visible);

    } else {

        gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 9), visible);
    }


}


void fill_list_item(Task *task_item, GtkTreeIter *iter) {

    if (iter != NULL) {
        Task *task = task_item;
        gchar cpu[16], value[16];
        char *rss, *vsz;
        char *prio;

        char *duration;
        float cpu_user = 0;
        float cpu_system = 0;
        cpu_user = (float) strtod(task->cpu_user,NULL);
        cpu_system = (float) strtod(task->cpu_system,NULL);

        rss = g_format_size_full(task->rss, G_FORMAT_SIZE_IEC_UNITS);
        vsz = g_format_size_full(task->vsz, G_FORMAT_SIZE_IEC_UNITS);

        g_snprintf(value, 14, "%.f", cpu_user + cpu_system);

        g_snprintf(cpu, 16, ("%s%%"), value);
        char *pid = g_strdup_printf("%i", task->pid);
        char *ppid = g_strdup_printf("%i", task->ppid);
        char *state = g_strdup_printf("%s", task->state);

        char *name = g_strdup_printf("%s", task->name);
        char *uname = g_strdup_printf("%s", task->uid_name);
        prio = g_strdup_printf("%hi", task->prio);


        duration = g_strdup_printf("%d:%d:%d", task->duration.tm_hour, task->duration.tm_min, task->duration.tm_sec);


        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COL_TASK, name, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COL_PID, pid, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COL_PPID, ppid, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COL_STATE, state, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COL_VSZ, vsz, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COL_RSS, rss, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COL_CPU, cpu, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COL_UNAME, uname, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COL_PRIO, prio, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COL_DUR, duration, -1);


        free(pid);
        free(ppid);
        free(state);
        free(name);
        free(uname);
        free(rss);
        free(vsz);
        free(prio);

        free(duration);
    }
}


void fill_list_item_device(Devices *f_temp, GtkTreeIter *iter) {


    gchar *total, *avail, *used, *free, *directory, *type;

    if (iter != NULL) {


        Devices *device_temp =f_temp;
        gchar *name = g_strdup_printf("%s", device_temp->name);
        used = g_format_size_full((guint64) device_temp->used, G_FORMAT_SIZE_IEC_UNITS);
        total = g_format_size_full((guint64) device_temp->total, G_FORMAT_SIZE_IEC_UNITS);
        avail = g_format_size_full((guint64) device_temp->avail, G_FORMAT_SIZE_IEC_UNITS);
        free = g_format_size_full((guint64) device_temp->free, G_FORMAT_SIZE_IEC_UNITS);
        directory = g_strdup_printf("%s", device_temp->directory);
        type = g_strdup_printf("%s", device_temp->type);


        gtk_tree_store_set(GTK_TREE_STORE(list_store1), iter, COL_DEV, name, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store1), iter, COL_AVAILABLE, avail, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store1), iter, COL_USED, used, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store1), iter, COL_FREE, free, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store1), iter, COL_TOTAL, total, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store1), iter, COL_DIR, directory, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store1), iter, COL_TYPE, type, -1);


        g_free(name);
        g_free(used);
        g_free(total);
        g_free(avail);
        g_free(free);
        g_free(directory);
        g_free(type);

    }
}


void refresh_list_item_device(Devices *ref_temp) {
    GtkTreeIter iter;
    static gint g = 0;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list_store1), &iter);


    Devices *device =ref_temp ;
    while (valid) {
        gchar *str_data = "";
        gchar *str_data1 = "";
        gchar *str_data2 = "";
        gtk_tree_model_get(GTK_TREE_MODEL(list_store1), &iter, 0, &str_data, -1); //COL_DEV
        gtk_tree_model_get(GTK_TREE_MODEL(list_store1), &iter, 1, &str_data1, -1); //COL_DIR
        gtk_tree_model_get(GTK_TREE_MODEL(list_store1), &iter, 2, &str_data2, -1);  //type


        if (strcmp(device->name, str_data) == 0 &&
            strcmp(device->directory, str_data1) == 0 /* && strcmp(device->type , str_data2)*/) {
            g_free(str_data);
            g_free(str_data1);
            g_free(str_data2);

            fill_list_item_device(ref_temp, &iter);
            g++;
            break;
        }

        g_free(str_data);
        g_free(str_data1);
        g_free(str_data2);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(list_store1), &iter);
    }
}

void refresh_list_item(Task *task_item) {
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list_store), &iter);
    Task *task = task_item;
    while (valid) {

        gchar *str_data = "";
        gtk_tree_model_get(GTK_TREE_MODEL(list_store), &iter, 1, &str_data, -1);

        if (task->pid == atoi(str_data)) {
            g_free(str_data);
            fill_list_item(task_item, &iter);
            break;
        }

        g_free(str_data);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(list_store), &iter);
    }
}

void remove_list_item(gint pid) {
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list_store), &iter);

    while (valid) {

        gchar *str_data = "";
        gtk_tree_model_get(GTK_TREE_MODEL(list_store), &iter, 1, &str_data, -1);

        if (pid == atoi(str_data)) {
            g_free(str_data);
            gtk_tree_store_remove(GTK_TREE_STORE(list_store), &iter);
            break;
        }

        g_free(str_data);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(list_store), &iter);
    }
}

void remove_list_item_device(gchar *directory, gchar *name) {
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list_store1), &iter);

    while (valid) {
        gchar *str_data = "";
        gchar *str_data1 = "";
        gchar *str_data2 = "";
        gtk_tree_model_get(GTK_TREE_MODEL(list_store1), &iter, 0, &str_data, -1); //COL_DEV
        gtk_tree_model_get(GTK_TREE_MODEL(list_store1), &iter, 1, &str_data1, -1); //COL_DIR
        gtk_tree_model_get(GTK_TREE_MODEL(list_store1), &iter, 1, &str_data2, -1); //COL_DIR

        if (strcmp(directory, str_data1) == 0 && strcmp(name, str_data) == 0 && strcmp(directory, str_data2) == 0) {
            g_free(str_data);
            g_free(str_data1);
            g_free(str_data2);

            gtk_tree_store_remove(GTK_TREE_STORE(list_store1), &iter);
            break;
        }

        g_free(str_data);
        g_free(str_data1);
        g_free(str_data2);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(list_store1), &iter);
    }
}

gint compare_string_list_item(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column) {
    char *s1;
    char *s2;

    gint ret = 0;


    gtk_tree_model_get(model, iter1, GPOINTER_TO_INT(column), &s1, -1);
    if (s1 == NULL) {

        return 1;//if there is no first, second is first
    }
    gtk_tree_model_get(model, iter2, GPOINTER_TO_INT(column), &s2, -1);

    if (s2 == NULL) {
        free(s1);

        return -1;//if there is no second, first is first
    }


    ret = strcasecmp(s1, s2);


    if (s1 != NULL) {
        g_free(s1);
    }

    if (s2 != NULL) {
        g_free(s2);
    }


    return ret;
}

gint compare_int_list_item(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column) {
    gchar *s1;
    gchar *s2;

    gint i1 = 0;
    gint i2 = 0;

    gint ret = 0;


    gtk_tree_model_get(model, iter1, column, &s1, -1);
    if (s1 == NULL) {

        return ret; //if there is no first, second is first
    }
    gtk_tree_model_get(model, iter2, column, &s2, -1);
    if (s2 == NULL) {
        g_free(s1);
        return ret;//if there is no second, first is first
    }


    if (s1 != NULL) {
        i1 = atoi(s1);
    }


    if (s2 != NULL) {
        i2 = atoi(s2);
    }


    ret = i1 - i2;

    if (s1 != NULL) {
        g_free(s1);
    }

    if (s2 != NULL) {
        g_free(s2);
    }


    return ret;
}

void test_strtol(int val) {

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
        || (errno != 0 && val == 0)) {
        perror("strtol");
        gtk_main_quit();
    }
};

gint compare_int_list_item_size(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column) {
    gchar *s1 = NULL;
    gchar *s2 = NULL;
    gchar *z;
    gchar *z1;
    gchar *size;
    gchar *size1;
    gchar *end;


    gint ret = 0;
    gint ret1 = 0;
    gint ret2 = 0;


    gtk_tree_model_get(model, iter1, column, &s1, -1);
    if (s1 == NULL) {

        return ret;//if there is no first, second is first
    }
    gtk_tree_model_get(model, iter2, column, &s2, -1);


    if (s2 == NULL) {
        g_free(s1);
        return ret;//if there is no second, first is first
    }

    z = g_strrstr(s1, ",");
    z1 = g_strrstr(s2, ",");
    size = g_strstr_len(s1, strlen(s1), " "); //first comma
    size1 = g_strstr_len(s2, strlen(s2), " ");
    size++;
    size1++;
    gint i1 = 0;
    gint i2 = 0;
    gint i3 = 0;
    gint i4 = 0;
    gint isize = 0;
    gint isize1 = 0;

    errno = 0;


    ret2 = strcmp(size, size1);
    if (ret2 != 0) {
        if (strcmp(size, "MiB") == 0) {

            isize = 3;

        }
        if (strcmp(size1, "MiB") == 0) {

            isize1 = 3;

        }
        if (strcmp(size, "KiB") == 0) {

            isize = 2;

        }
        if (strcmp(size1, "KiB") == 0) {

            isize1 = 2;

        }
        if (strcmp(size, "GiB") == 0) {

            isize = 4;

        }
        if (strcmp(size1, "GiB") == 0) {

            isize1 = 4;

        }
        if (strcmp(size, "bytes") == 0) {

            isize = 1;

        }

        if (strcmp(size1, "bytes") == 0) {

            isize1 = 1;

        }

        ret2 = isize - isize1;
        g_free(s1);
        g_free(s2);
        return ret2;
    }

    if (z != NULL) {
        //when they are the same size
        i3 = (int) strtol(z + 1, &end, 10);//jump over the comma
        if(errno!=0){
            test_strtol(i3);
        }
    }



    if (z1 != NULL) {
        i4 = (int) strtol(z1 + 1, &end, 10); //jump over the comma
        if(errno!=0){
            test_strtol(i4);
        }

    }



    if (s1 != NULL) {
        i1 = (int) strtol(s1, &end, 10);
        if(errno!=0) {
            test_strtol(i1);
        }
        g_free(s1);
    }



    if (s2 != NULL) {
        i2 = (int) strtol(s2, &end, 10);
        if(errno!=0) {
            test_strtol(i2);
        }
        g_free(s2);
    }



    ret = i1 - i2;
    ret1 = i3 - i4;
    if (ret == 0) { //when the value before the comma is equal

        return ret1;
    }


    return ret;
}

gint compare_int_list_item_time(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column) {
    gchar *s1 = "";
    gchar *s2 = "";
    gchar *dt;
    gchar *dt1;


    gint ret = 0;
    errno=0;

    gtk_tree_model_get(model, iter1, column, &s1, -1);
    if (s1 == NULL) {

        return ret;//if there is no first ,second is first
    }
    gtk_tree_model_get(model, iter2, column, &s2, -1);


    if (s2 == NULL) {
        g_free(s1);
        return ret;//if there is no second, first is first
    }


    gint i1 = 0;
    gint i2 = 0;


    i1 = atoi(s1); //how much time is it



    i2 = atoi(s2); //how much time is it


    ret = i1 - i2;
    if (ret == 0)  { //if the hours are   the same


        dt = g_strstr_len(s1, strlen(s1), ":");
        dt1 = g_strstr_len(s2, strlen(s2), ":");

        dt++;
        dt1++;

        i1 = atoi(dt);


        i2 = atoi(dt1);
        ret = i1 - i2;
    } else {
        g_free(s1);
        g_free(s2);
        return ret;
    }

    if (ret == 0) { //if the minutes are the same


        dt = strrchr(s1, ':');//seconds
        dt1 = strrchr(s2, ':');//seconds move pointer at :
        dt++;
        dt1++;

        i1 = atoi(dt);
        i2 = atoi(dt1);

        ret = i1 - i2;
    } else {
        g_free(s1);
        g_free(s2);
        return ret;

    }
    g_free(s1);
    g_free(s2);
    return ret;
}