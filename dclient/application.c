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


    GtkWidget *frame2;
    GtkWidget *frame3;



    entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), "insert command here...");




    window = gtk_application_window_new (app);
    gtk_window_set_title(GTK_WINDOW(window), confy.name);
    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(destroy_window), NULL);

    if(closed_cpu_window){
        CPU_WINDOW= cpuWindow((int) cpu_num, &cpuGraphs);
        g_signal_connect(G_OBJECT(CPU_WINDOW), "destroy",
                         G_CALLBACK(close_window), CPU_WINDOW);
    }



    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);
    gtk_window_set_default_size(GTK_WINDOW(window),(gint) confy.width, (gint) confy.height);


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


    graphCpu        = gtk_drawing_area_new(); /*creating graphs*/
    graphNet        = gtk_drawing_area_new();
    graphMem        = gtk_drawing_area_new();
    graph_inttrp    = gtk_drawing_area_new();


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

    viewportcpu        = gtk_viewport_new (NULL,NULL);

    gtk_container_add (GTK_CONTAINER(viewportcpu), graphCpu);
    cpu_swindow  = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_set_border_width (GTK_CONTAINER (cpu_swindow), 1);
    gtk_container_add (GTK_CONTAINER(cpu_swindow), viewportcpu);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (cpu_swindow),
                                    GTK_POLICY_ALWAYS,
                                    GTK_POLICY_ALWAYS);
    gtk_widget_set_hexpand(GTK_WIDGET(cpu_swindow), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(cpu_swindow), TRUE);

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


   // gtk_container_add(GTK_CONTAINER(frame1), graphCpu);
  //  gtk_box_pack_start(GTK_BOX(hbox1), frame1, 1, TRUE, 0);
   // gtk_box_pack_start(GTK_BOX(hbox1), frame1, 1, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox1), cpu_swindow, 0, TRUE, 0);

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
    g_signal_connect_data((GObject *) (graphCpu),"draw",G_CALLBACK(on_draw_event),NULL,NULL,0);
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
                              G_CALLBACK( on_treeview_tasks_button_press_event),
                              NULL);
    printf("entered infitine loop\n");
    fflush(stdout);
    init_timeout();  /*starting the infinite loop for asking for data and drawing it*/

}

void set_record(GtkWidget *widget){

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget))) {

        confy.record = true;



    } else {

        confy.record = false;

    }


}

void destroy_window(void) {

    printf("HI %s \n",__func__);
    fflush(stdout);
//    GtkWidget *windo;
//    GtkWidget *button;
//    GtkWidget *button_box;
//
//    windo= gtk_application_window_new (gtkApplication);
//    gtk_window_set_title (GTK_WINDOW (windo), "Window");
//    gtk_window_set_default_size (GTK_WINDOW (windo), 200, 200);
//    gtk_window_set_position(GTK_WINDOW(windo), GTK_WIN_POS_CENTER);
//    button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
//    gtk_container_add (GTK_CONTAINER (windo), button_box);
//
//    button = gtk_button_new_with_label ("Hello World");
//  //  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
//    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), windo);
//    gtk_container_add (GTK_CONTAINER (button_box), button);
//
//    gtk_widget_show_all (windo);


    g_application_quit(G_APPLICATION(gtkApplication));

}

int test_strtol(__uint64_t val) {


    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
        || (errno != 0 )) {
        perror("strtol");


        return -1;

    }

    return 0;
}

int test_strtof(double val) {


    if ((errno == ERANGE && (val == DBL_MAX || val == DBL_MIN))
        || (errno != 0 )) {
        perror("strtol");
        return -1;

    }

    return 0;
}

void incRefresh() {

    if (confy.delay >= 10000) {

        confy.delay = 10000;
    } else {
        confy.delay += 250;
    }

    timeout_refresh();


}


void decRefresh() {


    if (confy.delay <= 250) {
        confy.delay = 250;


    } else {
        confy.delay -= 250;
    }

    timeout_refresh();

}

void pause_app(GtkWidget *button) {

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (button))) {


        if(refresh>0)
            if (!g_source_remove(refresh)) {
                g_critical ("Unable to remove source");
                return;
            }

        refresh = 0;




    } else {
        init_timeout();
    }


}




void timeout_refresh() {



    if(refresh>0)
        if (!g_source_remove(refresh)) {
            g_critical ("Unable to remove source");
            return;
        }

    refresh = 0;
    flagTimeout=false;

    init_timeout();

}






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
                if(temp_i2f->CPU)
                    free(temp_i2f->CPU);
                free(temp_i2f);
            }

        default:
            return;

    }

}

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
            if(temp_i->CPU)
                free(temp_i->CPU);

            free(temp_i);
        }
        m_ptr->interrupts_list=NULL;
        free(temp_ptr);


    }

}

int read_config(){

    FILE *fp;
    char *filename = "config.txt";
    char buffer[1024];
    int i=0;
    char *p;
    if ((fp = fopen(filename, "r")) == NULL)
        return 1;

    while((fgets(buffer,1024,fp))!=NULL){

        if(strncmp(buffer,"APPLICATION_NAME:",17)==0){
            p=&buffer[17];
            while(*p==' '){
                p++;
            }
           while(*p!='\n'){
               confy.name[i++]=*p++;
           }


            continue;
        }
        if(strncmp(buffer,"WINDOW_SIZE:",11)==0){

           sscanf( buffer,"WINDOW_SIZE: %d %d",&confy.width,&confy.height);

            continue;
        }
        if(strncmp(buffer,"DELAY:",6)==0){

            sscanf( buffer,"DELAY: %d",&confy.delay);

            continue;
        }
        if(strncmp(buffer,"RECORD:",7)==0){

            p=&buffer[7];

            while(*p==' '){
                p++;
            }
            if(strncmp(p,"FALSE",5)==0 ||strncmp(p,"false",4)==0){
                confy.record=false;

            }else if(strncmp(p,"TRUE",4)==0 || strncmp(p,"true",4)==0){
                confy.record=true;

            }




            continue;
        }
        if(strncmp(buffer,"FILE_SYSTEMS:",13)==0){

            p=&buffer[13];

            while(*p==' '){
                p++;
            }
            if(strncmp(p,"FALSE",5)==0){
                confy.fileS=false;

            }else if(strncmp(p,"TRUE",4)==0){
                confy.fileS=true;

            }

            continue;

        }
        if(strncmp(buffer,"FONT:",5)==0){

            p=&buffer[5];

            while(*p==' '){
                p++;
            }
            i=0;
            while(*p!='\n' && i<256){
                confy.font[i++]=*p++;
            }





            continue;

        }



    }

return 0;

}

void check_config(){

    if(confy.delay<=0){
        confy.delay=1000;
    }


    if(strlen(confy.name)==0){
        strcpy(confy.name,"Appication");
    }
    if(confy.fontS<=0){
        confy.fontS=8;
    }
    if(strlen(confy.font)<=0){
        strcpy(confy.font,"Arial");
    }



}

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
            if(temp_i->CPU)
                free(temp_i->CPU);

            free(temp_i);
        }
        free(temp_ptr);


    }
    (*m_ptr)=NULL;
}

void allocate_stats(Mega_Data **n_ptr) {
    Mega_Data *ptr;

    ptr=(Mega_Data*) calloc(1,sizeof(Mega_Data));
    if(ptr==NULL){
        printf("calloc error %d \n", errno);
        free(ptr);
        *n_ptr=NULL;
        return ;
    }
    *n_ptr=ptr;
    ptr->cpu_stats=calloc((size_t)cpu_num,sizeof(ptr->cpu_stats));
    if(ptr->cpu_stats==NULL){
        printf("calloc error %d \n", errno);
        free(ptr->cpu_stats);
        ptr->cpu_stats=NULL;
        free(ptr);
        ptr=NULL;
        return ;
    }


    ptr->net_stats=calloc(2,sizeof(ptr->net_stats));
    if(ptr->net_stats==NULL){
        printf("calloc error %d \n", errno);
        free(ptr->net_stats);
        ptr->net_stats=NULL;
        free(ptr->cpu_stats);
        ptr->cpu_stats=NULL;
        free(ptr);
        ptr=NULL;
        return ;
    }
    ptr->mem_stats=calloc(2,sizeof(ptr->mem_stats));
    if(ptr->mem_stats==NULL){
        printf("calloc error %d \n", errno);
        free(ptr->mem_stats);
        ptr->net_stats=NULL;
        free(ptr->net_stats);
        ptr->net_stats=NULL;
        free(ptr->cpu_stats);
        ptr->cpu_stats=NULL;
        free(ptr);
        ptr=NULL;
        return ;
    }
    ptr->next=m_data;
    if(m_data!=NULL){
        m_data->prev=ptr;
    }
    if(m_data==NULL){
        mDataHead=ptr;
    }
    m_data=ptr;


}


gboolean init_timeout() {

    int             ret              ;
    __int32_t       dev_num      =  0;//in the begging its zero
    __int32_t       task_num     =  0;



    Cpu_usage       cpu_usage   ;
    Network         network      =  {0};
    Memory_usage    memory_usage =  {0};


    Mega_Data       *temp_mega  ;





    ret=(int) test_recv(newsockfd);
    if (ret <= 0) {

        if(refresh>0)
            g_source_remove(refresh);


        g_application_quit(G_APPLICATION(gtkApplication));

        return FALSE;
    }

    cpu_usage.percentage=calloc((size_t)cpu_num,sizeof(char[16]));
    if(cpu_usage.percentage==NULL){
        printf("calloc error %d \n", errno);
        free(cpu_usage.percentage);
        if(refresh>0)
            g_source_remove(refresh);

        g_application_quit(G_APPLICATION(gtkApplication));


        return FALSE;
    }



    allocate_stats(&temp_mega);
    if(temp_mega==NULL){
        printf("calloc error %d \n", errno);
        free(temp_mega);
        free(cpu_usage.percentage);



        if(refresh>0)
            g_source_remove(refresh);

        g_application_quit(G_APPLICATION(gtkApplication));



        return FALSE;

    }






    if (data_transfer(newsockfd, &cpu_usage, &network, &memory_usage, &temp_mega->task_list, &temp_mega->device_list,
                      &task_num, &dev_num, &temp_mega->interrupts_list) != 0) {

        free_mega_data(&m_data);
        free(cpu_usage.percentage);
        if(refresh>0)
            g_source_remove(refresh);


        g_application_quit(G_APPLICATION(gtkApplication));


        return FALSE;
    }



    if((device_check(temp_mega->device_list, &devices_old))!=0){


        free(cpu_usage.percentage);


        if(refresh>0)
            g_source_remove(refresh);

        g_application_quit(G_APPLICATION(gtkApplication));


        return FALSE;

    }


    if(( task_check(temp_mega->task_list, &tasks_old))!=0){
        free(cpu_usage.percentage);


        if(refresh>0)
            g_source_remove(refresh);

        g_application_quit(G_APPLICATION(gtkApplication));



        return FALSE;

    }





    if (list_num_size < LIST_SIZE+1) {
        list_num_size++;
    }


    if (list_num_size >= LIST_SIZE+1) {

        temp_mega=mDataHead->prev;
        gl_delay-=mDataHead->delay;
        free_one_mega_data(mDataHead);
        mDataHead= temp_mega;
        mDataHead->next=NULL;

    }

    printf("delay %lu delay/250 %lu\n",gl_delay,gl_delay/250);
    fflush(stdout);
    static int counter =0;
    if(counter ==10){
        g_application_quit(G_APPLICATION(gtkApplication));

    }
    counter++;

    if(confy.record){

        printf("writting \n");
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


    m_data->gtk_delay=   time_step = 60000 / confy.delay;


    gl_delay  += m_data->delay=confy.delay;


    gtk_widget_queue_draw(graphCpu);
    gtk_widget_queue_draw(graphNet);
    gtk_widget_queue_draw(graphMem);
    gtk_widget_queue_draw(graph_inttrp);
    if(closed_cpu_window==true) {


        for (int i = 0; i < cpu_num; i++) {
            gtk_widget_queue_draw(&cpuGraphs[i]);
        }
    }



    printf("pass %d \n",counter);
    fflush(stdout);
    if (refresh == 0){


        refresh = g_timeout_add(confy.delay, (GSourceFunc) init_timeout, NULL);

    }



    if(flagTimeout== false){

        flagTimeout=true;
        return FALSE;
    }

    return TRUE;
}