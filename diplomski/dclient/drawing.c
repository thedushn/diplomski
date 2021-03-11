//
// Created by dushn on 23.7.20..
//



//
// Created by dushn on 8.9.17..
//

#include <inttypes.h>
#include <errno.h>
#include "drawing.h"
#include <assert.h>
/*
 * function on_draw_event(): creating graphs by sending the draw signal to the function we create a cairo_t structure
 *
 * input:pointer to the graph ,and pointer to cairo_t on which to draw on
 * output:if successful return true
 * */
gboolean on_draw_event(GtkWidget *widget, cairo_t *cr) {


    if (widget == graph1) {/*depending on which graph we want to draw on*/

        do_drawing_cpu(widget, cr, time_step, cpu_list);
    } else if (widget == graph_net) {

        do_drawing_net(widget, cr, time_step, net_list);
    } else if (widget == graph_mem) {

        do_drawing_mem(widget, cr, time_step, mem_list);
    } else {
            //TODO set size to the proper amount of interrupts
        gtk_widget_set_size_request(widget,50*interrupt_num+100,gtk_widget_get_allocated_height(interrupts_swindow));
       // assert(interrupts);
       // do_drawing_int(widget, cr, interrupts);
        do_drawing_int2(widget, cr, interrupts2);






    }


    return TRUE;

}
void writing_interrupt_names2(cairo_t *cr, double font_size, double length, int position, const gchar *name1) {
    PangoLayout *layout;



    cairo_move_to(cr, 5 * font_size + 5 * length * position, 0);

    layout  = gtk_widget_create_pango_layout (graph_inttrp, name1 );
    printf("Width %d \n",pango_layout_get_width(layout));
    pango_layout_set_font_description (layout, fontdesc);
    pango_layout_set_width (layout,
                            (int)(5*font_size*1000));
    pango_layout_set_wrap (layout, PANGO_WRAP_WORD_CHAR);
    pango_layout_set_ellipsize (layout,
                                PANGO_ELLIPSIZE_END);

    pango_layout_set_height (layout,(int)(5*font_size*1000));
    pango_cairo_show_layout (cr, layout);
    g_object_unref (layout);




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
void draw_interrupts2(cairo_t *cr, int position, Interrupts2 *peak, double height, double font_size, __int64_t max_num,
                     double length) {

    double percentage;

    writing_interrupt_names2(cr,font_size,length,position,peak->name);
    cairo_set_line_width(cr, 1);

    for(int i=0;i<cpu_num;i++){
        percentage = ((height - font_size) / max_num) * peak->CPU[i];

        cairo_rectangle(cr, (int)(cpu_num+1) * font_size + length * ((int)(cpu_num+1)*position+i), height - font_size, length - 1, -percentage);
       // cairo_set_source_rgb(cr, 0, 0, 0);

        cairo_set_source_rgb(cr, 1, 0, 0);
        cairo_stroke_preserve(cr);


//        cairo_move_to(cr, 5 * font_size + length * (5*position+i), height - font_size);
//        cairo_line_to(cr, 5 * font_size + length * (5*position+i), 0);
//        cairo_stroke_preserve(cr);
//        cairo_set_source_rgb(cr, 1, 0, 0); //todo change colors

        cairo_fill(cr);
    }
  //  cairo_set_source_rgb(cr, 1, 0, 0); //todo change colors
    for(int i=0;i<cpu_num+2;i++) {


        cairo_set_source_rgba(cr, i+.7, i+.7, i+.7, 0.5);
        cairo_move_to(cr, (int)(cpu_num+1) * font_size + length * ((int)(cpu_num+1) * position + i), height - font_size);

        cairo_line_to(cr, (int)(cpu_num+1) * font_size + length * ((int)(cpu_num+1) * position + i), 0);
        cairo_stroke_preserve(cr);


        cairo_fill(cr);
    }
    for(int i=0;i<cpu_num;i++) {


        cairo_set_source_rgb(cr,0,1,1);
        cairo_move_to(cr, (int)(cpu_num+1) * font_size + length * ((int)(cpu_num+1) * position + i), height - font_size);
//cairo_show_text(cr,"da");
        //cairo_line_to(cr, (int)(cpu_num+1) * font_size + length * ((int)(cpu_num+1) * position + i), 0);
        cairo_stroke_preserve(cr);


        cairo_fill(cr);
    }





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
           // step = (width - 5 * font_size - 5 * font_size) / time_step;
        } else {/*if its memory usage we move the graph by 3 so we can have space for the percentage number on the right*/
            percentage = ((height - font_size) / 100) * peak;
          //  step = (width - 3 * font_size - 3 * font_size) / time_step;
        }
        step = (width - i * font_size - i * font_size) / time_step;

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
void do_drawing_int2(GtkWidget *widget, cairo_t *cr, I_Collection2 *interrupts1) {


    double width, height;

    gchar name[3];
    __int64_t max_num = 0;
    __uint64_t *p      = NULL;



    I_Collection2 *temp_p;

    gchar num[5];

    cairo_surface_t *graph_surface;



    double font_size = 10;
    height = (double)   gtk_widget_get_allocated_height(widget);
    width = (double)    gtk_widget_get_allocated_width(widget);

    cairo_set_line_width(cr, 1);
    cairo_set_font_size(cr, font_size);

    graph_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int) width, (int) height);


    draw_frame(cr, width, height, font_size, 5);
    cairo_set_source_rgb(cr, 0, 0, 0);

    temp_p = interrupts1;
    cairo_move_to(cr, 5 * font_size, height);
    while(temp_p) {

        p=temp_p->interrupts.CPU;
        for(int i=0;i<cpu_num;i++){
            if(max_num<*p){
                max_num=*p;
            }
            p++;
        }


        temp_p=temp_p->next;

    }

    max_num = max_num * 5 / 4;
    cairo_move_to(cr, 0, font_size);
    sprintf(num, "%"PRIi64, max_num);
    cairo_show_text(cr, num);
    for (int i = 1; i <= 3; i++) {


        cairo_move_to(cr, 0, (height - font_size) / 4 * (4 - i));
        sprintf(num, "%"PRIi64, (max_num / 4 * i));
        cairo_show_text(cr, num);


    }

    cairo_move_to(cr, 0, height - font_size);
    cairo_show_text(cr, "0");

    temp_p = interrupts1;
    int i=0;
    while(temp_p) {




        // cairo_move_to(cr, 5 * font_size + 5 * length * (i), height);
        cairo_move_to(cr,(5 * font_size )+ 5 * font_size * (i), height);
        sprintf(name, "%s", temp_p->interrupts.irq);

        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_show_text(cr, name);

        draw_interrupts2(cr, i, &temp_p->interrupts, height, font_size, max_num, font_size);
        temp_p=temp_p->next;
        i++;
    }
    // printf("i %d\n",i);
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
    if (list_num_size < time_step) {/*how many elements do we have in an array*/
        g = list_num_size;
    }
    for (int j = 0; j <= 1; j++) {/*seaching for the highest number in network usage*/
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

    if (max_num > 1024) {/*if highest number is larger then a mb*/


        rec_bytes = max_num / 1024;//mb

        rec_bytes += 1;
        track = mb;

        max_num = max_num + 1024;

    } else if (max_num <= 1024 && max_num > 1) {/*if highest number is less then a mb and bigger then a kb*/

        rec_bytes = max_num;//kb

        rec_bytes += 100;
        track = kb;
        max_num = max_num + 100;


    } else { /*if its lesser then a kb*/


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
/**
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


    for(__int32_t i=0;i<cpu_num;i++){/*draws the lines*/

        if((*temp_bool)==true){/*display the cpus we want to be displayed*/
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





