

#include "drawing.h"
#include "testing_tree.h"
#include "buttons.h"
#include <errno.h>
#include "functions.h"
#include "testing.h"
#include <semaphore.h>
#include <asm/errno.h>
#include <inttypes.h>


GtkWidget *window; /*main window*/

sem_t semt; /*semaphore for letting the init_timeout function finish before we change the time interval */

bool flag_timeout=true;/*flag for letting the init_timeout function know what to do */

bool writing=true;
/*
 * function set_record(): sets the record flag to true or false depending on if the button is clicked or not
 * input  : widget.
 * output : none.
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
/*
 * function inc_refresh(): increments the time that we want the client to request data from server
 * input  : none.
 * output : none.
 * */
void inc_refresh() {

    if (t >= 10000) {

        t = 10000;
    } else {
        t += 250;
    }

   timeout_refresh();


}

/*
 * function dec_refresh(): decrease the time that we want the client to request data from server
 * input  : none.
 * output : none.
 * */
void dec_refresh() {


    if (t <= 250) {
        t = 250;


    } else {
        t -= 250;
    }

    timeout_refresh();

}
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



/*
 * function timeout_refresh(): reruns the function init_timeout and tells the previous version to stop
 * input  : none.
 * output : none.
 * */
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




/*
 * function freeing_memory(): frees different types of memory
 * input  : void pointer to an array, pointer to the size of the array and the type of the array.
 * output : none.
 * */


void freeing_memory(void *array, __int32_t *array_size, int type){

    T_Collection *temp_task;
    T_Collection *temp_taskf;

    D_Collection *temp_device;
    D_Collection *temp_devicef;

    NetMem_list *temp_netmem;
    NetMem_list *temp_netmemf;
    Cpu_list    *temp_cpu;
    Cpu_list    *temp_cpuf;




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
/*
 * function init_timeout(): sends a request to server and then waits for data,after it got all the data it inputs it in
 * the right places and checks if the list_num_size is bigger then the LIST_SIZE if that is the case it removes the
 * oldest element of the list and adds the newest to the begging.After the data has been properly handled it displays it
 * in the lists and draws the new data on the graph.We check if the function is running in an infinite loop,if not we
 * set it to run in regular intervals that we have set.
 * input  : none
 * output : returns TRUE if we want to continue or FALSE if we want to stop;
 * */

gboolean init_timeout() {

    int             ret          =  0;
    __int32_t       dev_num      =  0;//in the begging its zero
    __int32_t       task_num     =  0;
    T_Collection    *tasks_new   =  NULL;
    D_Collection    *devices_new =  NULL;


    Cpu_usage       cpu_usage    =  {0};
    Network         network      =  {0};
    Memory_usage    memory_usage =  {0};

    Cpu_list        *temp_collection;
    NetMem_list     *temp_net;
    NetMem_list     *temp_mem;



    sem_wait(&semt);

    ret=(int) test_recv(newsockfd);
    if (ret <= 0) {

        if(refresh>0)
            g_source_remove(refresh);

        if (gtk_main_level() > 0)
            gtk_main_quit();

        sem_post(&semt);
        return FALSE;
    }




    if (data_transfer(newsockfd, &cpu_usage, &network, &memory_usage, &tasks_new, &devices_new, &task_num, &dev_num) != 0) {


        freeing_memory(devices_new,&dev_num,DEVICES);
        freeing_memory(tasks_new,&task_num,TASK);

        if(refresh>0)
            g_source_remove(refresh);

        if (gtk_main_level() > 0)
            gtk_main_quit();

        sem_post(&semt);

        return FALSE;
    }


    if((device_check(devices_new, dev_num))!=0){


        freeing_memory(devices_new,&dev_num,DEVICES);
        freeing_memory(tasks_new,&task_num,TASK);


        if(refresh>0)
            g_source_remove(refresh);

        if (gtk_main_level() > 0)
            gtk_main_quit();

        sem_post(&semt);
        return FALSE;

    }


    if(( task_check(tasks_new, task_num))!=0){

        freeing_memory(devices_new,&dev_num,DEVICES);
        freeing_memory(tasks_new,&task_num,TASK);

        if(refresh>0)
            g_source_remove(refresh);

        if (gtk_main_level() > 0)
            gtk_main_quit();

        sem_post(&semt);

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

        sem_post(&semt);

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

        sem_post(&semt);
        return FALSE;
    }


    //point it to old first node
    temp_mem->next = mem_list;
    //point first to new first node
    mem_list       = temp_mem;


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

        sem_post(&semt);
        return FALSE;
    }


    //point it to old first node
    temp_net->next = net_list;
    //point first to new first node
    net_list       = temp_net;



    if (list_num_size < LIST_SIZE) {
        list_num_size++;
    }



    if (list_num_size >= LIST_SIZE) {

        temp_collection = cpu_list;
        temp_mem        = mem_list;
        temp_net        = net_list;

        for (int g = 0; g < LIST_SIZE; g++) {

            temp_collection = temp_collection->next;
            temp_mem        = temp_mem->next;
            temp_net        = temp_net->next;

        }

        free(temp_collection);
        free(temp_net);
        free(temp_mem);

    }

    if(record){


        if(writing==true){//beginning


            char *test;
            ssize_t size;

            struct stat st;

            time_t clk=time(NULL);
            strcat(p_dir,ctime(&clk));
            size= strlen(p_dir);
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

           interrupts_write(interrupts) ;
            device_write(devices_old);


            task_write(tasks_old);
            cpu_write(cpu_usage);
            memory_write(&memory_usage);
            netw_write( network.transmited_bytes, network.received_bytes);
        }else{

           interrupts_write(interrupts) ;
            device_write(devices_old);


            task_write(tasks_old);
            cpu_write(cpu_usage);
            memory_write(&memory_usage);
            netw_write( network.transmited_bytes, network.received_bytes);
        }

    }else{

        writing=true;

    }

    cpu_change(cpu_usage);
    network_change(&network);
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

    sem_wait(&semt);
    if (gtk_main_level() > 0){
        gtk_main_quit();
    }
    sem_post(&semt);
}

void test_strtol(int val) {

    sem_wait(&semt);
    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
        || (errno != 0 && val == 0)) {
        perror("strtol");

        if (gtk_main_level() > 0){
            gtk_main_quit();
        }

    }
         sem_post(&semt);
};


/*
 * function main(): creates a TPC socket and tries to connect to the server,if that was successful
 * it initializes the window and starts to request for data from the server;
 *
 * input  : port number and IP address
 * output : returns a non zero value if something goes wrong
 * */
int main(int argc, char *argv[]) {

        list_num_size   = 0;
        t               = 1000; /*what delay we want when asking for data*/
        refresh         = 0;
        time_step       = 0;    /*space between data*/
        device_all      = false;
        record          = false;
        newsockfd       = 0;
        newsockfd1      = 0;



//    float rec=0;
//    float tr=0;
//    ifstat_calculate(&rec, &tr);
//
//    float  received=0;
//    float  trans=0;
//    netw_calculate(&trans,&received);
//    printf("klijent poslao %f primio %f   \nifstat  poslao %f primio %f\n",trans,received,rec,tr);


    if (argc < 3) {

        printf("port not provided \n");
        printf("ip_address not provided \n");
        return -1;
    }

    if (argv[1] == NULL) {
        printf("argv failed %s", argv[1]);
        return -1;

    }
    if (argv[2] == NULL) {
        printf("argv failed %s", argv[2]);
        return -1;

    }

    if ((newsockfd = connection(argv[1], argv[2])) < 0) {
        close(newsockfd);
        return -1;
    }

    if ((newsockfd1 = connection(argv[1], argv[2])) < 0) {
        close(newsockfd);
        close(newsockfd1);
        return -1;
    }




    cpu_status=calloc((size_t)CPU_NUM,sizeof(bool));
    if(cpu_status==NULL){
        free(cpu_status);
        close(newsockfd);
        close(newsockfd1);
        printf("calloc error %d \n", errno);
        return -1;
    }

    for(int i=0;i<CPU_NUM;i++){
        cpu_status[i]=true;
    }

    cpu_buttons=calloc((size_t)CPU_NUM,sizeof(GtkWidget));
    if(cpu_buttons==NULL){

        free(cpu_status);
        free(cpu_buttons);
        close(newsockfd);
        close(newsockfd1);
        printf("calloc error %d \n", errno);
        return -1;
    }

    gtk_disable_setlocale(); /*sscanf and sprintf don't work if this isn't used */

    gtk_init(&argc, &argv);


    interrupts=calloc(10,sizeof(Interrupts));
    if(interrupts==NULL){

        free(interrupts);
        free(cpu_status);
        free(cpu_buttons);
        close(newsockfd);
        close(newsockfd1);

        printf("calloc error %d \n", errno);
        return -1;
    }





    device_swindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(device_swindow), GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_ALWAYS);
    process_swindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(process_swindow), GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_ALWAYS);






    window = main_window(device_swindow, process_swindow);/*creating the main window*/

    g_signal_connect(button_inc, "clicked", G_CALLBACK(inc_refresh), NULL);
    g_signal_connect(button_dec, "clicked", G_CALLBACK(dec_refresh), NULL);
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


    /*creating graphs by sending the draw signal to the function we create a cairo_t structure*/
    g_signal_connect_data((GObject *) (graph1),"draw",G_CALLBACK(on_draw_event),NULL,NULL,0);
    g_signal_connect_data((GObject *) (graph2),"draw",G_CALLBACK(on_draw_event),NULL,NULL,0);
    g_signal_connect_data((GObject *) (graph3),"draw",G_CALLBACK(on_draw_event),NULL,NULL,0);
    g_signal_connect_data((GObject *) (graph4),"draw",G_CALLBACK(on_draw_event),NULL,NULL,0);




    time_step = 60000 / t;








    g_signal_connect (window, "destroy", G_CALLBACK(destroy_window), NULL);
    g_signal_connect ((GObject *)(process_swindow), "destroy",
                      G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect ((GObject *)(device_swindow), "destroy",
                      G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(entry, "activate", G_CALLBACK(input_command), NULL);



    sem_init(&semt,0,1);

    init_timeout();  /*starting the infinite loop for asking for data and drawing it*/


    gtk_main();




    gtk_tree_store_clear(list_tasks); /*freeing memory allocated for task list*/
    gtk_tree_store_clear(list_devices);/*freeing memory allocated for device list*/
    g_object_unref(list_tasks);
    g_object_unref(list_devices);

    free(interrupts);
    freeing_memory(cpu_list,&list_num_size,CPU_USAGE);
    freeing_memory(devices_old,&dev_num_old,DEVICES);
    freeing_memory(tasks_old,&task_num_old,TASK);
    freeing_memory(net_list,&list_num_size,NETWORK);
    freeing_memory(mem_list,&list_num_size,MEMORY);

    free(cpu_status);
    free(cpu_buttons);




    sem_destroy(&semt);



    close(newsockfd);
    close(newsockfd1);






    return 0;
}
