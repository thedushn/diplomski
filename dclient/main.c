

#include "drawing.h"
#include "testing_tree.h"
#include "window.h"
#include "buttons.h"
#include"sys/socket.h"
#include <netdb.h>
#include <fontconfig/fontconfig.h>
#include <errno.h>
#include <inttypes.h>
#include <assert.h>
#include "functions.h"



GtkWidget *window;


static guint refresh = 0;
static guint t = 2000;

static guint time_step = 0;









gboolean on_draw_event(GtkWidget *widget, cairo_t *cr) {


    if (widget == graph1) {

        do_drawing_cpu(widget, cr, bjorg, time_step, CPU0_line, CPU1_line, CPU2_line, CPU3_line);
    } else if (widget == graph2) {

        do_drawing_net(widget, cr, bjorg, time_step);
    } else if (widget == graph3) {

        do_drawing_mem(widget, cr, time_step);
    } else {

        do_drawing_int(widget, cr);

    }


    return FALSE;

}


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


void graph_refresh(GtkWidget *widget, gboolean CPU) {

    if (widget == button_graph0) {


        CPU0_line = CPU;


    } else if (widget == button_graph1) {


        CPU1_line = CPU;

    } else if (widget == button_graph2) {

        CPU2_line = CPU;


    } else {

        CPU3_line = CPU;


    }

    timeout_refresh();


}


void timeout_refresh() {

    if (!g_source_remove(refresh)) {
        g_critical ("Unable to remove source");
        return;
    }
    refresh = 0;
    init_timeout();

}

int connection(char *argv1, char *argv2) {


    struct addrinfo hints, *servinfo, *p;
    int rv;
    int socketfd = 0;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv2, argv1, &hints, &servinfo)) != 0) {
        fprintf(stderr, "get_addr_info: %s\n", gai_strerror(rv));
        return -2;
    }

// loop through all the results and connect to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((socketfd = socket(p->ai_family, p->ai_socktype,
                               p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        if (connect(socketfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("connect");
            close(socketfd);
            continue;
        }

        break; // if we get here, we must have connected successfully
    }

    if (p == NULL) {
        // looped off the end of the list with no connection
        free(servinfo);
        fprintf(stderr, "failed to connect\n");
        return -2;

    }


    if (socketfd < 0) {
        printf("Error creating socket!\n");
        return -2;
    }


    free(servinfo);
    return socketfd;
}
void freeing_memory(Unification *array){

    Unification test=*array;
    assert(array);
    switch(array->type_in_union){

        case CPU_S :
            free(array);
            break;
        case COLLETION :

            break;
        default:
            printf("something went wrong\n");
            break;
    }


    free(array);
}

void init_timeout() {

    int i , j,ret ;
    Collection *temp_collection;
    dev_num=0;//in the begging its zero
    task_num=0;




    Cpu_usage *cpu_usage1=NULL;
    Network *network=NULL;
    Memory_usage *memory_usage=NULL;
    Task *tasks_new=NULL;
    Devices *devices_new=NULL;
    network = calloc(1, sizeof(Network));
    if(network==NULL){

        printf("calloc error %d \n", errno);
        free(network);
        exit(1);
    }
    memory_usage = calloc(1, sizeof(Memory_usage));
    if(memory_usage==NULL){

        printf("calloc error %d \n", errno);
        free(memory_usage);
        free(network);

        exit(1);
    }
    cpu_usage1 = calloc(1, sizeof(Cpu_usage));
    if(cpu_usage1==NULL){

        printf("calloc error %d \n", errno);
        free(memory_usage);
        free(cpu_usage1);
        free(network);
        exit(1);
    }


   ret= data_transfer(newsockfd, cpu_usage1, network, memory_usage, &tasks_new, &devices_new);

    if(ret!=0){

    printf("freeing memory\n");

        free(cpu_usage1);

        free(network);

        free(memory_usage);
        if(tasks_new!=NULL){
            free(tasks_new);
        }
        if(devices_new!=NULL){
            free(devices_new);
        }


         exit(1);
    }




//    /*devices */
    D_Collection *rem_old=devices_old;
    Devices *rem_new=devices_new;
    //refreshing data
    for (i = 0; i < dev_num_old; i++) //uzimamo element niza
    {
        Devices *tmp = &devices_old->devices;

        tmp->checked = FALSE;

        for (j = 0; j < dev_num; j++) {
            Devices *new_tmp =devices_new;

            if (strcmp(new_tmp->directory, tmp->directory) == 0
                && strcmp(new_tmp->name, tmp->name) == 0
                && strcmp(new_tmp->type, tmp->type) == 0
                && new_tmp->fid == tmp->fid
                && new_tmp->total == tmp->total
                    )  //comparing elements of the array
            {


                if ((gint) tmp->avail != (gint) new_tmp->avail //ako ima razlika
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
                devices_new=rem_new;
                break;


            } else
                tmp->checked = FALSE;

            devices_new++;
        }
        devices_new=rem_new;
        devices_old=devices_old->next;

    }

    devices_old=rem_old;

    devices_new=rem_new;
    //  check for unchecked old-devices for deleting
    i = 0;
    rem_old=devices_old;
    while (i <dev_num_old) {
        if(devices_old==NULL){
            break;
        }
        Devices *tmp = &devices_old->devices;
        D_Collection *dtemp;

        if (!tmp->checked)//element of the array that does not exist in the new array anymore
        {
            remove_list_item_device(tmp->directory, tmp->name);
            if(i==0){
                dtemp=devices_old;
                if(devices_old->next!=NULL){
                    devices_old=devices_old->next;
                }



                free(dtemp);
                rem_old=devices_old; //setting the first node
            }else{

                devices_old=rem_old;
                for(int k=0;k<i-1;k++){

                    devices_old=devices_old->next;


                }
                dtemp=devices_old->next;
                devices_old->next=dtemp->next;
                free(dtemp);

                devices_old=rem_old;


            }



            dev_num_old--;
        } else{
            i++;


            devices_old=devices_old->next;


        }

    }
    devices_old=rem_old;


    //  check for unchecked new devices for inserting

    Devices *rem_tmp=devices_new;

    for (i = 0; i < dev_num; i++) {
        D_Collection *new_tmp;

        if (!devices_new->checked) {
            new_tmp=calloc(1,sizeof(D_Collection));
            if(new_tmp==NULL){

                printf("calloc error %d \n", errno);
                free(new_tmp);
                free(cpu_usage1);
                free(network);
                free(memory_usage);
                free(tasks_new);
                free(devices_new);
                exit(1);
            }
            new_tmp->devices=*devices_new;
            new_tmp->next=devices_old;

            devices_old=new_tmp;

            add_new_list_item_dev(&devices_old->devices);
            dev_num_old++;
        }

        devices_new++;
    }
    devices_new=rem_tmp;

    /*devices */

    /*tasks */
    T_Collection *rem_task_old=tasks_old;
    Task *rem_task_new=tasks_new;

    for (i = 0; i <task_num_old; i++) {

        Task *tmp = &tasks_old->task;
        assert(tmp);
        tmp->checked = FALSE;

        for (j = 0; j < task_num; j++) {
            Task *new_tmp = tasks_new;

            assert(new_tmp);
            float cpu_user_tmp ;
            float cpu_system_tmp ;
            float cpu_user_tmp_new ;
            float cpu_system_tmp_new ;

            cpu_system_tmp = (float) strtod(tmp->cpu_system,NULL);

            cpu_system_tmp_new = (float) strtod(new_tmp->cpu_system,NULL);

            cpu_user_tmp = (float) strtod(tmp->cpu_user,NULL);

            cpu_user_tmp_new = (float) strtod(new_tmp->cpu_system,NULL);


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
                tasks_new=rem_task_new;

                break;

            } else
                tmp->checked = FALSE;

            tasks_new++;

        }
        tasks_new=rem_task_new;
        tasks_old=tasks_old->next;

    }

    tasks_old=rem_task_old;

    tasks_new=rem_task_new;
    //  check for unchecked old-tasks for deleting
    i = 0;
    rem_task_old=tasks_old;
    while (i < task_num_old) {

        Task *tmp = &tasks_old->task;
        T_Collection *t_temp;
        if (!tmp->checked) {

            remove_list_item((gint) tmp->pid);
            if(i==0){
                t_temp=tasks_old;

                tasks_old=tasks_old->next;

                free(t_temp);
                rem_task_old=tasks_old;
            }else{

                tasks_old=rem_task_old;

                for(int k=0;k<i-1;k++){
                    tasks_old=tasks_old->next;

                }

                t_temp=tasks_old->next;

                tasks_old->next=t_temp->next;
                free(t_temp);

                tasks_old=rem_task_old;

            }


            task_num_old--;
        } else{
            i++;

            tasks_old=tasks_old->next;

        }

    }

    tasks_old=rem_task_old;

    //  check for unchecked new tasks for inserting

    rem_task_new=tasks_new;

    for (i = 0; i < task_num; i++) {
        T_Collection *new_tmp;


        if (!tasks_new->checked) {
            new_tmp=calloc(1,sizeof(T_Collection));
            if(new_tmp==NULL){

                printf("calloc error %d \n", errno);
                free(new_tmp);
                free(cpu_usage1);
                free(network);
                free(memory_usage);
                free(tasks_new);
                free(devices_new);
                exit(1);
            }

            new_tmp->task=*tasks_new;
            //point it to old first node
            new_tmp->next=tasks_old;
            //point first to new first node
            tasks_old=new_tmp;

            add_new_list_item(&tasks_old->task);
            task_num_old++;
        }

        tasks_new++;

    }

    tasks_new=rem_task_new;

    //Cpu network memory_usage

    temp_collection=(Collection *)calloc(1, sizeof(Collection));
    if(temp_collection==NULL){

        printf("calloc error %d \n", errno);
        free(temp_collection);


        free(tasks_new);
        free(cpu_usage1);

        free(network);

        free(memory_usage);
        free(tasks_new);
        free(devices_new);
        exit(1);
    }




    //point it to old first node
    temp_collection->next = collection;
    //point first to new first node
    collection=temp_collection;

    if(bjorg<LIST_SIZE){
        bjorg++;
    }



    cpu_change(cpu_usage1);
    network_change_rc(network);
    memory_change(memory_usage);
    swap_change(memory_usage);
    if(bjorg>=LIST_SIZE){
           temp_collection=collection;

       for(int g=0;g<LIST_SIZE;g++){
           collection=collection->next;

           }

           free(collection);
        collection=NULL;
        collection=temp_collection;


       }



    gtk_widget_queue_draw(window);









    free(cpu_usage1);

    free(network);

    free(memory_usage);
    free(devices_new);
    free(tasks_new);


    time_step = 60000 / t;




    //if (refresh == 0) {

        refresh = g_timeout_add(t, (GSourceFunc) init_timeout, NULL);

  //  }

}

static void
destroy_window(void) {
    if (gtk_main_level() > 0)
        gtk_main_quit();
}






int main(int argc, char *argv[]) {

        bjorg=0;
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
    CPU0_line=TRUE;
    CPU1_line=TRUE;
    CPU2_line=TRUE;
    CPU3_line=TRUE;

    gtk_init(&argc, &argv);

    interrupts=calloc(10,sizeof(Interrupts));

    Collection *  temp;





    //circular list
//    Cpu_Stats *  temp=(Cpu_Stats *)calloc(1, sizeof(Cpu_Stats));
//
//    cpu_stats = temp;
//    //end of the link
//    stats0_tail=temp;
//    // Create the link.
//    cpu_stats->next = cpu_stats;
//
//
//
//    for(int i=0;i<list_size;i++){
//      temp=(Cpu_Stats *)calloc(1, sizeof(Cpu_Stats));
//
//
//
//
//      //  temp->cpu_0=i;
//        temp->next = cpu_stats->next;
//
//
//        cpu_stats->next = temp;
//
//
//    }
////circular list





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



    init_timeout();

    gtk_main();
    gtk_tree_store_clear(list_store);
    gtk_tree_store_clear(list_store1);

    if (refresh > 0) {
        g_source_remove(refresh);

    }







    for(int i=0;i<bjorg;i++){
       // save reference to first link
      temp = collection;

      //mark next to first link as first
        collection = collection->next;

      //return the deleted link
      free(temp);

    }

    free(interrupts);

    D_Collection *rem_tmp;
    for(int k=0;k<dev_num_old;k++){
        // save reference to first link
        rem_tmp = devices_old;

        //mark next to first link as first
        devices_old = devices_old->next;

        //return the deleted link
        free(rem_tmp);

    }

    T_Collection *rem_tmp_task;
    for(int k=0;k<task_num_old;k++){
        // save reference to first link
        rem_tmp_task = tasks_old;

        //mark next to first link as first
        tasks_old = tasks_old->next;

        //return the deleted link
        free(rem_tmp_task);

    }






    close(newsockfd);
    close(newsockfd1);
    //save reference to first link


   // cairo_debug_reset_static_data();
    //   FcFini();


    return 0;
}
