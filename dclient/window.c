//
// Created by dushn on 23.7.20..
//

#include "window.h"
#include "testing_tree.h"
#include "buttons.h"
#include "drawing.h"


/**
 * cpuWindow() creates a window that contains graphs of all the individual cpu stats
 * @param cpuNumber number of cpus
 * @param graphList double pointer to the list of graphs
 * @return a pointer to the window we created
 * */
GtkWidget *cpuWindow(int cpuNumber, GtkWidget **graphList){
    GtkWidget *graphP;
    graphP=malloc((size_t)cpuNumber * sizeof(GtkWidget));
    GtkWidget *window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL); /*creates the window*/
    gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window1), 800, 400);
    GtkWidget  *vbox            = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    for(int i=0; i < cpuNumber; i++){
        graphP[i]=*gtk_drawing_area_new();
        gtk_box_pack_start(GTK_BOX(vbox), &graphP[i], 1, TRUE, 0);
        g_signal_connect_data((GObject *) &graphP[i], "draw", G_CALLBACK(on_draw_event), NULL, NULL, 0);
    }
    gtk_container_add(GTK_CONTAINER(window1), vbox);
    *graphList=graphP;

    gtk_widget_show_all(window1);
    gtk_window_set_title(GTK_WINDOW(window1), "CPU_monitor");
    return window1;
}







/**
 * swap_change(): inputs swap usage into list and displays it textually in window
 * @param memoryUsage pointer to memory usage
 * @returns void
 * */
void swap_change(Memory_usage *memoryUsage) {


    gchar *swapTotal, *swapUsed;



    m_data->mem_stats[1]=(float) strtof(memoryUsage->swap_percentage, NULL);
    if(test_strtof(m_data->mem_stats[1])){
        g_application_quit(G_APPLICATION(gtkApplication));
    }

    swapUsed  = g_format_size_full((guint64) memoryUsage->swap_used, G_FORMAT_SIZE_IEC_UNITS);
    swapTotal = g_format_size_full((guint64) memoryUsage->swap_total, G_FORMAT_SIZE_IEC_UNITS);




    gchar *swapUsageText = g_strdup_printf(("SWAP: %0.2f%% (%s) %s"), m_data->mem_stats [1], swapUsed, swapTotal);
    gtk_label_set_text(GTK_LABEL (label_swap), swapUsageText);

    g_free(swapUsageText);
    g_free(swapTotal);
    g_free(swapUsed);

}
/**
 * memory_change(): inputs memory usage into list and displays it textually in window
 * @param memoryUsage pointer to memory usage
 * @return void
 * */
void memoryChange(Memory_usage *memoryUsage) {


    gchar *used, *total, *memoryUsageText1;

    m_data->mem_stats [0]   = (float) strtof(memoryUsage->memory_percentage, NULL);

    if(test_strtof( m_data->mem_stats [0])){
        g_application_quit(G_APPLICATION(gtkApplication));
    }

    used                = g_format_size_full((guint64) memoryUsage->memory_used, G_FORMAT_SIZE_IEC_UNITS);

    total               = g_format_size_full((guint64) memoryUsage->memory_total, G_FORMAT_SIZE_IEC_UNITS);


    memoryUsageText1  = g_strdup_printf(("Memory: %0.2f%%(%s)%s"), m_data->mem_stats [0], used, total);
    gtk_label_set_text(GTK_LABEL (label_mem), memoryUsageText1);

    g_free(memoryUsageText1);
    g_free(total);
    g_free(used);

}
/**
 * function cpu_change(): inputs cpu usage into list and displays it textually in window
 * @param cpu_usage pointer to structure of  cpu_usage usage
 * @return void
 * */
void cpu_change(Cpu_usage *cpu_usage) {



    gchar  *cpu0UsageText =NULL;
    gchar  *temp_char     =NULL;

    for(int i=0;i<cpu_num;i++){


        m_data->cpu_stats[i]= strtof (cpu_usage->percentage[i],NULL) ;

        if(test_strtof( m_data->cpu_stats[i])){
            g_application_quit(G_APPLICATION(gtkApplication));
        }
       }

    for(int i=0;i<cpu_num;i++){
        gchar *p;
        if(i==0){
            cpu0UsageText=g_strdup_printf("CPU%d: %.4s%% ", i, cpu_usage->percentage[i]);

        }else{
           p=cpu0UsageText;
            temp_char=g_strdup_printf("CPU%d: %.4s%% ",i,cpu_usage->percentage[i]);
            cpu0UsageText=  g_strconcat(temp_char, p, NULL);
            g_free(p);
            g_free(temp_char);
        }


    }




    gtk_label_set_text(GTK_LABEL (label_cpu0), cpu0UsageText);

    g_free(cpu0UsageText);



}
/**
 *network_change(): inputs network usage into list and displays it textually in window
 * @param network   pointer to  Network usage
 * @return void
 * */
void network_change(Network *network) {



    m_data->network[0]    =     network->received_bytes ;
    m_data->network[1]    =     network->transmited_bytes ;


    gchar *recBytes = g_format_size_full(network->received_bytes, G_FORMAT_SIZE_IEC_UNITS);
    gchar *trBytes  = g_format_size_full(network->transmited_bytes, G_FORMAT_SIZE_IEC_UNITS);

    gchar *networkUsageReceivedText      = g_strdup_printf("RECEIVED:  %s/s", recBytes);
    gchar *networkUsageTransmittedText   = g_strdup_printf("TRANSMITTED: %s/s", trBytes);

    gtk_label_set_text(GTK_LABEL (label_rec), networkUsageReceivedText);
    gtk_label_set_text(GTK_LABEL (label_trans), networkUsageTransmittedText);

    g_free(recBytes);
    g_free(trBytes);
    g_free(networkUsageReceivedText);
    g_free(networkUsageTransmittedText);

}


