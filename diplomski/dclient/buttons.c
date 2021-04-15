
#include "buttons.h"
#include "drawing.h"
#include "testing_tree.h"
#include "functions.h"


/**
 * process_window(): create a window with buttons that represent columns in the task list
 * when a button is checked the column connected to that button is shown or hidden. The buttons are checked depending on
 * the columns visibility
 *
 * @return void
 * */
void process_window() {

    GList               *array;
    GList               *temp;
    GtkTreeViewColumn   *column;
    gint                columnId;
    GtkWidget           *box2;


    if(proc_window !=NULL){
        if (proc_window->parent_instance.qdata != NULL) {
            if (gtk_widget_get_visible(proc_window)) {
                gtk_widget_destroy(proc_window);

            }
        }
    }


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



    array= gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview_tasks));
    temp=array;

    while(temp!=NULL) {

        column = GTK_TREE_VIEW_COLUMN(temp->data);
        columnId = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(column), "column-id"));
        switch (columnId) {

            case COL_CPU:

                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_cpu),
                                             gtk_tree_view_column_get_visible(column));

                break;
            case COL_PID:

                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_pid),
                                             gtk_tree_view_column_get_visible(column));

                break;
            case COL_PPID:

                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_ppid),
                                             gtk_tree_view_column_get_visible(column));

                break;
            case COL_RSS:

                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_rss),
                                             gtk_tree_view_column_get_visible(column));

                break;
            case COL_STATE:

                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_state),
                                             gtk_tree_view_column_get_visible(column));

                break;
            case COL_TASK:

                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_task),
                                             gtk_tree_view_column_get_visible(column));

                break;
            case COL_VSZ:

                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_vm_size),
                                             gtk_tree_view_column_get_visible(column));

                break;
            case COL_UNAME:

                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_user),
                                             gtk_tree_view_column_get_visible(column));

                break;
            case COL_PRIO:

                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_prio),
                                             gtk_tree_view_column_get_visible(column));

                break;
            case COL_DUR:

                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_process_duration),
                                             gtk_tree_view_column_get_visible(column));

                break;
            default:
                break;

        }
        temp=temp->next;
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


    g_signal_connect(button_process_user, "toggled", G_CALLBACK(change_list_store_view_process), NULL);
    g_signal_connect(button_process_rss, "toggled", G_CALLBACK(change_list_store_view_process), NULL);
    g_signal_connect(button_process_task, "toggled", G_CALLBACK(change_list_store_view_process), NULL);
    g_signal_connect(button_process_vm_size, "toggled", G_CALLBACK(change_list_store_view_process), NULL);
    g_signal_connect(button_process_state, "toggled", G_CALLBACK(change_list_store_view_process), NULL);
    g_signal_connect(button_process_cpu, "toggled", G_CALLBACK(change_list_store_view_process), NULL);
    g_signal_connect(button_process_pid, "toggled", G_CALLBACK(change_list_store_view_process), NULL);
    g_signal_connect(button_process_ppid, "toggled", G_CALLBACK(change_list_store_view_process), NULL);
    g_signal_connect(button_process_prio, "toggled", G_CALLBACK(change_list_store_view_process), NULL);
    g_signal_connect(button_process_duration, "toggled", G_CALLBACK(change_list_store_view_process), NULL);


    gtk_window_set_position(GTK_WINDOW(proc_window), GTK_WIN_POS_CENTER);

    g_signal_connect(G_OBJECT(proc_window), "destroy",
                     G_CALLBACK(close_window), proc_window);

    gtk_widget_show_all(proc_window);

    g_list_free(array);

}
//void write_window(){
//
//    GtkWidget *vbox;
//    GtkWidget *hbox;
//    GtkWidget *window_temp;
//    GtkWidget *grid;
//    GtkWidget *viewport;
//    GtkWidget *frame4;
//    if(wr_window !=NULL){
//        if ( wr_window->parent_instance.qdata != NULL) {
//            if (gtk_widget_get_visible(wr_window)) {
//                gtk_widget_destroy(wr_window);
//
//
//
//            }
//
//        }
//    }
//    wr_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//    gtk_window_set_position(GTK_WINDOW(wr_window), GTK_WIN_POS_CENTER);
//    gtk_window_set_default_size(GTK_WINDOW(wr_window), 200, 200);
//    window_temp = gtk_scrolled_window_new(NULL, NULL);
//    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(window_temp), GTK_POLICY_AUTOMATIC,
//                                   GTK_POLICY_ALWAYS);
//
//
//    graph_write      = gtk_drawing_area_new();
//
//
//    graph_write      = gtk_drawing_area_new();
//    frame4           = gtk_frame_new(NULL);
//    viewport=gtk_viewport_new(NULL,NULL);
//
//
//    gtk_window_set_title(GTK_WINDOW (wr_window), "write window");
//
//    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
//
//
//    gtk_container_add(GTK_CONTAINER(viewport),graph_write);
//   // gtk_container_add(GTK_CONTAINER(frame4), graph_write);
//
//   // gtk_box_pack_start(GTK_BOX(hbox), frame4, 1, TRUE, 0);
//
//    gtk_box_pack_start(GTK_BOX(hbox), window_temp, TRUE, TRUE, 1);
//
//    gtk_container_add(GTK_CONTAINER(window_temp),viewport);
//
//
//    gtk_container_add(GTK_CONTAINER(wr_window), hbox);
//    gtk_widget_set_hexpand(window_temp,TRUE);
//    gtk_widget_set_vexpand(window_temp,TRUE);
//
//
//
//    gtk_window_set_position(GTK_WINDOW(wr_window), GTK_WIN_POS_CENTER);
//
//    g_signal_connect(G_OBJECT(wr_window), "destroy",
//                     G_CALLBACK(close_window), wr_window);
//    g_signal_connect ((GObject *)(wr_window), "destroy",
//                      G_CALLBACK(gtk_main_quit), NULL);
//
//    gtk_widget_show_all(wr_window);
//
//
//
//
//}
/** record_window() creates a window that holds the record button
 *
 * */
void recordWindow(){

    GtkWidget *box2;
    if(rec_window !=NULL){
        if (rec_window->parent_instance.qdata != NULL) {
            if (gtk_widget_get_visible(rec_window)) {
                gtk_widget_destroy(rec_window);



            }

        }
    }
    rec_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    button_rec = gtk_check_button_new_with_label("Record");


    if (record == true) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_rec), TRUE);
    }




    gtk_window_set_title(GTK_WINDOW (rec_window), "Record window");

    box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(rec_window), box2);

    gtk_box_pack_start(GTK_BOX(box2), button_rec, 1, 1, 0);

    g_signal_connect(button_rec, "toggled", G_CALLBACK(set_record), NULL);



    gtk_window_set_position(GTK_WINDOW(rec_window), GTK_WIN_POS_CENTER);

    g_signal_connect(G_OBJECT(rec_window), "destroy",
                     G_CALLBACK(close_window), rec_window);


    gtk_widget_show_all(rec_window);




}
/**
 * function device_window(): create a window with buttons that represent columns in the device list
 * when a button is checked the column connected to that button is shown or hidden. The buttons are checked depending on
 * the columns visibility
 * input:none.
 * output:none.
 * */
void device_window() {


    GList *array,*temp;
    GtkTreeViewColumn *column;
    gint column_id;
    GtkWidget *box2;
    if(dev_window !=NULL){
        if (dev_window->parent_instance.qdata != NULL) {
            if (gtk_widget_get_visible(dev_window)) {
                gtk_widget_destroy(dev_window);



            }

        }
    }
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


    array= gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview_devices));
    temp=array;

        while(temp!=NULL) {

            column = GTK_TREE_VIEW_COLUMN(temp->data);
            column_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(column), "column-id"));
            switch (column_id) {

                case COL_DEV:

                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_devices),
                                                 gtk_tree_view_column_get_visible(column));

                    break;
                case COL_DIR:

                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_directory),
                                                 gtk_tree_view_column_get_visible(column));

                    break;
                case COL_USED:

                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_used),
                                                 gtk_tree_view_column_get_visible(column));

                    break;
                case COL_FREE:

                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_free),
                                                 gtk_tree_view_column_get_visible(column));

                    break;
                case COL_TOTAL:

                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_total),
                                                 gtk_tree_view_column_get_visible(column));

                    break;
                case COL_AVAILABLE:

                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_avail),
                                                 gtk_tree_view_column_get_visible(column));

                    break;
                case COL_TYPE:

                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_type),
                                                 gtk_tree_view_column_get_visible(column));

                    break;
                default:
                    break;

            }
            temp=temp->next;
        }

        if (device_all == TRUE) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_device_all), TRUE);
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
        g_signal_connect(button_device_devices, "toggled", G_CALLBACK(change_list_store_view_devices), NULL);
        g_signal_connect(button_device_all, "toggled", G_CALLBACK(show_all), NULL);
        g_signal_connect(button_device_directory, "toggled", G_CALLBACK(change_list_store_view_devices), NULL);
        g_signal_connect(button_device_avail, "toggled", G_CALLBACK(change_list_store_view_devices), NULL);
        g_signal_connect(button_device_total, "toggled", G_CALLBACK(change_list_store_view_devices), NULL);
        g_signal_connect(button_device_free, "toggled", G_CALLBACK(change_list_store_view_devices), NULL);
        g_signal_connect(button_device_used, "toggled", G_CALLBACK(change_list_store_view_devices), NULL);
        g_signal_connect(button_device_type, "toggled", G_CALLBACK(change_list_store_view_devices), NULL);


        gtk_window_set_position(GTK_WINDOW(dev_window), GTK_WIN_POS_CENTER);

        g_signal_connect(G_OBJECT(dev_window), "destroy",
                         G_CALLBACK(close_window), dev_window);

        gtk_widget_show_all(dev_window);



    g_list_free(array);







}


/**
 * function close_window_toggled(): when the graph window is closed we set the button graph to not be clicked
 * input:none.
 * output:none.
 * */
void close_window_toggled() {


    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_graph),
                                 FALSE);


}
/**
 * function close_window(): closes a widget
 * input:pointer to a widget.
 * output:none.
 * */
void close_window(GtkWidget *widget) {


    gtk_widget_hide(widget);


   if(widget==CPU_WINDOW){
       closed_cpu_window=!closed_cpu_window;
   }
    gtk_widget_destroyed(widget,&widget);



}

/*
 * function graph_button_clicked(): opens a window containing buttons that are connected to the displaying of cpu stats
 * input:pointer to a widget.
 * output:none.
 * */
void graph_button_clicked(GtkWidget *widget) {

    GtkWidget *box2;
    GtkWidget *temp;


    char   string[10];



    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget))) {

        window_graphs = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW (window_graphs), "GRAPH buttons");
        box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add(GTK_CONTAINER(window_graphs), box2);


        for(int i=0;i<cpu_num;i++){

            sprintf(string,"CPU%d",i);
            temp=gtk_toggle_button_new_with_label(string);


            if(cpu_status[i]==true){
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(temp), TRUE);
            }
            gtk_box_pack_start(GTK_BOX(box2), temp, 1, 1, 0);
            g_signal_connect(temp, "toggled", G_CALLBACK(graph_clicked), NULL);
            cpu_buttons[i]=*temp;

        }

        gtk_window_set_position(GTK_WINDOW(window_graphs), GTK_WIN_POS_CENTER);

        g_signal_connect(G_OBJECT(window_graphs), "destroy",
                         G_CALLBACK(close_window_toggled), NULL);

        gtk_widget_show_all(window_graphs);

    } else {

        gtk_widget_destroy(window_graphs);

    }
};

/*
 * function show_all(): sends command to server about what type of devices it wants to see
 * input:pointer to a widget.
 * output:none.
 * */
void show_all(GtkWidget *widget) {

    char *proxy = NULL;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget))) {

        device_all = TRUE;

        device_task_commands(proxy, proxy);

    } else {
        device_all = FALSE;
        device_task_commands(proxy, proxy);
    }





};


/**
 * graph_clicked(): draws specific cpus depending on which button is pressed
 * @param widget is a pointer to a button that when toggled shows graph of the cpu
 * @return void
 * */

void graph_clicked(GtkWidget *widget) {


    bool *temp_bool=cpu_status;
    GtkWidget *temp_widget=cpu_buttons;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget))) {/*if a button is toggled we set its bool to true*/

        for(int i=0;i<cpu_num;i++){

            if(temp_widget->priv==widget->priv){
                (*temp_bool)=true;
                gtk_widget_queue_draw(graph1);
                gtk_widget_show(&cpuGraphs[i]);
                gtk_widget_queue_draw(&cpuGraphs[i]);

            }

            temp_bool++;
            temp_widget++;

        }

    } else {

        for(int i=0;i<cpu_num;i++){

            if(temp_widget->priv==widget->priv){

                (*temp_bool)=false;
                gtk_widget_queue_draw(graph1);
                gtk_widget_hide(&cpuGraphs[i]);
              //  gtk_widget_queue_draw(&cpu_graphs[i]);
            }

            temp_bool++;
            temp_widget++;
        }


    }

}


/*
 * function show_hide(): shows or hides a window depending if the button is pressed or not
 * input:pointer to a button that is being pressed and pointer to window that is connected to that button
 * output:none.
 * */

void show_hide(GtkWidget *button, GtkWidget *window) {


    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (button) )) {



        gtk_widget_show(window);

    } else {

        gtk_widget_hide(window);

    }


}
/*
 * function handle_task_menu(): gets the selected task and send it a signal
 * input:pointer to a button that is being pressed and pointer to the signal that we want to send
 * output:none.
 * */

void handle_task_menu(GtkWidget *widget, char *signal) {

    if (signal != NULL) {


        if (strcmp(signal, "STOP") == 0 || strcmp(signal, "CONT") == 0 || strcmp(signal, "KILL") == 0 ||
            strcmp(signal, "TERM") == 0) {
            char *task_id = "";
            GtkTreeModel *model;
            GtkTreeIter iter;

            if (gtk_tree_selection_get_selected(selection, &model, &iter)) { /*get the tasks info*/
                gtk_tree_model_get(model, &iter, 1, &task_id, -1);
                device_task_commands(signal, task_id);

            }
        }
    }
}
/*
 * function handle_task_prio(): gets the selected task and send it a signal
 * input:pointer to a button that is being pressed and pointer to the signal that we want to send
 * output:none.
 * */
void handle_task_prio(GtkWidget *widget, char *signal) {

    if (signal != NULL) {


        if (strcmp(signal, "VERY_LOW") == 0 || strcmp(signal, "VERY_HIGH") == 0 || strcmp(signal, "NORMAL") == 0
            || strcmp(signal, "LOW") == 0 || strcmp(signal, "HIGH") == 0) {
            char *task_id = "";
            GtkTreeModel *model;
            GtkTreeIter iter;

            if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
                gtk_tree_model_get(model, &iter, 1, &task_id, -1);
                device_task_commands(signal, task_id);

            }
        }
    }
}
/*
 * function createTask_pop_up(): creates a popup menu
 * input:none.
 * output: return a pop_up_menu.
 * */
GtkWidget *createTask_pop_up(void) {
    GtkWidget *taskwidget;


    GtkWidget *menu_item;

    GtkWidget *menu_priority;


    taskwidget = gtk_menu_new();

    menu_item = gtk_menu_item_new_with_mnemonic(("Stop"));
    gtk_widget_show(menu_item);
    gtk_container_add(GTK_CONTAINER (taskwidget), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK(handle_task_menu), "STOP");

    menu_item = gtk_menu_item_new_with_mnemonic(("Continue"));
    gtk_widget_show(menu_item);
    gtk_container_add(GTK_CONTAINER (taskwidget), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK(handle_task_menu), "CONT");

    menu_item = gtk_menu_item_new_with_mnemonic(("Term"));
    gtk_widget_show(menu_item);
    gtk_container_add(GTK_CONTAINER (taskwidget), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK(handle_task_menu), "TERM");

    menu_item = gtk_menu_item_new_with_mnemonic(("Kill"));
    gtk_widget_show(menu_item);
    gtk_container_add(GTK_CONTAINER (taskwidget), menu_item);
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
    gtk_container_add(GTK_CONTAINER (taskwidget), menu_item);


    gtk_widget_show_all(taskwidget);

    return taskwidget;
}
/**
 * function on_treeview_tasks_button_press_event(): reacts to a right click on a task in the list and then creates a pop
 * up menu.
 * input:gtk button  and pointer to an event.
 * output: return bool.
 * */
gboolean on_treeview_tasks_button_press_event(GtkButton *button, GdkEventButton *event) {

    if (event->button == 3) {

        GdkEventButton *mouseevent = event;

        if (task_popup == NULL)
            task_popup = createTask_pop_up();

        gtk_menu_popup(GTK_MENU(task_popup), NULL, NULL, NULL, NULL, mouseevent->button, mouseevent->time);

    }
    return FALSE;
}