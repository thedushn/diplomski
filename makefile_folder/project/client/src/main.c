
#include "interrupts.h"
#include "drawing.h"
#include "testing_tree.h"
#include "window.h"
#include "buttons.h"
#include"sys/socket.h"
#include <netdb.h>
#include <fontconfig/fontconfig.h>
#include "buttons_s.h"


#include "functions.h"

GtkWidget *window;

static guint refresh = 0;
static guint t = 2000;
static int bjorg = 1;//prvi ispis


gint tasks_num;
gint dev_num;



static guint time_step = 0;


gboolean on_draw_event(GtkWidget *widget, cairo_t *cr) {


    if (widget == graph1) {

        do_drawing_cpu(widget, cr, bjorg, time_step, CPU0_line, CPU1_line, CPU2_line, CPU3_line);
    } else if (widget == graph2) {

        do_drawing_net(widget, cr, bjorg, time_step);
    } else if (widget == graph3) {

        do_drawing_mem(widget, cr, bjorg, time_step);
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


};

void dec_refresh() {


    if (t <= 250) {
        t = 250;


    } else {
        t -= 250;
    }

    timeout_refresh();

};


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


};


void timeout_refresh() {

    if (!g_source_remove(refresh)) {
        g_critical ("Unable to remove source");
        return;
    }
    refresh = 0;
    init_timeout();

}

int conection(char *argv1, char *argv2) {


    struct addrinfo hints, *servinfo, *p;
    int rv;
    int socketfd = 0;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv2, argv1, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
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
};


void init_timeout() {

    guint i = 0, j = 0;


    GArray *new_task_list = g_array_new(FALSE, FALSE, sizeof(Task));
    GArray *new_device_list = g_array_new(FALSE, FALSE, sizeof(Devices));
    GArray *new_interrupt_list = g_array_new(FALSE, FALSE, sizeof(Interrupts));

    Cpu_usage *cpu_usage1;
    Network *network;
    Memory_usage *memory_usage;

    network = calloc(1, sizeof(Network));
    memory_usage = calloc(1, sizeof(Memory_usage));
    cpu_usage1 = calloc(1, sizeof(Cpu_usage));


    primanje3(newsockfd, cpu_usage1, network, memory_usage, new_device_list, new_interrupt_list, new_task_list);
    upis(new_interrupt_list, interrupt_array_d);





    /*devices */
    for (i = 0; i < names_array->len; i++) //uzimamo element niza
    {
        Devices *tmp = &g_array_index(names_array, Devices, i);
        tmp->checked = FALSE;

        for (j = 0; j < new_device_list->len; j++) {
            Devices *new_tmp = &g_array_index(new_device_list, Devices, j);

            if (strcmp(new_tmp->directory, tmp->directory) == 0
                && strcmp(new_tmp->name, tmp->name) == 0
                && strcmp(new_tmp->type, tmp->type) == 0
                && new_tmp->fid == tmp->fid
                && new_tmp->total == tmp->total
                    )  //poredimo elemente nizova
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

                    refresh_list_item_device(i);

                }
                tmp->checked = TRUE; //
                new_tmp->checked = TRUE;
                break;


            } else
                tmp->checked = FALSE;
        }
    }
    //  check for unchecked old-devices for deleting
    i = 0;
    while (i < names_array->len) {

        Devices *tmp = &g_array_index(names_array, Devices, i);

        if (!tmp->checked)//element niza koji se ne nalazi vise u novom nizu
        {
            remove_list_item_device(tmp->directory, tmp->name);
            g_array_remove_index(names_array, i);
            dev_num--;
        } else
            i++;

    }


    //  check for unchecked new devices for inserting
    for (i = 0; i < new_device_list->len; i++) {
        Devices *new_tmp = &g_array_index(new_device_list, Devices, i);

        if (!new_tmp->checked) {
            Devices *new_device = new_tmp;

            g_array_append_val(names_array, *new_device);

            add_new_list_item_dev(dev_num);

            dev_num++;
        }
    }
    /*devices */



    for (i = 0; i < task_array->len; i++) {
        Task *tmp = &g_array_index(task_array, Task, i);
        tmp->checked = FALSE;

        for (j = 0; j < new_task_list->len; j++) {
            Task *new_tmp = &g_array_index(new_task_list, Task, j);
            float cpu_user_tmp = 0;
            float cpu_system_tmp = 0;
            float cpu_user_tmp_new = 0;
            float cpu_system_tmp_new = 0;
            cpu_system_tmp = (float) atof(tmp->cpu_system);
            cpu_system_tmp_new = (float) atof(new_tmp->cpu_system);
            cpu_user_tmp = (float) atof(tmp->cpu_user);
            cpu_user_tmp_new = (float) atof(new_tmp->cpu_system);

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

                    refresh_list_item(i);
                }
                tmp->checked = TRUE;
                new_tmp->checked = TRUE;
                break;
            } else
                tmp->checked = FALSE;
        }
    }


    i = 0;
    while (i < task_array->len) {

        Task *tmp = &g_array_index(task_array, Task, i);

        if (!tmp->checked) {
            remove_list_item((gint) tmp->pid);
            g_array_remove_index(task_array, i);
            tasks_num--;
        } else
            i++;

    }


    for (i = 0; i < new_task_list->len; i++) {
        Task *new_tmp = &g_array_index(new_task_list, Task, i);

        if (!new_tmp->checked) {
            Task *new_task = new_tmp;

            g_array_append_val(task_array, *new_task);
            add_new_list_item(tasks_num);
            tasks_num++;
        }
    }


    cpu_change(cpu_usage1);
    network_change_rc(network);
    memory_change(memory_usage);
    swap_change(memory_usage);


    gtk_widget_queue_draw(window);
    g_array_free(new_task_list, TRUE);


    g_array_free(new_interrupt_list, TRUE);


    g_array_free(new_device_list, TRUE);
    bjorg++;

    free(cpu_usage1);

    free(network);

    free(memory_usage);


    time_step = 60000 / t;

    if (bjorg >= time_step) {


        bjorg = time_step;
    }


    if (refresh == 0) {

        refresh = g_timeout_add(t, (GSourceFunc) init_timeout, NULL);
    }

};

static void
destroy_window(void) {
    if (gtk_main_level() > 0)
        gtk_main_quit();
}


int main(int argc, char *argv[]) {


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
    newsockfd = conection(argv[1], argv[2]);
    if (newsockfd < 0) {
        close(newsockfd);
        exit(1);
    }
    newsockfd1 = conection(argv[1], argv[2]);
    if (newsockfd1 < 0) {
        close(newsockfd);
        close(newsockfd1);
        exit(1);
    }

    gtk_init(&argc, &argv);


    dev_swindow = gtk_scrolled_window_new(NULL,
                                          NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(dev_swindow), GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_ALWAYS);
    process_swindow = gtk_scrolled_window_new(NULL,
                                              NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(process_swindow), GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_ALWAYS);


    task_array = g_array_new(FALSE, TRUE, sizeof(Task));


    interrupt_array_d = g_array_new(FALSE, TRUE, sizeof(Interrupts));


    g_array_set_size(interrupt_array_d, 10);


    names_array = g_array_new(FALSE, FALSE, sizeof(Devices));


    for (int i = 0; i < 8; i++) {

        history[i] = g_array_new(FALSE, TRUE, sizeof(gfloat));
        g_array_set_size(history[i], 240);
    }


    window = main_window(dev_swindow, process_swindow);
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


    g_signal_connect(G_OBJECT(graph1), "draw",
                     G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(G_OBJECT(graph2), "draw",
                     G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(G_OBJECT(graph3), "draw",
                     G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(G_OBJECT(graph4), "draw",
                     G_CALLBACK(on_draw_event), NULL);


    g_signal_connect (window, "destroy", G_CALLBACK(destroy_window), NULL);
    g_signal_connect (G_OBJECT(process_swindow), "destroy",
                      G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect (G_OBJECT(dev_swindow), "destroy",
                      G_CALLBACK(gtk_main_quit), NULL);



    init_timeout();

    gtk_main();


    if (refresh > 0) {
        g_source_remove(refresh);

    }


    g_array_free(task_array, TRUE);
    g_array_free(names_array, TRUE);
    g_array_free(interrupt_array_d, TRUE);

    for (int i = 0; i < 8; i++) {
        g_array_free(history[i], TRUE);

    }


    close(newsockfd);
    close(newsockfd1);

    cairo_debug_reset_static_data();
       FcFini();


    return 0;
}
