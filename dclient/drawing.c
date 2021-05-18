//
// Created by dushn on 23.7.20..
//

#include <inttypes.h>
#include "drawing.h"
#include "buttons.h"
#include <stdio.h>
#include <string.h>

#define MAX_ELEMENTS 240
/**
 * function on_draw_event(): creating graphs by sending the draw signal to the function we create a cairo_t structure
 *
 * @param  widget pointer to the graph in which to draw
 * @param  cr     canvas
 * @return true if function completed
 * */
gboolean on_draw_event(GtkWidget *widget, cairo_t *cr) {


    if (widget == graphCpu) {/*depending on which graph we want to draw on*/
        //gtk_widget_set_size_request(widget,(int)50*interrupt_num+100,gtk_widget_get_allocated_height(cpu_swindow));
        do_drawing_cpu(widget, cr, time_step, m_data);
    } else if (widget == graphNet) {

        do_drawing_net(widget, cr, time_step, m_data);
    } else if (widget == graphMem) {

        do_drawing_mem(widget, cr, time_step, m_data);
    } else if (widget == graph_inttrp){

        gtk_widget_set_size_request(widget,(int)50*interrupt_num+100,gtk_widget_get_allocated_height(interrupts_swindow));
       // assert(interrupts);
       // do_drawing_int(widget, cr, interrupts);
        do_drawing_int2(widget, cr, m_data->interrupts_list);






    }else{
        if(closed_cpu_window) {
            for (int i = 0; i < cpu_num; i++) {
                if (widget == &cpuGraphs[i]) {
                    do_drawing_one_cpu(widget, cr, time_step, m_data,i);
                }
            }
        }

    }



    return TRUE;

}
/** writing_interrupt_names2 writes the names on the canvas depending on their length position on the canvas and
 * font size
 * @param cr canvas
 * @param font_size
 * @param length
 * @param position
 * @param name1
 * @return void
 * */
void writing_interrupt_names2(cairo_t *cr, double font_size, double length, int position, const gchar *name1) {

    PangoLayout *layout;


    cairo_move_to(cr, 5 * font_size + 5 * length * position, 0);

    layout  = gtk_widget_create_pango_layout (graph_inttrp, name1 );
    //printf("Width %d \n",pango_layout_get_width(layout));
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




/**
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


    cairo_stroke(cr);


}
void writing_seconds2(cairo_t *cr, double width, double height, double font_size, int i,int time_lenght) {
    const char *name = "0 sec";
    gchar *time_string=NULL;
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, (double) i * font_size, height);
    cairo_show_text(cr, name);
    for(int j=10;j<=60;j+=10){
        cairo_move_to(cr, (width - i * 2 * font_size) / time_lenght *1000*j + i * font_size, height);

        time_string=  g_strdup_printf("%d sec",j);

        cairo_show_text(cr, time_string);
        g_free(time_string);
    }

if(gl_delay>60000){

    for(int j=60;j<=gl_delay/1000;j+=10){
        cairo_move_to(cr, (width - i * 2 * font_size) / time_lenght *1000*j + i * font_size, height);

        time_string=  g_strdup_printf("%d sec",j);

        cairo_show_text(cr, time_string);
        g_free(time_string);
    }
}
    cairo_move_to(cr, width - i * font_size, height);
    time_string=  g_strdup_printf("%.3f sec",(double)time_lenght/1000);

    cairo_show_text(cr, time_string);
    cairo_stroke_preserve(cr);
    g_free(time_string);

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
/**
 * draw_percentages(): drawing the percentages of the graph
 * @param cr canvas
 * @param height
 * @param font_size
 * @return void
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
void draw_interrupts2(cairo_t *cr, int position, I_Collection2 *peak, double height, double font_size, __uint64_t max_num,
                      double length) {

    double percentage;

    writing_interrupt_names2(cr,font_size,length,position,peak->interrupts.name);
    cairo_set_line_width(cr, 1);

    for(int i=0;i<cpu_num;i++){
        if(max_num==0){
            percentage=font_size;
        }else{
            percentage = ((height - font_size) / (double)max_num) * (double)peak->CPU[i];
        }


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
    for(int i=0;i<=cpu_num;i++) {




        cairo_move_to(cr, (int)(cpu_num+1) * font_size + length * ((int)(cpu_num+1) * position + i), height - font_size);
      //  cairo_set_source_rgb(cr,1,1,1);

      //  cairo_move_to(cr, (int)(cpu_num+1) * font_size + length * ((int)(cpu_num+1) * position + i), height - font_size);
        cairo_set_source_rgba(cr, i+.7, i+.7, i+.7, 0.5);
        cairo_line_to(cr, (int)(cpu_num+1) * font_size + length * ((int)(cpu_num+1) * position + i), 0);


        cairo_stroke_preserve(cr);


        cairo_fill(cr);
    }
    for(int i=0;i<cpu_num;i++) {

        char temp_c[32];
        memset(temp_c,0,sizeof(temp_c));
        sprintf(temp_c,"%d",i);
        cairo_set_source_rgb(cr,1,1,1);
        cairo_move_to(cr, (int)(cpu_num+1) * font_size + length * ((int)(cpu_num+1) * position + i), height - font_size);
        cairo_show_text(cr,temp_c);

        cairo_stroke_preserve(cr);


        cairo_fill(cr);
    }





}

/**
 * function draw_graph(): draws the lines on the graph
 * input: pointer to the canvas,index of the data,width,height,font size, step between
 * data, pointer to the array of data
 * output:none.
 * */
void
draw_graph(cairo_t *cr, int r, double width, double height, double font_size, double time_step, Mega_Data *array) {
    int corner      = 5;
    Mega_Data *temp = array;
    double prev     = height - font_size; //zero
    double step     = 0;
    double counter  = 0;
    gfloat peak;
    double percentage;

    if (r == 0) {

        cairo_set_source_rgb(cr, 1, 0, 0);//rgb
    } else if (r == 1 ) {

        cairo_set_source_rgb(cr, 0, 1, 0);
    } else if (r == 2 ) {

        cairo_set_source_rgb(cr, 0, 0, 1);
    }  else if (r == 3 ) {

        cairo_set_source_rgb(cr, 1, 0.5, 0);
    }







    while(temp){



        peak = temp->cpu_stats[r];



        cairo_move_to(cr, corner * font_size, prev);



            percentage  = ((height - font_size) / 100) * peak;
            step        = (width - corner * font_size - corner * font_size) /(double)temp->gtk_delay;




        prev = height - font_size - percentage;

        if (percentage == height) {
            percentage = height - 1;
        }


        cairo_line_to(cr, step + corner * font_size, height - font_size - percentage);

        cairo_translate(cr, step, 0);
        counter+=step;

        temp = temp->next;
    }


    cairo_line_to(cr, corner * font_size, height - font_size);/* the last line always touches the floor*/

    cairo_stroke(cr);


    cairo_translate(cr, -counter , 0);  /*return to the start of the graph*/
//    cairo_translate(cr, -step * g, 0);  /*return to the start of the graph*/

}
/**
 * function draw_graph_net(): draws the lines on the graph for network usage
 *
 *
 *  @param  cr canvas
 *  @param  r  color
 *  @param  i index
 *  @param  width
 *  @param  height
 *  @param  font_size
 *  @param  time_step
 *  @param  max_num  maximum bytes in the list
 *  @param  array data array
 *  @return void
 * */
void
draw_graph_net(cairo_t *cr, int r, int i, double width, double height, double font_size, double time_step,
               __uint64_t max_num, Mega_Data *array) {

    Mega_Data *temp;
    double prev = height - font_size; //zero
    double step = 0;
    double zero_p= height - font_size;





    __int32_t g = (__int32_t) time_step;
    if (list_num_size < time_step) {
        g = list_num_size;
    }

    __uint64_t peak;
    double percentage;

    if (r == 0) {
        cairo_set_source_rgb(cr, 1, 0, 0);//rgb
    } else {
        cairo_set_source_rgb(cr, 0, 1, 0);
    }
    temp=array;
    for (__int32_t j = 0; j < g; j++) {



        peak = temp->network[r];
       // peak = temp->net_stats[r];
        temp = temp->next;


        cairo_move_to(cr, i * font_size, prev);




            if(max_num==0 || peak ==0){
                percentage=0;
            }else
            percentage = (zero_p / max_num) * (double)peak;
            // step = (width - 5 * font_size - 5 * font_size) / time_step;

        step = (width - i * font_size - i * font_size) / time_step;

        prev = zero_p - percentage;

        if (percentage == height) {
            percentage = height - 1;
        }


        cairo_line_to(cr, step + i * font_size, zero_p - percentage);

        cairo_translate(cr, step, 0);


    }

    /* the last line always touches the floor*/
    cairo_line_to(cr, i * font_size, zero_p);

    cairo_stroke(cr);


    cairo_translate(cr, -step * g, 0);  /*return to the start of the graph*/



}
/**
 * function draw_graph_mem(): draws the lines on the graph for memory usage
 *  @param  cr canvas
 *  @param  r  color
 *  @param  index index
 *  @param  width
 *  @param  height
 *  @param  font_size
 *  @param  time_step
 *  @param  array data array
 *  @return void
 * */
void
draw_graph_mem(cairo_t *cr, int r, int index, double width, double height, double font_size, double time_step,
               Mega_Data *array) {

    Mega_Data *temp;
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

        peak = temp->mem_stats[r];
        temp = temp->next;


        cairo_move_to(cr, index * font_size, prev);



            percentage = ((height - font_size) / 100) * peak;
            //  step = (width - 3 * font_size - 3 * font_size) / time_step;

        step = (width - index * font_size - index * font_size) / time_step;

        prev = height - font_size - percentage;

        if (percentage == height) {
            percentage = height - 1;
        }


        cairo_line_to(cr, step + index * font_size, height - font_size - percentage);

        cairo_translate(cr, step, 0);


    }


    cairo_line_to(cr, index * font_size, height - font_size);/* the last line always touches the floor*/

    cairo_stroke(cr);


    cairo_translate(cr, -step * g, 0);  /*return to the start of the graph*/



}
void draw_lines(cairo_t *cr, double width, double height, double font_size, int i, guint time_step, Mega_Data *temp) {

    cairo_set_source_rgba(cr, .7, .7, .7, 0.5);

//    for(int g=1;temp!=NULL;temp=temp->next,g++){
//        cairo_move_to(cr, (width - 2 * i * font_size) /(double) temp->delay * g + i * font_size, height - font_size);
//        cairo_line_to(cr, (width - 2 * i * font_size) / (double)temp->delay  * g + i * font_size, 0);
//
//    }
//!<(width - 2 * i * font_size) take the width and subtract its corners so we have space for the percentages
//!<then we divide it by the sample delay then multiple it with the counter  so we move forward
    for (int g = 1; g <= time_step; g++) {
        cairo_move_to(cr, (width - 2 * i * font_size) /time_step * g + i * font_size, height - font_size);
        cairo_line_to(cr, (width - 2 * i * font_size) / time_step  * g + i * font_size, 0);

    }
    cairo_stroke(cr);
}
void draw_lines2(cairo_t *cr, double width, double height, double font_size, int i, guint time_step, Mega_Data *temp) {

    cairo_set_source_rgba(cr, .7, .7, .7, 0.5);

//    for(int g=1;temp!=NULL;temp=temp->next,g++){
//        cairo_move_to(cr, (width - 2 * i * font_size) /(double) temp->delay * g + i * font_size, height - font_size);
//        cairo_line_to(cr, (width - 2 * i * font_size) / (double)temp->delay  * g + i * font_size, 0);
//
//    }
    for (int g = 1; g <= time_step; g++) {
        cairo_move_to(cr, (width - 2 * i * font_size) /MAX_ELEMENTS * g + i * font_size, height - font_size);
        cairo_line_to(cr, (width - 2 * i * font_size) / MAX_ELEMENTS  * g + i * font_size, 0);

    }
    cairo_stroke(cr);
}
/**
 * function do_drawing_mem(): draws the entire graph from the frame to the percentages to the lines
 * @param   widget pointer to which graph to draw on
 * @param   cr canvas
 *  @param  time_step
 *  @param  array data array
 *  @return void
 * */
void do_drawing_mem(GtkWidget *widget, cairo_t *cr, guint time_step, Mega_Data *array) {
    double width, height;
    height  = (double) gtk_widget_get_allocated_height(widget);
    width   = (double) gtk_widget_get_allocated_width(widget);


    cairo_surface_t *graph_surface;
    graph_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int) width, (int) height);


    cairo_set_line_width(cr, 1);
    double font_size = 10;

    cairo_set_font_size(cr, font_size);

    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);


    cairo_set_font_size(cr, font_size);



    draw_frame(cr, width, height, font_size, 3);
    draw_lines(cr, width, height, font_size, 3, time_step, NULL);



    draw_percentages(cr, height, font_size);


    writing_seconds(cr, width, height, font_size, 3);


    draw_graph_mem(cr, 0, 3, width, height, font_size, time_step, array);
    draw_graph_mem(cr, 1, 3, width, height, font_size, time_step,  array);

    if (graph_surface != NULL) {
        cairo_set_source_surface(cr, graph_surface, 0.0, 0.0);
        cairo_paint(cr);
        cairo_surface_destroy(graph_surface);
    }


}
/**
 * function do_drawing_int2(): draws the entire graph from the frame to the max number of interrupts to the lines
 * @param   widget pointer to which graph to draw on
 * @param   cr canvas
 * @param  interrupts1 list of interrupts
 * @return void
 * */
void do_drawing_int2(GtkWidget *widget, cairo_t *cr, I_Collection2 *interrupts1) {


    double width, height;


    __uint64_t max_num = 0;
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

        p=temp_p->CPU;
        for(int i=0;i<cpu_num;i++){
            if(max_num<*p){
                max_num=*p;
            }
            p++;
        }


        temp_p=temp_p->next;

    }
     if(max_num!=0)
         max_num = max_num * 5 / 4;

    cairo_move_to(cr, 0, font_size);
    sprintf(num, "%"PRIu64, max_num);
    cairo_show_text(cr, num);
    for (int i = 1; i <= 3; i++) {


        cairo_move_to(cr, 0, (height - font_size) / 4 * (4 - i));
        if(max_num!=0){
            sprintf(num, "%"PRIu64, (max_num / 4 * i));
        }else{
            cairo_show_text(cr,"0");
        }

        cairo_show_text(cr, num);


    }

    cairo_move_to(cr, 0, height - font_size);
    cairo_show_text(cr, "0");

    temp_p = interrupts1;
    int i=0;
    while(temp_p) {




        // cairo_move_to(cr, 5 * font_size + 5 * length * (i), height);
        cairo_move_to(cr,(5 * font_size )+ 5 * font_size * (i), height);


        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_show_text(cr, temp_p->interrupts.irq);

        draw_interrupts2(cr, i, temp_p, height, font_size, max_num, font_size);
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

/**
 * function do_drawing_net(): draws the entire graph, by searching the array for the biggest number
 * @param   widget pointer to which graph to draw on
 * @param   cr canvas
 * @param  time_step
 * @param  array
 * @return void
 * */
void do_drawing_net(GtkWidget *widget, cairo_t *cr, guint time_step, Mega_Data *array) {

    double width, height;




    __uint64_t max_num=0;


    float font_size = 10;
    height = (double) gtk_widget_get_allocated_height(widget);
    width = (double) gtk_widget_get_allocated_width(widget);
    gchar *recBytes ;

    Mega_Data *temp;


    cairo_surface_t *graph_surface;

    graph_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int) width, (int) height);

    cairo_set_line_width(cr, 1);


    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);


    cairo_set_font_size(cr, font_size);


    draw_frame(cr, width, height, font_size, 5);
    draw_lines(cr, width, height, font_size, 5, time_step, array);


    writing_seconds(cr, width, height, font_size, 5);
    cairo_stroke(cr);

    int g = (int) time_step;
    if (list_num_size < time_step) {/*how many elements do we have in an array*/
        g = list_num_size;
    }

        temp = array;
        for (int i = 0; i < g; i++) { //checking the entire list for the highest number of bytes

            if(temp->network[0]>temp->network[1]){
               if(max_num<temp->network[0]){
                   max_num=temp->network[0];
               }
            }else{
                if(max_num<temp->network[1]){
                    max_num=temp->network[1];
                }
            }

            temp = temp->next;

        }



    cairo_set_source_rgb(cr, 0, 0, 0);


    recBytes = g_format_size_full((uint64_t)max_num, G_FORMAT_SIZE_IEC_UNITS);
    cairo_move_to(cr, 0, font_size);//biggest num
   // sprintf(num, "%.1f", rec_bytes);

    cairo_show_text(cr, recBytes);
    g_free(recBytes);
   // cairo_show_text(cr, track);
    for (int i = 1; i <= 3; i++) {
        if(max_num!=0){
            recBytes = g_format_size_full(max_num/4*i, G_FORMAT_SIZE_IEC_UNITS);
        }else{
            recBytes = g_format_size_full(max_num,G_FORMAT_SIZE_IEC_UNITS);
        }



        cairo_move_to(cr, 0, (height - font_size) / 4 * (4 - i));
      //  sprintf(num, "%.1f", rec_bytes / 4 * (float) i);
      //  cairo_show_text(cr, num);
        cairo_show_text(cr, recBytes);
        g_free(recBytes);

    }
    cairo_move_to(cr, 0, (height - font_size));
    cairo_show_text(cr, "0");
   // cairo_show_text(cr, track);


    draw_graph_net(cr, 0, 5, width, height, font_size, time_step, max_num, array);
    draw_graph_net(cr, 1, 5, width, height, font_size, time_step, max_num, array);


    if (graph_surface != NULL) {
        cairo_set_source_surface(cr, graph_surface, 0.0, 0.0);
        cairo_paint(cr);
        cairo_surface_destroy(graph_surface);
    }


}
void do_drawing_one_cpu(GtkWidget *widget, cairo_t *cr, guint time_step, Mega_Data *array1, int index) {

    double width, height;
    double font_size = 10;


    height = (double) gtk_widget_get_allocated_height(widget);
    width = (double) gtk_widget_get_allocated_width(widget);


    cairo_surface_t *graph_surface;
    graph_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int) width, (int) height);

    cairo_set_line_width(cr, 1);


    cairo_set_font_size(cr, font_size);

    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);


    draw_frame(cr, width, height, font_size, 3);
    draw_lines(cr, width, height, font_size, 3, time_step, array1);
    draw_percentages(cr, height, font_size);


    //for(__int32_t i=0;i<cpu_num;i++){/*draws the lines*/

        if((cpu_status[index])==true){/*display the cpus we want to be displayed*/
            draw_graph(cr, index, width, height, font_size, time_step, array1);
        }



   // }

    writing_seconds(cr, width, height, font_size, 3);

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
void do_drawing_cpu(GtkWidget *widget, cairo_t *cr, guint time_step, Mega_Data *array1) {
    int corner=5;
    double width, height;
    double font_size = 10;
    double wanted_widht=0;
    double temp_d;
    bool *temp_bool=cpu_status;

    height =    (double) gtk_widget_get_allocated_height(widget);
    temp_d= width =     (double) gtk_widget_get_allocated_width(cpu_swindow);
    //!> divide the size of the scrolled window into time pieces, all the pieces combined make 60sec
    temp_d=temp_d-corner*2*font_size;
    if(temp_d!=0){
        temp_d=temp_d/60000;
    }

    wanted_widht=2*corner*font_size+temp_d*(double)gl_delay;
    printf("widht %f wanted_widht %f\n",width,wanted_widht);
    if(wanted_widht>width){
        gtk_widget_set_size_request(widget,(gint)wanted_widht,gtk_widget_get_allocated_height(cpu_swindow));
      //  width=wanted_widht;
    }



   // gtk_widget_set_size_request(widget,gtk_widget_get_allocated_width(cpu_swindow)/60000*gl_delay/60000,gtk_widget_get_allocated_height(cpu_swindow));

    cairo_surface_t *graph_surface;
    graph_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int) width, (int) height);

    cairo_set_line_width(cr, 1);


    cairo_set_font_size(cr, font_size);

    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);

    if(wanted_widht>width){
        draw_frame(cr, wanted_widht, height, font_size, corner);
        draw_lines2(cr, width, height, font_size, corner, gl_delay/250, array1);
    }
    else{
        draw_lines(cr, width, height, font_size, corner, time_step, array1);
        draw_frame(cr, width, height, font_size, corner);
    }


   // draw_lines(cr, width, height, font_size, 3, time_step, array1);
    draw_percentages(cr, height, font_size);


    for(__int32_t i=0;i<cpu_num;i++){/*draws the lines*/

        if((*temp_bool)==true){/**display the cpus we want to be displayed*/
            draw_graph(cr, i, width, height, font_size, time_step, array1);
        }
        temp_bool++;


    }
    if(wanted_widht>width){
        writing_seconds2(cr, wanted_widht, height, font_size, corner,gl_delay);
    }
    else{
        writing_seconds(cr, width, height, font_size, corner);
    }


    if (graph_surface != NULL) {
        cairo_set_source_surface(cr, graph_surface, 0.0, 0.0);
        cairo_paint(cr);
        cairo_surface_destroy(graph_surface);


    }

}





