
/// @file main.c
#include "drawing.h"
#include "testing_tree.h"
#include "buttons.h"

#include "functions.h"


#include "main_header.h"
#include <fontconfig/fontconfig.h>
#include <errno.h>



#include <glib.h>
#include <pango/pangocairo.h>
#include <ctype.h>
#include <netinet/in.h>

int input_port_number(char *string) {

   char     *endptr;

    long    port;
    int     n;
    errno=0;
    port= strtol(string, &endptr, 10);

    while (isspace((unsigned char ) *endptr)) {
        endptr++;
    }
    while(test_strtol(port)<0|| endptr == string || port<=0 || *endptr!='\0'){
        endptr=NULL;
        errno=0;
        printf("port number is not correct\n");
        printf("input was %s\n",string);
        printf("try again by typing in a port number\n");
        printf("if you want to exit the program type in exit\n");

        n= scanf("%255s",string);//todo add error handeling
        if (n == 1) {
        } else if (errno != 0) {
            perror("scanf");
            return -1;
        } else {
            fprintf(stderr, "No matching characters\n");
            return -1;
        }

        if(strncmp(string,"exit",4)==0){
        printf("exiting the program\n");
        return -1;
        }

        port= strtol(string, &endptr, 10);
        while (isspace((unsigned char ) *endptr)) {
            endptr++;
        }
        if (*endptr) {
           printf("random chars found %s\n",endptr);
           return -1;
        }
    }

    return 0;

}
int input_ip(char *string){

    int n=0;
    printf("ip failed\n");
    printf("ip was %s\n",string);
    printf("type new one\n");
    printf("ip needs to look like XXX.XXX.XXX.XXX\n");
    printf("where the XXX must be a positive number from 0-255\n");
    printf("if you want to exit the program type in exit\n");
    errno=0;

   n= scanf("%255s",string);
    if (n == 1) {
    } else if (errno != 0) {
        perror("scanf");
        return -1;
    } else {
        fprintf(stderr, "No matching characters\n");
        return -1;
    }
    if(strncmp(string,"exit",4)==0){
        printf("exiting the program\n");
        return -1;
    }
    return 0;

}
int validate_ip4dotted(char *string)
{
    char *temp=string;
    char *endptr;
    char *ptr;
    long num;
    int count=0;
    num = (long)strlen(string);

    while (num < 7 || num > 15){
        printf("the length of the ip address is wrong\n");
       if(input_ip(string)<0){
           return -1;
       }
        num = (long)strlen(string);
    }


    for(int i=0;i<4;i++){
        errno=0;
        ptr=string;
        count=0;
        bool dot=true;
        while(*ptr!='\0'&& *ptr!=46){
            if(*ptr<'0' ||*ptr>'9'){
                endptr  =   NULL;
                i       =   0;
                string  =   temp;
                if(input_ip(string)<0){
                    return -1;
                }

            }
            ptr++;
            count++;
            if(count>=4){
                printf("to many numbers in the address\n");
            }
            if(*ptr==46){
                printf("%d",*ptr);
            }
        }
        num= strtol(string,&endptr,10);
        if(test_strtol(num)<0 ||(num<0 || num>255)){
            endptr  =   NULL;
            i       =   0;
            string  =   temp;
            if(input_ip(string)<0){
                return -1;
            }



        }
        if(endptr){
            if(*endptr=='.'){
                endptr++;
                string=endptr;

            }
        }


    }


    return 0;
}

static void
list_fonts ()
{
    int i;
    PangoFontFamily ** families;
    int n_families;
    PangoFontMap * fontmap;

    fontmap = pango_cairo_font_map_get_default();
    pango_font_map_list_families (fontmap, & families, & n_families);
    printf ("There are %d families\n", n_families);
    for (i = 0; i < n_families; i++) {
        PangoFontFamily * family = families[i];
        const char * family_name;

        family_name = pango_font_family_get_name (family);
        printf ("Family %d: %s\n", i, family_name);
    }
    g_free (families);
}

/**
 *main(): creates a TPC socket and tries to connect to the server,if that was successful
 * it initializes the window and starts to request for data from the server;
 *
 * @param argc number of arguments port number and IP address
 * @param argv strings that should be port number and IP address
 * @return returns a non zero value if something goes wrong
 * */
int main(int argc, char *argv[]) {
   // list_fonts ();
        m_data=NULL;

        list_num_size   = 0;
        refresh         = 0;
        time_step       = 0;    /*space between data*/
        newsockfd       = 0;
        newsockfd1      = 0;
        fontdesc        = NULL;
        closed_cpu_window=false;
        int n;
        char buffer[256];
        char buffer2[256];
        memset(buffer,0,sizeof(buffer));
        memset(buffer2,0,sizeof(buffer2));




    read_config();
    check_config();



    if(argc>=3){
        strcpy(buffer,argv[1]);
        if(input_port_number(buffer)<0){
            return -1;
        }
        strcpy(buffer2,argv[2]);

        if(validate_ip4dotted(buffer2) < 0){
            return -1;
        }
    }else if(argc<3){
        printf("input port\n");
        n= scanf("%255s",buffer);//todo add error handling
        if (n == 1) {
        } else if (errno != 0) {
            perror("scanf");
            return -1;
        } else {
            fprintf(stderr, "No matching characters\n");
            return -1;
        }
        if(input_port_number(buffer)<0){
            return -1;
        }
        printf("input ip_address\n");
        n= scanf("%255s",buffer2);
        if (n == 1) {
        } else if (errno != 0) {
            perror("scanf");
            return -1;
        } else {
            fprintf(stderr, "No matching characters\n");
            return -1;
        }
        if(validate_ip4dotted(buffer2) < 0){
            return -1;
        }
    }









    while ((newsockfd = connection(buffer, buffer2)) < 0) {

        printf("want to try another address or port?\n");
        memset(buffer,0,sizeof(buffer));
        memset(buffer2,0,sizeof(buffer2));
        printf("input port\n");
        n= scanf("%255s",buffer);
        if (n == 1) {
        } else if (errno != 0) {
            perror("scanf");
            return -1;
        } else {
            fprintf(stderr, "No matching characters\n");
            return -1;
        }
        if(input_port_number(buffer)<0){
            return -1;
        }
        printf("input ip_address\n");
        n= scanf("%255s",buffer2);
        if (n == 1) {
        } else if (errno != 0) {
            perror("scanf");
            return -1;
        } else {
            fprintf(stderr, "No matching characters\n");
            return -1;
        }
        if(validate_ip4dotted(buffer2) < 0){
            return -1;
        }


    }

    if ((newsockfd1 = connection(buffer, buffer2)) < 0) {
        close(newsockfd);
        close(newsockfd1);
        return -1;
    }

    printf("connection established\n");
    if(!(cpu_num=receive_number_cpu(newsockfd))){
        printf("cpu number failed\n");
        close(newsockfd);
        close(newsockfd1);
        return -1;
    }
    printf("receive_number_cpu\n");


    fontdesc = pango_font_description_from_string (confy.font);


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




    time_step = 60000 / confy.delay;






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


        if(refresh>0){
            if (!g_source_remove(refresh)){
                g_critical ("Unable to remove source");
            }
        }

    free(cpu_status);
    free(cpu_buttons);
    I_Collection2 *temp;
    while(interrupts2){
        temp=interrupts2;
        interrupts2=interrupts2->next;
        free(temp->CPU);
        free(temp);


    }







    close(newsockfd);
    close(newsockfd1);
    free(cpuGraphs);

    pango_cairo_font_map_set_default(NULL);
//    cairo_debug_reset_static_data();
//    FcFini();



    return 0;
}
