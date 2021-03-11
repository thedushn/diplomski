//
// Created by dushn on 23.7.20..
//

#include "window.h"
#include "testing_tree.h"
#include "buttons.h"
#include "testing.h"


GtkWidget *label_rec;   /*GtkWidget where we input received data*/
GtkWidget *label_trans; /*GtkWidget where we input transmitted data*/
GtkWidget *label_cpu0;  /*GtkWidget where we input cpu data*/
GtkWidget *label_mem;   /*GtkWidget where we input memory data*/
GtkWidget *label_swap;  /*GtkWidget where we input swap memory data*/


/*
 * function main_window(): creates the main window and all the components in it
 * input:pointer to the device window, pointer to the process window;
 * output: main window
 * */

GtkWidget *main_window(GtkWidget *dev_swindow, GtkWidget *process_swindow) {


    GtkWidget *menubar;


    GtkWidget *filemenu;
    GtkWidget *filemenu2;
    GtkWidget *speed;
    GtkWidget *devices_menu;
    GtkWidget *quit;
    GtkWidget *increase_refresh;
    GtkWidget *decrease_refresh;
    GtkWidget *file_system;
    GtkWidget *process_menu;
    GtkWidget *filemenu3;
    GtkWidget *process_item;

    GtkWidget *record;


    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *hbox1;
    GtkWidget *hbox3;
    GtkWidget *hbox2;

    GtkWidget *frame1;
    GtkWidget *frame2;
    GtkWidget *frame3;



    entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), "insert command here...");





    GtkWidget *window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL); /*creates the window*/
    gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window1), 800, 400);


    vbox            = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);/*creating boxes where we are going to stack or graphs*/
    hbox            = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);//cpu labels
    hbox1           = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);//graph1 graph_net frame1 frame2
    hbox2           = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);//in between graphs
    hbox3           = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);//graph_mem graph_inttrp frame3 frame4

    button_inc      = gtk_button_new_with_label("refresh rate +");/*creating buttons under the main menu*/
    button_dec      = gtk_button_new_with_label("refresh rate-");
    button_proc     = gtk_toggle_button_new_with_label("Process");
    button_dev      = gtk_toggle_button_new_with_label("Dev");
    button_graph    = gtk_toggle_button_new_with_label("graphs");
    button_pause    = gtk_toggle_button_new_with_label("pause");


    menubar         = gtk_menu_bar_new(); /*creating the menu bar*/
    filemenu        = gtk_menu_new();   /*menu bar items*/
    filemenu2       = gtk_menu_new();
    filemenu3       = gtk_menu_new();


    speed            = gtk_menu_item_new_with_label("speed");
    devices_menu     = gtk_menu_item_new_with_label("devices");
    process_menu     = gtk_menu_item_new_with_label("Process Manager");

    process_item     = gtk_menu_item_new_with_label("Processes");
    increase_refresh = gtk_menu_item_new_with_label("+250");
    decrease_refresh = gtk_menu_item_new_with_label("-250");
    file_system      = gtk_menu_item_new_with_label("file_systems");
    record           = gtk_menu_item_new_with_label("record");

    quit = gtk_menu_item_new_with_label("Quit");


    gtk_menu_item_set_submenu(GTK_MENU_ITEM(speed), filemenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), increase_refresh);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), decrease_refresh);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), record);

    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);


    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), speed);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), process_menu);


    gtk_menu_item_set_submenu(GTK_MENU_ITEM(devices_menu), filemenu2);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(process_menu), filemenu3);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu2), file_system);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu3), process_item);


    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), devices_menu);


    g_signal_connect(quit, "activate", G_CALLBACK(destroy_window), NULL);

    g_signal_connect(increase_refresh, "activate", G_CALLBACK(inc_refresh), NULL);
    g_signal_connect(decrease_refresh, "activate", G_CALLBACK(dec_refresh), NULL);
    g_signal_connect(file_system, "activate", G_CALLBACK(device_window), NULL);
    g_signal_connect(record, "activate", G_CALLBACK(record_window), NULL);


    g_signal_connect(process_item, "activate", G_CALLBACK(process_window), NULL);


    treeview_tasks = gtk_tree_view_new();


    create_list_store_task();

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_tasks));


    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_tasks), GTK_TREE_MODEL(list_tasks));

    gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(list_tasks), 1, GTK_SORT_ASCENDING);
    gtk_container_add(GTK_CONTAINER(process_swindow), treeview_tasks);


    treeview_devices = gtk_tree_view_new();


    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
    create_list_store_dev();


    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_devices), GTK_TREE_MODEL(list_devices));

    gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(list_devices), 1, GTK_SORT_ASCENDING);
    gtk_container_add(GTK_CONTAINER(dev_swindow), treeview_devices);


    graph1      = gtk_drawing_area_new(); /*creating graphs*/
    graph_net      = gtk_drawing_area_new();
    graph_mem      = gtk_drawing_area_new();
    graph_inttrp      = gtk_drawing_area_new();

    frame1      = gtk_frame_new(NULL);
    frame2      = gtk_frame_new(NULL);
    frame3      = gtk_frame_new(NULL);



    viewport  = gtk_viewport_new (NULL,NULL);
    gtk_container_add (GTK_CONTAINER(viewport), graph_inttrp);
    interrupts_swindow  = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_set_border_width (GTK_CONTAINER (interrupts_swindow), 1);
    gtk_container_add (GTK_CONTAINER(interrupts_swindow), viewport);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (interrupts_swindow),
                                    GTK_POLICY_ALWAYS,
                                    GTK_POLICY_ALWAYS);
    gtk_widget_set_hexpand(GTK_WIDGET(interrupts_swindow), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(interrupts_swindow), TRUE);

    label_rec   = gtk_label_new(NULL);/*network_received*/
    label_trans = gtk_label_new(NULL);/*network_transmitted*/
    label_cpu0  = gtk_label_new(NULL);/*cpu usage */

    label_mem   = gtk_label_new(NULL);/*memory*/
    label_swap  = gtk_label_new(NULL);/*swap memory*/




    gtk_box_pack_start(GTK_BOX(vbox), hbox, 0, FALSE, 0);


    gtk_box_pack_start(GTK_BOX(hbox), button_inc, FALSE, FALSE, FALSE);//expand,fill,padding
    gtk_box_pack_start(GTK_BOX(hbox), button_dec, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(hbox), button_proc, 0, 0, 0);


    gtk_box_pack_start(GTK_BOX(hbox), button_dev, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(hbox), button_graph, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(hbox), button_pause, 0, 0, 0);

    gtk_box_pack_start(GTK_BOX(hbox), label_cpu0, 0, 0, 0);

    gtk_box_pack_start(GTK_BOX(hbox), label_rec, 1, 1, 1);
    gtk_box_pack_start(GTK_BOX(hbox), label_trans, 1, TRUE, 1);


    gtk_box_pack_start(GTK_BOX(vbox), hbox1, 1, 1, 0);


    gtk_container_add(GTK_CONTAINER(frame1), graph1);
    gtk_box_pack_start(GTK_BOX(hbox1), frame1, 1, TRUE, 0);


    gtk_box_pack_start(GTK_BOX(vbox), hbox2, 0, FALSE, 0);


    gtk_box_pack_start(GTK_BOX(hbox2), label_mem, 0, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox2), label_swap, 0, FALSE, 1);


    gtk_container_add(GTK_CONTAINER(frame2), graph_net);
    gtk_box_pack_start(GTK_BOX(hbox1), frame2, 1, TRUE, 0);


    gtk_box_pack_start(GTK_BOX(vbox), hbox3, 1, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(frame3), graph_mem);

    //gtk_container_add(GTK_CONTAINER(frame4), graph_inttrp);

    gtk_box_pack_start(GTK_BOX(hbox3), frame3, 1, TRUE, 0);
   // gtk_box_pack_start(GTK_BOX(hbox3), frame4, 1, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox3), interrupts_swindow, 0, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), dev_swindow, TRUE, TRUE, 1);
    gtk_box_pack_start(GTK_BOX(vbox), process_swindow, TRUE, TRUE, 1);
  //  gtk_container_add(GTK_CONTAINER(vbox), process_swindow);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 1);

    gtk_container_add(GTK_CONTAINER(window1), vbox);


    gtk_widget_show_all(window1);
    gtk_window_set_title(GTK_WINDOW(window1), "System resource monitor");

    return window1;
};



/*
 * function swap_change(): inputs swap usage into list and displays it textually in window
 * input:pointer to memory usage
 * output: none
 * */
void swap_change(Memory_usage *memory_usage) {


    gchar *swap_total, *swap_used;

    float f;
    f = (float) atof(memory_usage->swap_percentage);


    mem_list->data[1]=f;

    swap_used  = g_format_size_full((guint64) memory_usage->swap_used, G_FORMAT_SIZE_IEC_UNITS);
    swap_total = g_format_size_full((guint64) memory_usage->swap_total, G_FORMAT_SIZE_IEC_UNITS);




    gchar *swap_usage_text = g_strdup_printf(("SWAP: %0.2f%% (%s) %s"), f, swap_used, swap_total);
    gtk_label_set_text(GTK_LABEL (label_swap), swap_usage_text);

    g_free(swap_usage_text);
    g_free(swap_total);
    g_free(swap_used);

}
/*
 * function memory_change(): inputs memory usage into list and displays it textually in window
 * input:pointer to memory usage
 * output: none
 * */
void memory_change(Memory_usage *memory_usage) {


    gchar *used, *total, *memory_usage_text1;

    //float f = 0;
   // (float) atof(memory_usage->memory_percentage);

    mem_list->data[0]   = (float) strtof(memory_usage->memory_percentage,NULL);


    used                = g_format_size_full((guint64) memory_usage->memory_used, G_FORMAT_SIZE_IEC_UNITS);

    total               = g_format_size_full((guint64) memory_usage->memory_total, G_FORMAT_SIZE_IEC_UNITS);


    memory_usage_text1  = g_strdup_printf(("Memory: %0.2f%%(%s)%s"), mem_list->data[0], used, total);
    gtk_label_set_text(GTK_LABEL (label_mem), memory_usage_text1);

    g_free(memory_usage_text1);
    g_free(total);
    g_free(used);

}
/*
 * function cpu_change(): inputs cpu usage into list and displays it textually in window
 * input:   structure of  cpu_usage usage
 * output: none
 * */
void cpu_change(Cpu_usage cpu_usage) {



    gchar  *cpu0_usage_text =NULL;
    gchar  *temp_char       =NULL;

    for(int i=0;i<cpu_num;i++){

     // sscanf (cpu_usage.percentage[i],"%f",&cpu_list->data[i] ) ;
        cpu_list->data[i]= strtof (cpu_usage.percentage[i],NULL) ;

       }

    for(int i=0;i<cpu_num;i++){
        gchar *p;
        if(i==0){
            cpu0_usage_text=g_strdup_printf("CPU%d: %.4s%% ",i,cpu_usage.percentage[i]);

        }else{
           p=cpu0_usage_text;
            temp_char=g_strdup_printf("CPU%d: %.4s%% ",i,cpu_usage.percentage[i]);
            cpu0_usage_text=  g_strconcat(temp_char,p,NULL);
        }

        g_free(temp_char);

    }




    gtk_label_set_text(GTK_LABEL (label_cpu0), cpu0_usage_text);

    g_free(cpu0_usage_text);



}
/**
 * function network_change(): inputs network usage into list and displays it textually in window
 * input:   pointer to  Network usage
 * output: none
 * */
void network_change(Network *network) {


    net_list->data[0] = (float) network->received_bytes / 1024;
    net_list->data[1] = (float) network->transmited_bytes / 1024;


    gchar *rec_bytes = g_format_size_full(network->received_bytes, G_FORMAT_SIZE_IEC_UNITS);
    gchar *rec_tr_bytes = g_format_size_full(network->transmited_bytes, G_FORMAT_SIZE_IEC_UNITS);

    gchar *network_usage_received_text = g_strdup_printf("RECEIVED:  %s/s", rec_bytes);
    gchar *network_usage_transmitted_text = g_strdup_printf("TRANSMITTED: %s/s", rec_tr_bytes);
    gtk_label_set_text(GTK_LABEL (label_rec), network_usage_received_text);


    gtk_label_set_text(GTK_LABEL (label_trans), network_usage_transmitted_text);
    g_free(rec_bytes);
    g_free(rec_tr_bytes);
    g_free(network_usage_received_text);
    g_free(network_usage_transmitted_text);

}


