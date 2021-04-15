
/// @file main.c
#include "drawing.h"
#include "testing_tree.h"
#include "buttons.h"

#include "functions.h"


#include "main_header.h"
#include <fontconfig/fontconfig.h>
#include <errno.h>




/**
 *main(): creates a TPC socket and tries to connect to the server,if that was successful
 * it initializes the window and starts to request for data from the server;
 *
 * @param argc number of arguments port number and IP address
 * @param argv strings that should be port number and IP address
 * @return returns a non zero value if something goes wrong
 * */
int main(int argc, char *argv[]) {

        m_data=NULL;
        list_num_size   = 0;
        t               = 1000; /*what delay we want when asking for data*/
        refresh         = 0;
        time_step       = 0;    /*space between data*/
        device_all      = false;
        record          = false;
        newsockfd       = 0;
        newsockfd1      = 0;
        fontdesc        = NULL;
        closed_cpu_window=true;




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


    if(!(cpu_num=receive_number_cpu(newsockfd))){
        printf("cpu number failed\n");
        close(newsockfd);
        close(newsockfd1);
        return -1;
    }


    fontdesc = pango_font_description_from_string ("Arial 8");


    cpu_status=calloc((size_t)cpu_num,sizeof(bool));
    if(cpu_status==NULL){
        free(cpu_status);
        close(newsockfd);
        close(newsockfd1);
        printf("calloc error %d \n", errno);
        return -1;
    }

    for(int i=0;i<cpu_num;i++){
        cpu_status[i]=true;
    }

    cpu_buttons=calloc((size_t)cpu_num,sizeof(GtkWidget));
    if(cpu_buttons==NULL){

        free(cpu_status);
        free(cpu_buttons);
        close(newsockfd);
        close(newsockfd1);
        printf("calloc error %d \n", errno);
        return -1;
    }

    gtk_disable_setlocale(); /*sscanf and sprintf don't work if this isn't used */


    gtkApplication = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);




    time_step = 60000 / t;



    sem_init(&semt,0,1);


    g_signal_connect (gtkApplication, "activate", G_CALLBACK (activate), NULL);
    g_application_run (G_APPLICATION (gtkApplication), 0, NULL);



    pango_font_description_free (fontdesc);

    g_object_unref (gtkApplication);

    gtk_tree_store_clear(list_tasks); /*freeing memory allocated for task list*/
    gtk_tree_store_clear(list_devices);/*freeing memory allocated for device list*/
    g_object_unref(list_tasks);
    g_object_unref(list_devices);

    free_mega_data(&m_data);
    freeingMemory(devices_old,&dev_num_old,DEVICES);
    freeingMemory(tasks_old,&task_num_old,TASK);


    free(cpu_status);
    free(cpu_buttons);
    I_Collection2 *temp;
    while(interrupts2){
        temp=interrupts2;
        interrupts2=interrupts2->next;
        free(temp->interrupts.CPU);
        free(temp);


    }



    sem_destroy(&semt);



    close(newsockfd);
    close(newsockfd1);
    free(cpuGraphs);

    pango_cairo_font_map_set_default(NULL);
  //  cairo_debug_reset_static_data();
   // FcFini();



    return 0;
}
