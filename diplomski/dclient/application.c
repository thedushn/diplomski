//
// Created by dushn on 15.4.21..
//
#include "main_header.h"
#include "testing_tree.h"
#include "buttons.h"
#include <semaphore.h>

#include "errno.h"

#include "functions.h"

#include "window.h"
#include "drawing.h"
#include "testing.h"

GtkWidget *window; //!< main window
bool flagTimeout=true;//!<flag for letting the init_timeout function know what to do

bool writing=true; //!<is recording being done
/**
 * activate() creates the application , the main window ,
 * the cpu window, the graphs the ,labels and menu ,and list
 * @param app
 * @param user_data default
 * @return void
 * */
void
activate (GtkApplication *app,
          gpointer        user_data)
{
    GtkWidget *menubar;


    GtkWidget *filemenu;
    GtkWidget *filemenu2;
    GtkWidget *speed;
    GtkWidget *devicesMenu;
    GtkWidget *quit;
    GtkWidget *increaseRefresh;
    GtkWidget *decrease_refresh;
    GtkWidget *file_system;
    GtkWidget *process_menu;
    GtkWidget *filemenu3;
    GtkWidget *processItem;

    GtkWidget *recordL;


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




    window = gtk_application_window_new (app);
    gtk_window_set_title(GTK_WINDOW(window), "System resource monitor");

    CPU_WINDOW= cpuWindow((int) cpu_num, &cpuGraphs);
    g_signal_connect(G_OBJECT(CPU_WINDOW), "destroy",
                     G_CALLBACK(close_window), CPU_WINDOW);


    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 400);


    device_swindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(device_swindow), GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_ALWAYS);
    process_swindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(process_swindow), GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_ALWAYS);



    vbox            = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);/*creating boxes where we are going to stack or graphs*/
    hbox            = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);//cpu labels
    hbox1           = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);//graph1 graphNet frame1 frame2
    hbox2           = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);//in between graphs
    hbox3           = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);//graphMem graph_inttrp frame3 frame4

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
    devicesMenu      = gtk_menu_item_new_with_label("devices");
    process_menu     = gtk_menu_item_new_with_label("Process Manager");

    processItem      = gtk_menu_item_new_with_label("Processes");
    increaseRefresh  = gtk_menu_item_new_with_label("+250");
    decrease_refresh = gtk_menu_item_new_with_label("-250");
    file_system      = gtk_menu_item_new_with_label("file_systems");
    recordL          = gtk_menu_item_new_with_label("recordL");

    quit             = gtk_menu_item_new_with_label("Quit");


    gtk_menu_item_set_submenu(GTK_MENU_ITEM(speed), filemenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), increaseRefresh);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), decrease_refresh);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), recordL);

    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);


    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), speed);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), process_menu);


    gtk_menu_item_set_submenu(GTK_MENU_ITEM(devicesMenu), filemenu2);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(process_menu), filemenu3);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu2), file_system);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu3), processItem);


    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), devicesMenu);


    g_signal_connect(quit, "activate", G_CALLBACK(destroy_window), NULL);

    g_signal_connect(increaseRefresh, "activate", G_CALLBACK(incRefresh), NULL);
    g_signal_connect(decrease_refresh, "activate", G_CALLBACK(decRefresh), NULL);
    g_signal_connect(file_system, "activate", G_CALLBACK(device_window), NULL);
    g_signal_connect(recordL, "activate", G_CALLBACK(recordWindow), NULL);


    g_signal_connect(processItem, "activate", G_CALLBACK(process_window), NULL);


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
    gtk_container_add(GTK_CONTAINER(device_swindow), treeview_devices);


    graph1          = gtk_drawing_area_new(); /*creating graphs*/
    graphNet        = gtk_drawing_area_new();
    graphMem        = gtk_drawing_area_new();
    graph_inttrp    = gtk_drawing_area_new();

    frame1          = gtk_frame_new(NULL);
    frame2          = gtk_frame_new(NULL);
    frame3          = gtk_frame_new(NULL);

    viewport        = gtk_viewport_new (NULL,NULL);

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


    gtk_container_add(GTK_CONTAINER(frame2), graphNet);
    gtk_box_pack_start(GTK_BOX(hbox1), frame2, 1, TRUE, 0);


    gtk_box_pack_start(GTK_BOX(vbox), hbox3, 1, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(frame3), graphMem);

    //gtk_container_add(GTK_CONTAINER(frame4), graph_inttrp);

    gtk_box_pack_start(GTK_BOX(hbox3), frame3, 1, TRUE, 0);
    // gtk_box_pack_start(GTK_BOX(hbox3), frame4, 1, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox3), interrupts_swindow, 0, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), device_swindow, TRUE, TRUE, 1);
    gtk_box_pack_start(GTK_BOX(vbox), process_swindow, TRUE, TRUE, 1);
    //  gtk_container_add(GTK_CONTAINER(vbox), process_swindow);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 1);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    /*creating graphs by sending the draw signal to the function we create a cairo_t structure*/
    g_signal_connect_data((GObject *) (graph1),"draw",G_CALLBACK(on_draw_event),NULL,NULL,0);
    g_signal_connect_data((GObject *) (graphNet), "draw", G_CALLBACK(on_draw_event), NULL, NULL, 0);
    g_signal_connect_data((GObject *) (graphMem), "draw", G_CALLBACK(on_draw_event), NULL, NULL, 0);
    g_signal_connect_data((GObject *) (graph_inttrp), "draw", G_CALLBACK(on_draw_event), NULL, NULL, 0);




    g_signal_connect(entry, "activate", G_CALLBACK(input_command), NULL);

    gtk_widget_show_all(window);

    g_signal_connect(button_inc, "clicked", G_CALLBACK(incRefresh), NULL);
    g_signal_connect(button_dec, "clicked", G_CALLBACK(decRefresh), NULL);
    g_signal_connect(button_pause, "toggled", G_CALLBACK(pause_app), button_pause);
    g_signal_connect(button_proc, "toggled", G_CALLBACK(show_hide), process_swindow);
    g_signal_connect(button_dev, "toggled", G_CALLBACK(show_hide), device_swindow);


    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_proc),
                                 TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_dev),
                                 TRUE);




    g_signal_connect(button_graph, "clicked", G_CALLBACK(graph_button_clicked),NULL);


    g_signal_connect_swapped ((gpointer) treeview_tasks, "button-press-event",
                              G_CALLBACK(on_treeview_tasks_button_press_event),
                              NULL);

    init_timeout();  /*starting the infinite loop for asking for data and drawing it*/

}
/**
 *set_record(): sets the record flag to true or false depending on if the button is clicked or not
 * @param widget
 * @return void
 * */
void set_record(GtkWidget *widget){

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget))) {

        record = true;
        sem_post(&semt);


    } else {
        sem_wait(&semt);
        record = false;
        sem_post(&semt);
    }


}
/**
 * destroy_window() destroys window
 * @return void
 *
 * */
void destroy_window(void) {

    sem_wait(&semt);
    g_application_quit(G_APPLICATION(gtkApplication));
    sem_post(&semt);
}
/**
 * test_strtol() tests if strtol didnt convert properly
 * @param val
 * @return void
 *
 * */
void test_strtol(long val) {


    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
        || (errno != 0 )) {
        perror("strtol");
        sem_wait(&semt);

        g_application_quit(G_APPLICATION(gtkApplication));

        sem_post(&semt);
    }


}
/**
 * incRefresh(): increments the time that we want the client to request data from server
 *
 * @return void
 * */
void incRefresh() {

    if (t >= 10000) {

        t = 10000;
    } else {
        t += 250;
    }

    timeout_refresh();


}

/**
 * decRefresh(): decrease the time that we want the client to request data from server

 * @return void
 * */
void decRefresh() {


    if (t <= 250) {
        t = 250;


    } else {
        t -= 250;
    }

    timeout_refresh();

}
/**
 * pause_app(): stops or restarts the client from to requesting data from server
 * @param button if the button is toggled it pauses the data gathering
 * @return void
 * */
void pause_app(GtkWidget *button) {

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (button))) {

        sem_wait(&semt);
        if(refresh>0)
            if (!g_source_remove(refresh)) {
                g_critical ("Unable to remove source");
                return;
            }

        refresh = 0;
        sem_post(&semt);



    } else {
        init_timeout();
    }


}



/**
 * timeout_refresh(): reruns the function init_timeout and tells the previous version to stop
 *
 * @return void
 * */
void timeout_refresh() {


    sem_wait(&semt);
    if(refresh>0)
        if (!g_source_remove(refresh)) {
            g_critical ("Unable to remove source");
            return;
        }

    refresh = 0;
    flagTimeout=false;
    sem_post(&semt);
    init_timeout();

}




/**
 * freeingMemory(): frees different types of memory
 * @param array
 * @param array_size
 * @param type of the array.
 * @return  void.
 * */


void freeingMemory(void *array, __int32_t *array_size, int type){

    T_Collection *temp_task;
    T_Collection *temp_taskf;

    D_Collection *temp_device;
    D_Collection *temp_devicef;

    NetMem_list *temp_netmem;
    NetMem_list *temp_netmemf;
    Cpu_list    *temp_cpu;
    Cpu_list    *temp_cpuf;

    I_Collection2 *temp_i2=NULL;
    I_Collection2 *temp_i2f=NULL;



    switch (type) {

        case CPU_USAGE:

            temp_cpu=(Cpu_list*)array;
            for(__int32_t i=0;i<(*array_size);i++){
                temp_cpuf= temp_cpu;
                temp_cpu=temp_cpu->next;
                free(temp_cpuf->data);
                free(temp_cpuf);


            }


            break;

        case NETWORK:
            temp_netmem=(NetMem_list*)array;
            for(__int32_t i=0;i<(*array_size);i++){
                temp_netmemf= temp_netmem;
                temp_netmem=temp_netmem->next;
                free(temp_netmemf);


            }


            break;

        case MEMORY:

            temp_netmem=(NetMem_list*)array;
            for(__int32_t i=0;i<(*array_size);i++){
                temp_netmemf= temp_netmem;
                temp_netmem=temp_netmem->next;
                free(temp_netmemf);


            }


            break;

        case TASK:
            temp_task=(T_Collection*)array;
            for(__int32_t i=0;i<(*array_size);i++){
                temp_taskf= temp_task;
                temp_task=temp_task->next;
                free(temp_taskf);


            }
            (*array_size)=0;


            break;

        case DEVICES:
            temp_device=(D_Collection*)array;
            for(__int32_t i=0;i<(*array_size);i++){
                temp_devicef= temp_device;
                temp_device=temp_device->next;
                free(temp_devicef);


            }
            (*array_size)=0;

            break;
        case INTERRUPTS:
            temp_i2=(I_Collection2 *)array;
            while(temp_i2){
                temp_i2f=temp_i2;
                temp_i2=temp_i2->next;
                if(temp_i2f->interrupts.CPU)
                    free(temp_i2f->interrupts.CPU);
                free(temp_i2f);
            }

        default:
            return;

    }

}
/**
 * free_one_mega_data() frees the allocated memory of one element of a list of Mega_Data structs
 * @param m_ptr
 * @return void
 * */
void  free_one_mega_data(Mega_Data *m_ptr){
    Mega_Data       *temp_ptr;
    T_Collection    *temp_task;
    D_Collection    *temp_device;
    I_Collection2   *temp_i=NULL;

    if(m_ptr){
        temp_ptr=m_ptr;


        if(temp_ptr->cpu_stats){
            free(temp_ptr->cpu_stats);
            temp_ptr->cpu_stats=NULL;
        }


        if(temp_ptr->net_stats){
            free(temp_ptr->net_stats);
            temp_ptr->net_stats=NULL;
        }
        if(temp_ptr->mem_stats){
            free(temp_ptr->mem_stats);
            temp_ptr->mem_stats=NULL;
        }
        while(temp_ptr->device_list){
            temp_device=temp_ptr->device_list;
            temp_ptr->device_list=temp_ptr->device_list->next;
            free(temp_device);
        }
        m_ptr->device_list=NULL;
        while(temp_ptr->task_list){
            temp_task=temp_ptr->task_list;
            temp_ptr->task_list=temp_ptr->task_list->next;
            free(temp_task);
        }
        m_ptr->task_list=NULL;
        while(temp_ptr->interrupts_list){
            temp_i=temp_ptr->interrupts_list;
            temp_ptr->interrupts_list=temp_ptr->interrupts_list->next;
            if(temp_i->interrupts.CPU)
                free(temp_i->interrupts.CPU);

            free(temp_i);
        }
        m_ptr->interrupts_list=NULL;
        free(temp_ptr);


    }

}
/**
 * free_mega_data() frees the allocated memory for a doubly linked list of Mega_Data structs
 * @param m_ptr
 * @return void
 * */
void  free_mega_data(Mega_Data **m_ptr){
    Mega_Data       *temp_ptr;
    T_Collection    *temp_task;
    D_Collection    *temp_device;
    I_Collection2   *temp_i=NULL;

    while((*m_ptr)){
        temp_ptr=(*m_ptr);
        (*m_ptr)=(*m_ptr)->next;

        if(temp_ptr->cpu_stats){
            free(temp_ptr->cpu_stats);
            temp_ptr->cpu_stats=NULL;
        }


        if(temp_ptr->net_stats){
            free(temp_ptr->net_stats);
            temp_ptr->net_stats=NULL;
        }
        if(temp_ptr->mem_stats){
            free(temp_ptr->mem_stats);
            temp_ptr->mem_stats=NULL;
        }
        while(temp_ptr->device_list){
            temp_device=temp_ptr->device_list;
            temp_ptr->device_list=temp_ptr->device_list->next;
            free(temp_device);
        }
        while(temp_ptr->task_list){
            temp_task=temp_ptr->task_list;
            temp_ptr->task_list=temp_ptr->task_list->next;
            free(temp_task);
        }
        while(temp_ptr->interrupts_list){
            temp_i=temp_ptr->interrupts_list;
            temp_ptr->interrupts_list=temp_ptr->interrupts_list->next;
            if(temp_i->interrupts.CPU)
                free(temp_i->interrupts.CPU);

            free(temp_i);
        }
        free(temp_ptr);


    }
    (*m_ptr)=NULL;
}
/**
 * allocate_stats() allocates memory used for storing data gather from server
 *
 * @returns pointer to struct  Mega_Data
 * */
Mega_Data *allocate_stats(){
    Mega_Data *ptr;
    ptr=(Mega_Data*) calloc(1,sizeof(Mega_Data));
    if(ptr==NULL){
        printf("calloc error %d \n", errno);
        free(ptr);
        return NULL;
    }
    ptr->cpu_stats=calloc(cpu_num,sizeof(ptr->cpu_stats));
    if(ptr->cpu_stats==NULL){
        printf("calloc error %d \n", errno);
        free(ptr->cpu_stats);
        free(ptr);
        return NULL;
    }


    ptr->net_stats=calloc(2,sizeof(ptr->net_stats));
    if(ptr->net_stats==NULL){
        printf("calloc error %d \n", errno);
        free(ptr->net_stats);
        free(ptr->cpu_stats);
        free(ptr);
        return NULL;
    }
    ptr->mem_stats=calloc(2,sizeof(ptr->mem_stats));
    if(ptr->mem_stats==NULL){
        printf("calloc error %d \n", errno);
        free(ptr->mem_stats);
        free(ptr->net_stats);
        free(ptr->cpu_stats);
        free(ptr);
        return NULL;
    }
    ptr->next=m_data;
    if(m_data!=NULL){
        m_data->prev=ptr;
    }
    if(m_data==NULL){
        mDataHead=ptr;
    }
    m_data=ptr;

    return ptr;
}
/**
 * init_timeout(): sends a request to server and then waits for data,after it got all the data it inputs it in
 * the right places and checks if the list_num_size is bigger then the LIST_SIZE if that is the case it removes the
 * oldest element of the list and adds the newest to the begging.After the data has been properly handled it displays it
 * in the lists and draws the new data on the graph.We check if the function is running in an infinite loop,if not we
 * set it to run in regular intervals that we have set.
 *
 * @return  returns TRUE if we want to continue or FALSE if we want to stop
 * */

gboolean init_timeout() {

    int             ret              ;
    __int32_t       dev_num      =  0;//in the begging its zero
    __int32_t       task_num     =  0;



    Cpu_usage       cpu_usage   ;
    Network         network      =  {0};
    Memory_usage    memory_usage =  {0};


    Mega_Data       *temp_mega  ;

    sem_wait(&semt);




    ret=(int) test_recv(newsockfd);
    if (ret <= 0) {

        if(refresh>0)
            g_source_remove(refresh);


        g_application_quit(G_APPLICATION(gtkApplication));
        sem_post(&semt);
        return FALSE;
    }

    cpu_usage.percentage=calloc(cpu_num,sizeof(char[16]));
    if(cpu_usage.percentage==NULL){
        printf("calloc error %d \n", errno);
        free(cpu_usage.percentage);
        if(refresh>0)
            g_source_remove(refresh);

        g_application_quit(G_APPLICATION(gtkApplication));

        sem_post(&semt);
        return FALSE;
    }



    temp_mega=allocate_stats();
    if(temp_mega==NULL){
        printf("calloc error %d \n", errno);
        free(temp_mega);
        free(cpu_usage.percentage);



        if(refresh>0)
            g_source_remove(refresh);

        g_application_quit(G_APPLICATION(gtkApplication));

        sem_post(&semt);

        return FALSE;

    }






    if (data_transfer(newsockfd, &cpu_usage, &network, &memory_usage, &temp_mega->task_list, &temp_mega->device_list,
                      &task_num, &dev_num, &temp_mega->interrupts_list) != 0) {

        free_mega_data(&m_data);
        free(cpu_usage.percentage);
        if(refresh>0)
            g_source_remove(refresh);


        g_application_quit(G_APPLICATION(gtkApplication));
        sem_post(&semt);

        return FALSE;
    }



    if((device_check(temp_mega->device_list, dev_num))!=0){


        free(cpu_usage.percentage);


        if(refresh>0)
            g_source_remove(refresh);

        g_application_quit(G_APPLICATION(gtkApplication));

        sem_post(&semt);
        return FALSE;

    }


    if(( task_check(temp_mega->task_list, task_num))!=0){
        free(cpu_usage.percentage);


        if(refresh>0)
            g_source_remove(refresh);

        g_application_quit(G_APPLICATION(gtkApplication));

        sem_post(&semt);

        return FALSE;

    }





    if (list_num_size < LIST_SIZE+1) {
        list_num_size++;
    }



    if (list_num_size >= LIST_SIZE+1) {

        temp_mega=mDataHead->prev;
        free_one_mega_data(mDataHead);
        mDataHead= temp_mega;
        mDataHead->next=NULL;

    }

    if(record){


        if(writing==true){//beginning


            char *test;
            ssize_t size;

            struct stat st;

            time_t clk=time(NULL);
            strcat(p_dir,ctime(&clk));
            size=(ssize_t) strlen(p_dir);
            p_dir[size-1]=' ';
            while((test=strchr(p_dir,' '))!=strrchr(p_dir,' ')){
                *test='_';
            }
            test=strrchr(p_dir,' ');
            *test='/';
            if(stat(p_dir,&st)==-1){
                mkdir(p_dir,0777);
            }

            writing=false;

            interrupts_write(m_data->interrupts_list) ;
            device_write(devices_old);


            task_write(tasks_old);
            cpu_write(cpu_usage);
            memory_write(&memory_usage);
            netw_write( network.transmited_bytes, network.received_bytes);
        }else{

            interrupts_write(m_data->interrupts_list) ;
            device_write(devices_old);


            task_write(tasks_old);
            cpu_write(cpu_usage);
            memory_write(&memory_usage);
            netw_write( network.transmited_bytes, network.received_bytes);
        }

    }else{

        writing=true;

    }

    cpu_change(&cpu_usage);
    network_change(&network);
    memoryChange(&memory_usage);
    swap_change(&memory_usage);


    free(cpu_usage.percentage);


    time_step = 60000 / t;

    gtk_widget_queue_draw(graph1);
    gtk_widget_queue_draw(graphNet);
    gtk_widget_queue_draw(graphMem);
    gtk_widget_queue_draw(graph_inttrp);
    if(closed_cpu_window==true) {


        for (int i = 0; i < cpu_num; i++) {
            gtk_widget_queue_draw(&cpuGraphs[i]);
        }
    }



    if (refresh == 0){


        refresh = g_timeout_add(t, (GSourceFunc) init_timeout, NULL);

    }
    sem_post(&semt);
    if(flagTimeout== false){

        flagTimeout=true;
        return FALSE;
    }

    return TRUE;
}