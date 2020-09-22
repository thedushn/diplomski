

#include "drawing.h"
#include "testing_tree.h"
#include "buttons.h"
#include <errno.h>
#include "functions.h"
#include <semaphore.h>


GtkWidget *window;

sem_t semt;

bool flag_timeout=true;



void inc_refresh() {

    if (t >= 10000) {

        t = 10000;
    } else {
        t += 250;
    }

   timeout_refresh();


}

void dec_refresh() {


    if (t <= 250) {
        t = 250;


    } else {
        t -= 250;
    }

    timeout_refresh();

}





void timeout_refresh() {


    sem_wait(&semt);
    if(refresh>0)
        if (!g_source_remove(refresh)) {
        g_critical ("Unable to remove source");
        return;
    }

    refresh = 0;
    flag_timeout=false;
    sem_post(&semt);
    init_timeout();

}







void freeing_memory(void *array, __int32_t *array_size, int type){

    T_Collection *temp_task;
    T_Collection *temp_taskf;

    D_Collection *temp_device;
    D_Collection *temp_devicef;

    NetMem_list *temp_netmem;
    NetMem_list *temp_netmemf;
    Cpu_list *temp_cpu;
    Cpu_list *temp_cpuf;




    switch (type) {

        case CPU_USAGE:

            temp_cpu=(Cpu_list*)array;
            for(__int32_t i=0;i<(*array_size);i++){
                temp_cpuf= temp_cpu;
                temp_cpu=temp_cpu->next;
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


        default:
            return;

    }

    }

gboolean init_timeout() {

    int ret;

    __int32_t  dev_num = 0;//in the begging its zero
    __int32_t  task_num = 0;


    Cpu_usage cpu_usage = {0};
    Network network = {0};
    Memory_usage memory_usage = {0};
    T_Collection *tasks_new = NULL;
    D_Collection *devices_new = NULL;

    Cpu_list *temp_collection;
    NetMem_list *temp_net;
    NetMem_list *temp_mem;


    sem_wait(&semt);
    ret = data_transfer(newsockfd, &cpu_usage, &network, &memory_usage, &tasks_new, &devices_new, &task_num, &dev_num);


    if (ret != 0) {

        printf("freeing memory\n");

        freeing_memory(devices_new,&dev_num,DEVICES);
        freeing_memory(tasks_new,&task_num,TASK);



        if(refresh>0)
            g_source_remove(refresh);

        if (gtk_main_level() > 0)
            gtk_main_quit();
        return FALSE;
    }


    if((device_check(devices_new, dev_num))!=0){


        freeing_memory(devices_new,&dev_num,DEVICES);
        freeing_memory(tasks_new,&task_num,TASK);


        if(refresh>0)
            g_source_remove(refresh);
        if (gtk_main_level() > 0)
            gtk_main_quit();

        return FALSE;

    }



    if(( task_check(tasks_new, task_num))!=0){


        freeing_memory(devices_new,&dev_num,DEVICES);
        freeing_memory(tasks_new,&task_num,TASK);


        if(refresh>0)
            g_source_remove(refresh);
        if (gtk_main_level() > 0)
            gtk_main_quit();

        return FALSE;

    }

    temp_collection = (Cpu_list *) calloc(1, sizeof(Cpu_list));

    if (temp_collection == NULL) {

        printf("calloc error %d \n", errno);
        free(temp_collection);

        freeing_memory(devices_new,&dev_num,DEVICES);
        freeing_memory(tasks_new,&task_num,TASK);


        if(refresh>0)
        g_source_remove(refresh);
        if (gtk_main_level() > 0)
            gtk_main_quit();

       return FALSE;
    }



    //point it to old first node
    temp_collection->next = cpu_list;
    //point first to new first node
    cpu_list = temp_collection;



    temp_mem = (NetMem_list *) calloc(1, sizeof(NetMem_list));

    if (temp_mem == NULL) {

        printf("calloc error %d \n", errno);
        free(temp_mem);

        freeing_memory(devices_new,&dev_num,DEVICES);
        freeing_memory(tasks_new,&task_num,TASK);


        if(refresh>0)
            g_source_remove(refresh);
        if (gtk_main_level() > 0)
            gtk_main_quit();

        return FALSE;
    }


    //point it to old first node
    temp_mem->next = mem_list;
    //point first to new first node
    mem_list = temp_mem;


    temp_net = (NetMem_list *) calloc(1, sizeof(NetMem_list));

    if (temp_net == NULL) {

        printf("calloc error %d \n", errno);
        free(temp_net);

        freeing_memory(devices_new,&dev_num,DEVICES);
        freeing_memory(tasks_new,&task_num,TASK);


        if(refresh>0)
            g_source_remove(refresh);
        if (gtk_main_level() > 0)
            gtk_main_quit();

        return FALSE;
    }


    //point it to old first node
    temp_net->next = net_list;
    //point first to new first node
    net_list = temp_net;



    if (bjorg < LIST_SIZE) {
        bjorg++;
    }



    if (bjorg >= LIST_SIZE) {

        temp_collection = cpu_list;
        temp_mem=mem_list;
        temp_net=net_list;

        for (int g = 0; g < LIST_SIZE; g++) {
            temp_collection = temp_collection->next;
            temp_mem= temp_mem->next;
            temp_net= temp_net->next;

        }

        free(temp_collection);
        free(temp_net);
        free(temp_mem);




    }




    cpu_change(cpu_usage);
    network_change_rc(&network);
    memory_change(&memory_usage);
    swap_change(&memory_usage);

    freeing_memory(devices_new,&dev_num,DEVICES);
    freeing_memory(tasks_new,&task_num,TASK);



    time_step = 60000 / t;

    gtk_widget_queue_draw(window);




    if (refresh == 0){


        refresh = g_timeout_add(t, (GSourceFunc) init_timeout, NULL);

    }
    sem_post(&semt);
    if(flag_timeout==false){
        flag_timeout=true;
        return FALSE;
    }

    return TRUE;
}

void destroy_window(void) {
    if (gtk_main_level() > 0)
        gtk_main_quit();
}





int main(int argc, char *argv[]) {

        bjorg=0;
        t = 2000;
        refresh=0;
        time_step=0;
    show_before=FALSE;
    device_all=false;


    if (argc < 3) {

        printf("port not provided \n");
        printf("ip_address not provided \n");
        exit(1);
    }

    if (argv[1] == NULL) {
        printf("argv failed %s", argv[1]);
        exit(1);

    }
    if (argv[2] == NULL) {
        printf("argv failed %s", argv[2]);
        exit(1);

    }
    newsockfd = connection(argv[1], argv[2]);
    if (newsockfd < 0) {
        close(newsockfd);
        exit(1);
    }
    newsockfd1 = connection(argv[1], argv[2]);
    if (newsockfd1 < 0) {
        close(newsockfd);
        close(newsockfd1);
        exit(1);
    }




    cpu_status=calloc((size_t)CPU_NUM,sizeof(bool));
    if(cpu_status==NULL){
        free(cpu_status);

        printf("calloc error %d \n", errno);
        return -1;
    }

    for(int i=0;i<CPU_NUM;i++){
        cpu_status[i]=true;
    }

    cpu_buttons=calloc((size_t)CPU_NUM,sizeof(GtkWidget));
    if(cpu_buttons==NULL){

        free(cpu_buttons);

        printf("calloc error %d \n", errno);
        return -1;
    }

    gtk_disable_setlocale();

    gtk_init(&argc, &argv);


    interrupts=calloc(10,sizeof(Interrupts));
    if(interrupts==NULL){

        free(interrupts);
        free(cpu_status);

        printf("calloc error %d \n", errno);
        return -1;
    }









    device_swindow = gtk_scrolled_window_new(NULL,
                                             NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(device_swindow), GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_ALWAYS);
    process_swindow = gtk_scrolled_window_new(NULL,
                                              NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(process_swindow), GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_ALWAYS);






    window = main_window(device_swindow, process_swindow);

    g_signal_connect(button_inc, "clicked", G_CALLBACK(inc_refresh), NULL);
    g_signal_connect(button_dec, "clicked", G_CALLBACK(dec_refresh), NULL);
    g_signal_connect(button_proc, "toggled", G_CALLBACK(button_clicked_view_process), NULL);


    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_proc),
                                 TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_dev),
                                 TRUE);
    g_signal_connect(button_dev, "toggled", G_CALLBACK(dev_button_clicked2), NULL);



    g_signal_connect(button_graph, "clicked", G_CALLBACK(graph_button_clicked), NULL);

    g_signal_connect_swapped ((gpointer) treeview, "button-press-event", G_CALLBACK(on_treeview1_button_press_event),
                              NULL);



    g_signal_connect_data((GObject *) (graph1),"draw",G_CALLBACK(on_draw_event),NULL,NULL,0);
    g_signal_connect_data((GObject *) (graph2),"draw",G_CALLBACK(on_draw_event),NULL,NULL,0);
    g_signal_connect_data((GObject *) (graph3),"draw",G_CALLBACK(on_draw_event),NULL,NULL,0);
    g_signal_connect_data((GObject *) (graph4),"draw",G_CALLBACK(on_draw_event),NULL,NULL,0);




    g_signal_connect (window, "destroy", G_CALLBACK(destroy_window), NULL);
    g_signal_connect ((GObject *)(process_swindow), "destroy",
                      G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect ((GObject *)(device_swindow), "destroy",
                      G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(entry, "activate", G_CALLBACK(input_command), NULL);


    sem_init(&semt,0,1);

    init_timeout();


    gtk_main();

    gtk_tree_store_clear(list_store);
    gtk_tree_store_clear(list_store1);


    free(interrupts);



    freeing_memory(cpu_list,&bjorg,CPU_USAGE);
    freeing_memory(devices_old,&dev_num_old,DEVICES);
    freeing_memory(tasks_old,&task_num_old,TASK);
    freeing_memory(net_list,&bjorg,NETWORK);
    freeing_memory(mem_list,&bjorg,MEMORY);
    free(cpu_status);
    free(cpu_buttons);




    sem_destroy(&semt);



    close(newsockfd);
    close(newsockfd1);



    return 0;
}
