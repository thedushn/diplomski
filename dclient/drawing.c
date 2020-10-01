//
// Created by dushn on 23.7.20..
//



//
// Created by dushn on 8.9.17..
//

#include <inttypes.h>
#include <errno.h>
#include "drawing.h"

/*
 * function on_draw_event(): creating graphs by sending the draw signal to the function we create a cairo_t structure
 *
 * input:pointer to the graph ,and pointer to cairo_t on which to draw on
 * output:if successful return true
 * */
gboolean on_draw_event(GtkWidget *widget, cairo_t *cr) {


    if (widget == graph1) {/*depending on which graph we want to draw on*/

        do_drawing_cpu(widget, cr, time_step, cpu_list);
    } else if (widget == graph2) {

        do_drawing_net(widget, cr, time_step, net_list);
    } else if (widget == graph3) {

        do_drawing_mem(widget, cr, time_step, mem_list);
    } else {

        do_drawing_int(widget, cr, interrupts);

    }


    return TRUE;

}
/*
 * function writing_interrupt_names(): writing the name of the interrupts depending on the length of the names and the
 * size of the window,divide the name depending on how much space we have and writing them in rows
 * input: pointer to the canvas, font size , length of one interrupt graph,position on the interrupt graph,
 * first part of the name of the interrupt and the second part
 * output:none.
 * */
void writing_interrupt_names(cairo_t *cr, double font_size, double length, int position, const gchar *name1,
                             const gchar *name2) {

    char * text_int;
    int counter;
    size_t row_length;//how many letters can stand in one row
    row_length = (size_t) (floor(length) - 2);/*we subtract 2 because not all letters are the same size*/
    int row_num = 0;//how many rows
    size_t name_length;
    size_t temp_length;
    if (name1[0] != '\0') {


       name_length = strlen(name1);


        while (row_length < name_length) {

            name_length -= row_length;
            row_num++;//increase num  of rows
        }
        if (row_num == 0) {

            cairo_move_to(cr, 5 * font_size + 5 * length * position, font_size);//move him to the start position
            cairo_show_text(cr, name1);
        }
        else {

            for (int r = 0; r <= row_num; r++) {
                //move him depending on how many rows we need
                cairo_move_to(cr, 5 * font_size + 5 * length * position, font_size + (r * font_size));
                temp_length = row_length;
                name_length = strlen(name1);

                if (row_length > name_length) {

                    temp_length = name_length;

                }

                if (r == row_num) {


                    cairo_show_text(cr, name1);
                } else {


                    text_int = (char *) calloc(1, sizeof(char) * temp_length + 1);

                    if(text_int==NULL){
                        printf("calloc error %d \n", errno);
                        free(text_int);
                        if (gtk_main_level() > 0)
                            gtk_main_quit();
                        return;
                    }

                    for (int s = 0; s < temp_length; s++) {


                        text_int[s] = name1[s];

                    }
                    counter = (int) temp_length;
                    for (int c = 0; c < counter; c++) {
                        name1++;
                    }

                    cairo_show_text(cr, text_int);
                    free(text_int);

                }


            }
        }

        int movement = row_num;
        row_num = 0;

        if (name2[0] != '\0') {

            name_length = strlen(name2);

            while (row_length < name_length) {

                name_length -= row_length;
                row_num++;// increase  num of rows
            }
            if (row_num == 0) {

                cairo_move_to(cr, 5 * font_size + 5 * length * position,
                              2 * font_size + movement * font_size);//move him to the start position



                cairo_show_text(cr, name2);
            } else {

                for (int r = 0; r <= row_num; r++) {
                    //move him depending on how many rows we need
                    cairo_move_to(cr, 5 * font_size + 5 * length * position,
                                  2 * font_size + movement * font_size + r * font_size);

                    temp_length = row_length;
                    name_length = strlen(name2);

                    if (row_length > name_length) {

                        temp_length = name_length;

                    }

                    if (r == row_num) {


                        cairo_show_text(cr, name2);

                    } else {

                       text_int = (char *) calloc(1, sizeof(char) * temp_length + 1);

                        if(text_int==NULL){
                            printf("calloc error %d \n", errno);
                            free(text_int);
                            if (gtk_main_level() > 0)
                                gtk_main_quit();
                            return;
                        }



                        strncpy(text_int, name2, temp_length);
                        counter = (int) temp_length;
                        for (int c = 0; c < counter; c++) {
                            name2++;
                        }
                        cairo_show_text(cr, text_int);

                        free(text_int);
                    }


                }

            }
        }
    }


}
/*
 * function checking_interrupt_names(): checking to see how many names the interrupts has and acting accordingly
 * input: pointer to the canvas, font size , length of one interrupt graph,position on the interrupt graph,
 * and four pointers to the names of the interrupts
 * output:none.
 * */
void checking_interrupt_names(cairo_t *cr, double font_size, double length, int position, const char *name1,
                              const char *name2,
                              const char *name3, const char *name4) {


    if (name4[0] != '\0') {

        writing_interrupt_names(cr, font_size, length, position, name3, name4);
    }

    if (name4[0] == '\0') {

        if (name3[0] != '\0') {
            writing_interrupt_names(cr, font_size, length, position, name2, name3);

        }

        if (name3[0] == '\0') {

            writing_interrupt_names(cr, font_size, length, position, name1, name2);

        }
    }


}
/*
 * function writing_seconds(): drawing the seconds on the graphs
 * input: pointer to the canvas,width of the graph,height of the graph, font size and position
 * output:none.
 * */
void writing_seconds(cairo_t *cr, double width, double height, double font_size, int i) {


    const char *name = "0 sec";
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, (double) i * font_size, height);
    cairo_show_text(cr, name);
    cairo_move_to(cr, (width - i * 2 * font_size) / 6 + i * font_size, height);
    cairo_show_text(cr, "10 sec");
    cairo_move_to(cr, (width - i * 2 * font_size) / 6 * 2 + i * font_size, height);
    cairo_show_text(cr, "20 sec");
    cairo_move_to(cr, (width - i * 2 * font_size) / 6 * 3 + i * font_size, height);
    cairo_show_text(cr, "30 sec");
    cairo_move_to(cr, (width - i * 2 * font_size) / 6 * 4 + i * font_size, height);
    cairo_show_text(cr, "40 sec");
    cairo_move_to(cr, (width - i * 2 * font_size) / 6 * 5 + i * font_size, height);
    cairo_show_text(cr, "50 sec");
    cairo_move_to(cr, width - i * font_size, height);
    cairo_show_text(cr, "60 s");
    cairo_stroke_preserve(cr);

    cairo_set_source_rgba(cr, .7, .7, .7, 0.5);
    for (int g = 1; g <= 5; g++) {
        cairo_move_to(cr, (width - 2 * i * font_size) / 6 * g + i * font_size, height - font_size);
        cairo_line_to(cr, (width - 2 * i * font_size) / 6 * g + i * font_size, 0);
    }
    cairo_stroke(cr);


}
/*
 * function draw_frame(): drawing the frame of the graph
 * input: pointer to the canvas,width of the graph,height of the graph, font size and position
 * output:none.
 * */
void draw_frame(cairo_t *cr, double width, double height, double font_size, int i) {



    //frame
    cairo_set_source_rgb(cr, 0, 0, 0);

    cairo_rectangle(cr, i * font_size, height - font_size, width - 2 * i * font_size, -height + font_size);
    cairo_stroke_preserve(cr);

    cairo_set_source_rgba(cr, 1.0, 0.43, 0.0, 0.3);
    cairo_fill(cr);
    //Percentage line
    cairo_set_source_rgba(cr, .7, .7, .7, 0.5);
    cairo_move_to(cr, i * font_size, (height - font_size) / 4);//75%
    cairo_line_to(cr, width - i * font_size, (height - font_size) / 4);
    cairo_move_to(cr, i * font_size, (height - font_size) / 4 * 2);//50
    cairo_line_to(cr, width - i * font_size, (height - font_size) / 4 * 2);
    cairo_move_to(cr, i * font_size, (height - font_size) / 4 * 3);//25
    cairo_line_to(cr, width - i * font_size, (height - font_size) / 4 * 3);
    cairo_stroke(cr);


}
/*
 * function draw_percentages(): drawing the percentages of the graph
 * input: pointer to the canvas,width of the graph,height of the graph, font size
 * output:none.
 * */
void draw_percentages(cairo_t *cr, double height, double font_size) {


    cairo_set_font_size(cr, font_size);
    cairo_move_to(cr, 0, font_size);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_show_text(cr, "100%");
    cairo_move_to(cr, 0, (height - font_size) / 4);
    cairo_show_text(cr, "75%");
    cairo_move_to(cr, 0, (height - font_size) / 4 * 2);
    cairo_show_text(cr, "50%");
    cairo_move_to(cr, 0, (height - font_size) / 4 * 3);
    cairo_show_text(cr, "25%");
    cairo_move_to(cr, 0, height - font_size);
    cairo_show_text(cr, "0%");
    cairo_stroke(cr);


}

/*
 * function draw_interrupts(): drawing the interrupts of the graph
 * input: pointer to the canvas,position on the graph,pointer to the interrupt ,height of the graph, font size,
 * max number of interrupts and the length of the interrupt piece
 * output:none.
 * */
void draw_interrupts(cairo_t *cr, int position, Interrupts *peak, double height, double font_size, __uint64_t max_num,
                     double length) {

    double percentage;

    cairo_set_line_width(cr, 1);


    percentage = ((height - font_size) / max_num) * peak->CPU0;


    checking_interrupt_names(cr, font_size, length, position, peak->ime1, peak->ime2, peak->ime3, peak->ime4);


    cairo_rectangle(cr, 5 * font_size + 5 * length * position, height - font_size, length - 1, -percentage);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_stroke_preserve(cr);
    cairo_set_source_rgba(cr, .7, .7, .7, 0.5);
    cairo_move_to(cr, 5 * font_size + 5 * length * position, height - font_size);
    cairo_line_to(cr, 5 * font_size + 5 * length * position, 0);
    cairo_stroke_preserve(cr);
    cairo_set_source_rgb(cr, 1, 0, 0);
    cairo_fill(cr);
    percentage = ((height - font_size) / max_num) * peak->CPU1;
    cairo_rectangle(cr, 5 * font_size + length * (5 * position + 1), height - font_size, length - 1, -percentage);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_stroke_preserve(cr);
    cairo_set_source_rgb(cr, 0, 1, 0);
    cairo_fill(cr);
    percentage = ((height - font_size) / max_num) * peak->CPU2;
    cairo_rectangle(cr, 5 * font_size + length * (5 * position + 2), height - font_size, length - 1, -percentage);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_stroke_preserve(cr);
    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_fill(cr);
    percentage = ((height - font_size) / max_num) * peak->CPU3;
    cairo_rectangle(cr, 5 * font_size + length * (5 * position + 3), height - font_size, length - 1, -percentage);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_stroke_preserve(cr);
    cairo_set_source_rgb(cr, 1, .5, 0);
    cairo_fill(cr);


}
/*
 * function draw_graph(): draws the lines on the graph
 * input: pointer to the canvas,index of the data,width,height,font size, step between
 * data, pointer to the array of data
 * output:none.
 * */
void
draw_graph(cairo_t *cr, int r, double width, double height, double font_size, double time_step, Cpu_list *array) {

    Cpu_list *temp = array;
    double prev = height - font_size; //zero
    double step = 0;


    if (r == 0) {

        cairo_set_source_rgb(cr, 1, 0, 0);//rgb
    } else if (r == 1 ) {

        cairo_set_source_rgb(cr, 0, 1, 0);
    } else if (r == 2 ) {

        cairo_set_source_rgb(cr, 0, 0, 1);
    }  else if (r == 3 ) {

        cairo_set_source_rgb(cr, 1, 0.5, 0);
    }




    int g = (int) time_step;
    if (list_num_size < time_step) {
        g = list_num_size;
    }


    for (int j = 0; j < g; j++) {

        gfloat peak;
        double percentage;

        peak = temp->data[r];
        temp = temp->next;


        cairo_move_to(cr, 3 * font_size, prev);



            percentage = ((height - font_size) / 100) * peak;
            step = (width - 3 * font_size - 3 * font_size) / time_step;


        prev = height - font_size - percentage;

        if (percentage == height) {
            percentage = height - 1;
        }


        cairo_line_to(cr, step + 3 * font_size, height - font_size - percentage);

        cairo_translate(cr, step, 0);


    }


    cairo_line_to(cr, 3 * font_size, height - font_size);/* the last line always touches the floor*/

    cairo_stroke(cr);


    cairo_translate(cr, -step * g, 0);  /*return to the start of the graph*/


}
/*
 * function draw_graph_net_mem(): draws the lines on the graph for memory usage or network usage
 * input: pointer to the canvas,index of the data,type of graph,width,height,font size, step between
 * data,max number for (network usage), pointer to the array of data
 * output:none.
 * */
void
draw_graph_net_mem(cairo_t *cr, int r, int i, double width, double height, double font_size, double time_step,
                   float max_num, NetMem_list *array) {

    NetMem_list *temp;
    double prev = height - font_size; //zero
    double step = 0;





    __int32_t g = (__int32_t) time_step;
    if (list_num_size < time_step) {
        g = list_num_size;
    }



    if (r == 0) {
        cairo_set_source_rgb(cr, 1, 0, 0);//rgb
    } else {
        cairo_set_source_rgb(cr, 0, 1, 0);
    }
    temp=array;
    for (__int32_t j = 0; j < g; j++) {

        gfloat peak;
        double percentage;

        peak = temp->data[r];
        temp = temp->next;


        cairo_move_to(cr, i * font_size, prev);


        if (i==5) {/*if its network usage we move the graph by 5 so we can have space for the data/s stat on the right*/


            percentage = ((height - font_size) / max_num) * peak;
            step = (width - 5 * font_size - 5 * font_size) / time_step;
        } else {/*if its memory usage we move the graph by 3 so we can have space for the percentage number on the right*/
            percentage = ((height - font_size) / 100) * peak;
            step = (width - 3 * font_size - 3 * font_size) / time_step;
        }

        prev = height - font_size - percentage;

        if (percentage == height) {
            percentage = height - 1;
        }


        cairo_line_to(cr, step + i * font_size, height - font_size - percentage);

        cairo_translate(cr, step, 0);


    }


    cairo_line_to(cr, i * font_size, height - font_size);/* the last line always touches the floor*/

    cairo_stroke(cr);


    cairo_translate(cr, -step * g, 0);  /*return to the start of the graph*/



}

/*
 * function do_drawing_mem(): draws the entire graph from the frame to the percetages to the lines
 * input:pointer to the graph, pointer to the canvas,step between data and the pointer to the array of data
 * output:none.
 * */
void do_drawing_mem(GtkWidget *widget, cairo_t *cr, guint time_step, NetMem_list *array) {
    double width, height;
    height = (double) gtk_widget_get_allocated_height(widget);
    width = (double) gtk_widget_get_allocated_width(widget);


    cairo_surface_t *graph_surface;
    graph_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int) width, (int) height);


    cairo_set_line_width(cr, 1);
    double font_size = 10;

    cairo_set_font_size(cr, font_size);

    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);


    cairo_set_font_size(cr, font_size);



    draw_frame(cr, width, height, font_size, 3);




    draw_percentages(cr, height, font_size);


    writing_seconds(cr, width, height, font_size, 3);


    draw_graph_net_mem(cr, 0, 3, width, height, font_size, time_step, 0, array);
    draw_graph_net_mem(cr, 1, 3, width, height, font_size, time_step, 0, array);

    if (graph_surface != NULL) {
        cairo_set_source_surface(cr, graph_surface, 0.0, 0.0);
        cairo_paint(cr);
        cairo_surface_destroy(graph_surface);
    }


}
/*
 * function do_drawing_int(): draws the entire graph, by searching the interrupt with the highest number of interrupts
 * input:pointer to the graph, pointer to the canvas and pointer to the array of interrupts
 * output:none.
 * */
void do_drawing_int(GtkWidget *widget, cairo_t *cr, Interrupts *interrupts1) {

    double width, height;

    gchar name[3];
    __uint64_t max_num = 0;

    double length;
    Interrupts *peak;
    Interrupts *temp_p;

    gchar num[5];

    cairo_surface_t *graph_surface;


    height = (double) gtk_widget_get_allocated_height(widget);
    width = (double) gtk_widget_get_allocated_width(widget);
    double font_size = 10;


    cairo_set_line_width(cr, 1);
    cairo_set_font_size(cr, font_size);

    graph_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int) width, (int) height);


    draw_frame(cr, width, height, font_size, 5);
    cairo_set_source_rgb(cr, 0, 0, 0);

    temp_p = interrupts1;
    for (int i = 0; i < 10; i++) {

        cairo_move_to(cr, 5 * font_size, height);

        peak = temp_p;

        if (max_num <= peak->CPU0) {

            max_num = peak->CPU0;
        }
        if (max_num <= peak->CPU1) {

            max_num = peak->CPU1;
        }
        if (max_num <= peak->CPU2) {

            max_num = peak->CPU2;
        }
        if (max_num <= peak->CPU3) {

            max_num = peak->CPU3;
        }
        temp_p++;

    }

    max_num = max_num * 5 / 4;
    cairo_move_to(cr, 0, font_size);
    sprintf(num, "%"PRIu64, max_num);
    cairo_show_text(cr, num);
    for (int i = 1; i <= 3; i++) {


        cairo_move_to(cr, 0, (height - font_size) / 4 * (4 - i));
        sprintf(num, "%"PRIu64, (max_num / 4 * i));
        cairo_show_text(cr, num);


    }

    cairo_move_to(cr, 0, height - font_size);
    cairo_show_text(cr, "0");
    length = (width - (5 * font_size * 2)) / 5 / 10;
    temp_p = interrupts1;
    for (int i = 0; i < 10; i++) {


        peak = temp_p;

        cairo_move_to(cr, 5 * font_size + 5 * length * (i), height);
        sprintf(name, "%s", peak->irq);

        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_show_text(cr, name);

        draw_interrupts(cr, i, peak, height, font_size, max_num, length);
        temp_p++;
    }

    if (graph_surface != NULL) {
        cairo_set_source_surface(cr, graph_surface, 0.0, 0.0);
        cairo_paint(cr);
        cairo_surface_destroy(graph_surface);
    }


}
/*
 * function do_drawing_net(): draws the entire graph, by searching the array for the biggest number
 * input:pointer to the graph, pointer to the canvas, step between data, and pointer to the array of network usage
 * output:none.
 * */
void do_drawing_net(GtkWidget *widget, cairo_t *cr, guint time_step, NetMem_list *array) {

    double width, height;

    char num[5];
    gchar *track;

    float rec_bytes;

    gchar *kb = "KB/s";
    gchar *mb = "MB/s";
    gchar *b = "b/s";
    gfloat max_num = 0;


    float peak;
    float font_size = 10;
    height = (double) gtk_widget_get_allocated_height(widget);
    width = (double) gtk_widget_get_allocated_width(widget);


    NetMem_list *temp;


    cairo_surface_t *graph_surface;

    graph_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int) width, (int) height);

    cairo_set_line_width(cr, 1);


    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);


    cairo_set_font_size(cr, font_size);


    draw_frame(cr, width, height, font_size, 5);


    writing_seconds(cr, width, height, font_size, 5);
    cairo_stroke(cr);

    int g = (int) time_step;
    if (list_num_size < time_step) {
        g = list_num_size;
    }
    for (int j = 0; j <= 1; j++) {
        temp = array;
        for (int i = 0; i < g; i++) {

            peak = temp->data[j];// kb
            if (max_num <= peak) {

                max_num = peak;
            }
            temp = temp->next;

        }
    }


    cairo_set_source_rgb(cr, 0, 0, 0);

    if (max_num > 1024) {


        rec_bytes = max_num / 1024;//mb

        rec_bytes += 1;
        track = mb;

        max_num = max_num + 1024;

    } else if (max_num <= 1024 && max_num > 1) {

        rec_bytes = max_num;//kb

        rec_bytes += 100;
        track = kb;
        max_num = max_num + 100;


    } else {


        rec_bytes = max_num * 1024;//bytes


        track = b;
        max_num = max_num + 1024;


    }


    cairo_move_to(cr, 0, font_size);//biggest num
    sprintf(num, "%.1f", rec_bytes);

    cairo_show_text(cr, num);

    cairo_show_text(cr, track);
    for (int i = 1; i <= 3; i++) {


        cairo_move_to(cr, 0, (height - font_size) / 4 * (4 - i));
        sprintf(num, "%.1f", rec_bytes / 4 * (float) i);
        cairo_show_text(cr, num);
        cairo_show_text(cr, track);


    }
    cairo_move_to(cr, 0, (height - font_size));
    cairo_show_text(cr, "0");
    cairo_show_text(cr, track);


    draw_graph_net_mem(cr, 0, 5, width, height, font_size, time_step, max_num, array);
    draw_graph_net_mem(cr, 1, 5, width, height, font_size, time_step, max_num, array);


    if (graph_surface != NULL) {
        cairo_set_source_surface(cr, graph_surface, 0.0, 0.0);
        cairo_paint(cr);
        cairo_surface_destroy(graph_surface);
    }


}
/*
 * function do_drawing_cpu(): draws the entire graph, the lines, the frame the seconds and the percentage
 * input:pointer to the graph, pointer to the canvas, step between data, and pointer to the array of cpu usage
 * output:none.
 * */
void do_drawing_cpu(GtkWidget *widget, cairo_t *cr, guint time_step, Cpu_list *array1) {

    double width, height;
    double font_size = 10;
    bool *temp_bool=cpu_status;

    height = (double) gtk_widget_get_allocated_height(widget);
    width = (double) gtk_widget_get_allocated_width(widget);


    cairo_surface_t *graph_surface;
    graph_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int) width, (int) height);

    cairo_set_line_width(cr, 1);


    cairo_set_font_size(cr, font_size);

    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);


    draw_frame(cr, width, height, font_size, 3);
    draw_percentages(cr, height, font_size);


    for(__int32_t i=0;i<CPU_NUM;i++){/*draws the lines*/

        if((*temp_bool)==true){
            draw_graph(cr, i, width, height, font_size, time_step, array1);
        }
        temp_bool++;


    }

    writing_seconds(cr, width, height, font_size, 3);

    if (graph_surface != NULL) {
        cairo_set_source_surface(cr, graph_surface, 0.0, 0.0);
        cairo_paint(cr);
        cairo_surface_destroy(graph_surface);


    }

}



