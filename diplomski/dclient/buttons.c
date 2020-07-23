#include <sys/socket.h>
#include "buttons.h"
#include "drawing.h"
#include "testing_tree.h"
#include "main_header.h"
#include "buttons_s.h"

static gboolean process_task = TRUE;
static gboolean process_user = TRUE;
static gboolean process_prio = TRUE;
static gboolean process_pid = TRUE;
static gboolean process_ppid = TRUE;
static gboolean process_cpu = TRUE;
static gboolean process_vm_size = TRUE;
static gboolean process_rss = TRUE;
static gboolean process_state = TRUE;
static gboolean process_duration = TRUE;


static gboolean show_before = FALSE;

void process_window() {
    GtkWidget *box2;
    GtkWidget *proc_window;
    proc_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(proc_window), 200, 200);
    button_process_cpu = gtk_check_button_new_with_label("CPU");
    button_process_pid = gtk_check_button_new_with_label("PID");
    button_process_ppid = gtk_check_button_new_with_label("PPID");
    button_process_rss = gtk_check_button_new_with_label("RSS");
    button_process_state = gtk_check_button_new_with_label("State");
    button_process_task = gtk_check_button_new_with_label("Task");
    button_process_vm_size = gtk_check_button_new_with_label("VM_size");
    button_process_user = gtk_check_button_new_with_label("User");
    button_process_prio = gtk_check_button_new_with_label("Prio");

    button_process_duration = gtk_check_button_new_with_label("Duration");

    if (process_cpu == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_cpu), TRUE);
    }
    if (process_pid == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_pid), TRUE);
    }
    if (process_ppid == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_ppid), TRUE);
    }
    if (process_rss == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_rss), TRUE);
    }

    if (process_state == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_state), TRUE);
    }
    if (process_task == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_task), TRUE);
    }
    if (process_vm_size == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_vm_size), TRUE);
    }
    if (process_user == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_user), TRUE);
    }
    if (process_prio == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_prio), TRUE);
    }
    if (process_duration == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_duration), TRUE);
    }


    gtk_window_set_title(GTK_WINDOW (proc_window), "Process window");

    box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(proc_window), box2);
    gtk_box_pack_start(GTK_BOX(box2), button_process_task, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_process_user, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_process_pid, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_process_ppid, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_process_state, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_process_vm_size, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_process_rss, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_process_cpu, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_process_prio, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_process_duration, 1, 1, 0);


    g_signal_connect(button_process_user, "toggled", G_CALLBACK(process_clicked), NULL);
    g_signal_connect(button_process_rss, "toggled", G_CALLBACK(process_clicked), NULL);
    g_signal_connect(button_process_task, "toggled", G_CALLBACK(process_clicked), NULL);
    g_signal_connect(button_process_vm_size, "toggled", G_CALLBACK(process_clicked), NULL);
    g_signal_connect(button_process_state, "toggled", G_CALLBACK(process_clicked), NULL);
    g_signal_connect(button_process_cpu, "toggled", G_CALLBACK(process_clicked), NULL);
    g_signal_connect(button_process_pid, "toggled", G_CALLBACK(process_clicked), NULL);
    g_signal_connect(button_process_ppid, "toggled", G_CALLBACK(process_clicked), NULL);
    g_signal_connect(button_process_prio, "toggled", G_CALLBACK(process_clicked), NULL);
    g_signal_connect(button_process_duration, "toggled", G_CALLBACK(process_clicked), NULL);


    gtk_window_set_position(GTK_WINDOW(proc_window), GTK_WIN_POS_CENTER);

    g_signal_connect(G_OBJECT(proc_window), "destroy",
                     G_CALLBACK(close_window), NULL);

    gtk_widget_show_all(proc_window);

}

void device_window() {

    GtkWidget *box2;
    GtkWidget *dev_window;
    dev_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(dev_window), 200, 200);
    button_device_devices = gtk_check_button_new_with_label("Devices");
    button_device_directory = gtk_check_button_new_with_label("Directories");
    button_device_total = gtk_check_button_new_with_label("Total size");
    button_device_avail = gtk_check_button_new_with_label("available");
    button_device_type = gtk_check_button_new_with_label("type");
    button_device_free = gtk_check_button_new_with_label("free");
    button_device_used = gtk_check_button_new_with_label("used");
    button_device_all = gtk_check_button_new_with_label("Show_all");

    if (device_devices == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_devices), TRUE);
    }
    if (device_all == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_all), TRUE);
    }
    if (device_directory == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_directory), TRUE);
    }
    if (device_used == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_used), TRUE);
    }
    if (device_free == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_free), TRUE);
    }

    if (device_total == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_total), TRUE);
    }
    if (device_avail == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_avail), TRUE);
    }
    if (device_type == TRUE) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_type), TRUE);
    }


    gtk_window_set_title(GTK_WINDOW (dev_window), "Device window");

    box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(dev_window), box2);
    gtk_box_pack_start(GTK_BOX(box2), button_device_all, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_device_devices, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_device_directory, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_device_avail, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_device_total, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_device_free, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_device_used, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_device_type, 1, 1, 0);
    g_signal_connect(button_device_devices, "toggled", G_CALLBACK(device_clicked), NULL);
    g_signal_connect(button_device_all, "toggled", G_CALLBACK(show_all), NULL);
    g_signal_connect(button_device_directory, "toggled", G_CALLBACK(device_clicked), NULL);
    g_signal_connect(button_device_avail, "toggled", G_CALLBACK(device_clicked), NULL);
    g_signal_connect(button_device_total, "toggled", G_CALLBACK(device_clicked), NULL);
    g_signal_connect(button_device_free, "toggled", G_CALLBACK(device_clicked), NULL);
    g_signal_connect(button_device_used, "toggled", G_CALLBACK(device_clicked), NULL);
    g_signal_connect(button_device_type, "toggled", G_CALLBACK(device_clicked), NULL);


    gtk_window_set_position(GTK_WINDOW(dev_window), GTK_WIN_POS_CENTER);

    g_signal_connect(G_OBJECT(dev_window), "destroy",
                     G_CALLBACK(close_window), NULL);

    gtk_widget_show_all(dev_window);


}



void button_clicked_view_process(GtkWidget *widget) {


    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget))) {

        //  pokazi_ili_hide(widget, process_swindow);


        gtk_widget_show_all(process_swindow);


    } else {


        pokazi_ili_hide(widget, process_swindow);

    }
}

void dev_button_clicked2(GtkWidget *widget) {


    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget))) {


        pokazi_ili_hide(widget, dev_swindow);


        gtk_widget_show_all(dev_swindow);

    } else {
        pokazi_ili_hide(widget, dev_swindow);

    }

}


void close_window() {


    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_graph),
                                 FALSE);


};

void start_stop(int show, char *signal, char *task_id) {
    int ret;

    Commands commands = {0};


    if (show == 1) {

        show_before = !show_before;
    }

    commands.mem = 0;
    commands.show = device_all;
    if (signal != NULL && task_id != NULL) {
        for (int i = 0; i < sizeof(signal); i++) {

            commands.command[i] = signal[i];
        }
        for (int i = 0; i < sizeof(task_id); i++) {

            commands.task_id[i] = task_id[i];
        }
    }

    ret = (int) send(newsockfd1, &commands, sizeof(Commands), 0);
    if (ret < 0) {

        printf("nije uspelo slanje cond \n");
        gtk_main_quit();


    }
    if (ret == 0) {

        printf("nije uspelo slanje cond \n");
        printf("socket closed\n");
        gtk_main_quit();


    }

}

void graph_button_clicked(GtkWidget *widget) {

    GtkWidget *box2;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget))) {
        window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);

        button_graph0 = gtk_toggle_button_new_with_label("CPu0");
        button_graph1 = gtk_toggle_button_new_with_label("CPu1");
        button_graph2 = gtk_toggle_button_new_with_label("CPu2");
        button_graph3 = gtk_toggle_button_new_with_label("CPu3");

        if (CPU0_line == TRUE) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_graph0), TRUE);
        }
        if (CPU1_line == TRUE) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_graph1), TRUE);
        }
        if (CPU2_line == TRUE) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_graph2), TRUE);
        }
        if (CPU3_line == TRUE) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_graph3), TRUE);
        }

        gtk_window_set_title(GTK_WINDOW (window2), "GRAPH buttons");

        box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add(GTK_CONTAINER(window2), box2);
        gtk_box_pack_start(GTK_BOX(box2), button_graph0, 1, 1, 0);
        gtk_box_pack_start(GTK_BOX(box2), button_graph1, 1, 1, 0);
        gtk_box_pack_start(GTK_BOX(box2), button_graph2, 1, 1, 0);
        gtk_box_pack_start(GTK_BOX(box2), button_graph3, 1, 1, 0);
        g_signal_connect(button_graph0, "toggled", G_CALLBACK(graph_clicked), NULL);
        g_signal_connect(button_graph1, "toggled", G_CALLBACK(graph_clicked), NULL);
        g_signal_connect(button_graph2, "toggled", G_CALLBACK(graph_clicked), NULL);
        g_signal_connect(button_graph3, "toggled", G_CALLBACK(graph_clicked), NULL);


        gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);

        g_signal_connect(G_OBJECT(window2), "destroy",
                         G_CALLBACK(close_window), NULL);

        gtk_widget_show_all(window2);

    } else {

        gtk_widget_destroy(window2);

    }
};

void show_all(GtkWidget *widget) {

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget))) {

        device_all = TRUE;
        start_stop(1, "", "");

    } else {
        device_all = FALSE;
        start_stop(1, "", "");
    }


    timeout_refresh();


};

void device_clicked(GtkWidget *widget) {

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget))) {
        if (widget == button_device_devices) {

            device_devices = TRUE;

            change_list_store_view_devices(widget, device_devices);

        } else if (widget == button_device_directory) {

            device_directory = TRUE;
            change_list_store_view_devices(widget, device_directory);

        } else if (widget == button_device_avail) {

            device_avail = TRUE;
            change_list_store_view_devices(widget, device_avail);

        } else if (widget == button_device_total) {
            device_total = TRUE;
            change_list_store_view_devices(widget, device_total);

        } else if (widget == button_device_used) {
            device_used = TRUE;
            change_list_store_view_devices(widget, device_used);

        } else if (widget == button_device_type) {
            device_type = TRUE;
            change_list_store_view_devices(widget, device_type);

        } else {
            device_free = TRUE;
            change_list_store_view_devices(widget, device_free);

        }

    } else {

        if (widget == button_device_devices) {

            device_devices = FALSE;

            change_list_store_view_devices(widget, device_devices);


        } else if (widget == button_device_directory) {

            device_directory = FALSE;
            change_list_store_view_devices(widget, device_directory);

        } else if (widget == button_device_avail) {

            device_avail = FALSE;
            change_list_store_view_devices(widget, device_avail);

        } else if (widget == button_device_total) {
            device_total = FALSE;
            change_list_store_view_devices(widget, device_total);

        } else if (widget == button_device_used) {
            device_used = FALSE;
            change_list_store_view_devices(widget, device_used);

        } else if (widget == button_device_type) {
            device_type = FALSE;
            change_list_store_view_devices(widget, device_type);

        } else {
            device_free = FALSE;
            change_list_store_view_devices(widget, device_free);

        }


    }

};

void process_clicked(GtkWidget *widget) {


    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget))) {
        if (widget == button_process_task) {

            process_task = TRUE;

            change_list_store_view_process(widget, process_task);

        } else if (widget == button_process_cpu) {

            process_cpu = TRUE;

            change_list_store_view_process(widget, process_cpu);

        } else if (widget == button_process_prio) {

            process_prio = TRUE;
            change_list_store_view_process(widget, process_prio);

        } else if (widget == button_process_pid) {

            process_pid = TRUE;
            change_list_store_view_process(widget, process_pid);

        } else if (widget == button_process_ppid) {
            process_ppid = TRUE;
            change_list_store_view_process(widget, process_ppid);

        } else if (widget == button_process_rss) {
            process_rss = TRUE;
            change_list_store_view_process(widget, process_rss);

        } else if (widget == button_process_vm_size) {
            process_vm_size = TRUE;
            change_list_store_view_process(widget, process_vm_size);

        } else if (widget == button_process_state) {
            process_state = TRUE;
            change_list_store_view_process(widget, process_state);

        } else if (widget == button_process_duration) {
            process_duration = TRUE;
            change_list_store_view_process(widget, process_duration);
        } else {

            process_user = TRUE;
            change_list_store_view_process(widget, process_user);

        }

    } else {

        if (widget == button_process_task) {

            process_task = FALSE;

            change_list_store_view_process(widget, process_task);

        } else if (widget == button_process_cpu) {

            process_cpu = FALSE;
            change_list_store_view_process(widget, process_cpu);

        } else if (widget == button_process_prio) {

            process_prio = FALSE;
            change_list_store_view_process(widget, process_prio);

        } else if (widget == button_process_pid) {

            process_pid = FALSE;
            change_list_store_view_process(widget, process_pid);

        } else if (widget == button_process_ppid) {
            process_ppid = FALSE;
            change_list_store_view_process(widget, process_ppid);

        } else if (widget == button_process_rss) {
            process_rss = FALSE;
            change_list_store_view_process(widget, process_rss);

        } else if (widget == button_process_vm_size) {
            process_vm_size = FALSE;
            change_list_store_view_process(widget, process_vm_size);

        } else if (widget == button_process_state) {
            process_state = FALSE;
            change_list_store_view_process(widget, process_state);

        } else if (widget == button_process_duration) {
            process_duration = FALSE;

            change_list_store_view_process(widget, process_duration);
        } else {

            process_user = FALSE;
            change_list_store_view_process(widget, process_user);

        }


    }

};

void graph_clicked(GtkWidget *widget) {


    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget))) {
        if (widget == button_graph0) {

            CPU0_line = TRUE;
            graph_refresh(widget, CPU0_line);


        } else if (widget == button_graph1) {

            CPU1_line = TRUE;
            graph_refresh(widget, CPU1_line);

        } else if (widget == button_graph2) {

            CPU2_line = TRUE;
            graph_refresh(widget, CPU2_line);

        } else {

            CPU3_line = TRUE;
            graph_refresh(widget, CPU3_line);

        }


    } else {

        if (widget == button_graph0) {

            CPU0_line = FALSE;
            graph_refresh(widget, CPU0_line);

        } else if (widget == button_graph1) {

            CPU1_line = FALSE;
            graph_refresh(widget, CPU1_line);

        } else if (widget == button_graph2) {

            CPU2_line = FALSE;
            graph_refresh(widget, CPU2_line);

        } else {

            CPU3_line = FALSE;
            graph_refresh(widget, CPU3_line);

        }

    }

};



void pokazi_ili_hide(GtkWidget *button, GtkWidget *window) {


    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (button)) != FALSE) {//ako je button toggled



        gtk_widget_show(window);
//        if (window == dev_swindow) {
//
//            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_proc), FALSE); //postavljamo butto_proc na not toggled
//            gtk_widget_hide(process_swindow);
//        } else {
//
//            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_dev), FALSE); //postavljamo butto_proc na not toggled
//            gtk_widget_hide(dev_swindow);
//        }

    } else {

        gtk_widget_hide(window);

    }


};


void handle_task_menu(GtkWidget *widget, char *signal) {

    if (signal != NULL) {


        if (strcmp(signal, "STOP") == 0 || strcmp(signal, "CONT") == 0 || strcmp(signal, "KILL") == 0 ||
            strcmp(signal, "TERM") == 0) {
            char *task_id = "";
            GtkTreeModel *model;
            GtkTreeIter iter;

            if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
                gtk_tree_model_get(model, &iter, 1, &task_id, -1);
                start_stop(0, signal, task_id);
                init_timeout();
            }
        }
    }
}

void handle_task_prio(GtkWidget *widget, char *signal) {
    if (signal != NULL) {


        if (strcmp(signal, "VERY_LOW") == 0 || strcmp(signal, "VERY_HIGH") == 0 || strcmp(signal, "NORMAL") == 0
            || strcmp(signal, "LOW") == 0 || strcmp(signal, "HIGH") == 0) {
            char *task_id = "";
            GtkTreeModel *model;
            GtkTreeIter iter;

            if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
                gtk_tree_model_get(model, &iter, 1, &task_id, -1);
                start_stop(0, signal, task_id);
                init_timeout();
            }
        }
    }
}

GtkWidget *create_taskpopup(void) {
    GtkWidget *taskpopup;


    GtkWidget *menu_item;

    GtkWidget *menu_priority;


    taskpopup = gtk_menu_new();

    menu_item = gtk_menu_item_new_with_mnemonic(("Stop"));
    gtk_widget_show(menu_item);
    gtk_container_add(GTK_CONTAINER (taskpopup), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK(handle_task_menu), "STOP");

    menu_item = gtk_menu_item_new_with_mnemonic(("Continue"));
    gtk_widget_show(menu_item);
    gtk_container_add(GTK_CONTAINER (taskpopup), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK(handle_task_menu), "CONT");

    menu_item = gtk_menu_item_new_with_mnemonic(("Term"));
    gtk_widget_show(menu_item);
    gtk_container_add(GTK_CONTAINER (taskpopup), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK(handle_task_menu), "TERM");

    menu_item = gtk_menu_item_new_with_mnemonic(("Kill"));
    gtk_widget_show(menu_item);
    gtk_container_add(GTK_CONTAINER (taskpopup), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK(handle_task_menu), "KILL");

    menu_priority = gtk_menu_new();

    menu_item = gtk_menu_item_new_with_label(("Very low"));

    gtk_container_add(GTK_CONTAINER (menu_priority), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK(handle_task_prio), "VERY_LOW");

    menu_item = gtk_menu_item_new_with_label(("Low"));

    gtk_container_add(GTK_CONTAINER (menu_priority), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK(handle_task_prio), "LOW");

    menu_item = gtk_menu_item_new_with_label(("Normal"));

    gtk_container_add(GTK_CONTAINER (menu_priority), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK(handle_task_prio), "NORMAL");

    menu_item = gtk_menu_item_new_with_label(("High"));

    gtk_container_add(GTK_CONTAINER (menu_priority), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK(handle_task_prio), "HIGH");

    menu_item = gtk_menu_item_new_with_label(("Very high"));

    gtk_container_add(GTK_CONTAINER (menu_priority), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK(handle_task_prio), "VERY_HIGH");

    menu_item = gtk_menu_item_new_with_label(("Priority"));
    gtk_menu_item_set_submenu(GTK_MENU_ITEM (menu_item), menu_priority);
    gtk_container_add(GTK_CONTAINER (taskpopup), menu_item);


    gtk_widget_show_all(taskpopup);

    return taskpopup;
}

gboolean on_treeview1_button_press_event(GtkButton *button, GdkEventButton *event) {
    if (event->button == 3) {

        GdkEventButton *mouseevent = event;

        if (taskpopup == NULL)
            taskpopup = create_taskpopup();
        gtk_menu_popup(GTK_MENU(taskpopup), NULL, NULL, NULL, NULL, mouseevent->button, mouseevent->time);

    }
    return FALSE;
}