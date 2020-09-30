//
// Created by dushn on 23.7.20..
//



//
// Created by dushn on 8.9.17..
//

#include <inttypes.h>
#include "drawing.h"


gboolean on_draw_event(GtkWidget *widget, cairo_t *cr) {


    if (widget == graph1) {

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

void writing_interrupts2(cairo_t *cr, double font_size, double length, int i, const gchar *name1, const gchar *name2) {


    double num = floor(length);
    int counter;
    size_t g;//how many letters can stand in one row
    g = (size_t) num - 2;
    int q = 0;//how many rows
    if (name1[0] != '\0') {


        size_t j = strlen(name1);


        while (g < j) {

            j -= g;
            q++;//increase num  of rows
        }
        if (q == 0) {

            cairo_move_to(cr, 5 * font_size + 5 * length * i, font_size);//move him to the start position
            cairo_show_text(cr, name1);
        }
        if (q >= 1) {

            for (int r = 0; r <= q; r++) {
                //move him depending on how many rows we need
                cairo_move_to(cr, 5 * font_size + 5 * length * i, font_size + (r * font_size));
                size_t y = g;
                j = strlen(name1);

                if (g > j) {

                    y = j;

                }

                if (r == q) {
                    char text_int[y];
                    memset(text_int, 0, y);
                    for (int s = 0; s < y; s++) {


                        text_int[s] = name1[s];
                    }

                    cairo_show_text(cr, name1);
                } else {


                    char *text_int = (char *) calloc(1, sizeof(char) * y + 1);


                    memset(text_int, 0, y);
                    for (int s = 0; s < y; s++) {


                        text_int[s] = name1[s];

                    }
                    counter = (int) y;
                    for (int c = 0; c < counter; c++) {
                        name1++;
                    }

                    cairo_show_text(cr, text_int);
                    free(text_int);

                }


            }
        }

        int movement = q;
        q = 0;

        if (name2[0] != '\0') {

            j = strlen(name2);

            while (g < j) {

                j -= g;
                q++;// increase  num of rows
            }
            if (q == 0) {

                cairo_move_to(cr, 5 * font_size + 5 * length * i,
                              2 * font_size + movement * font_size);//move him to the start position


                gchar text_int[j];
                memset(text_int, 0, j);
                for (int s = 0; s < j; s++) {

                    text_int[s] = name2[s];
                }
                cairo_show_text(cr, name2);
            } else {

                for (int r = 0; r <= q; r++) {
                    //move him depending on how many rows we need
                    cairo_move_to(cr, 5 * font_size + 5 * length * i,
                                  2 * font_size + movement * font_size + r * font_size);
                    size_t y = g;
                    j = strlen(name2);
                    if (g > j) {

                        y = j;

                    }

                    if (r == q) {


                        cairo_show_text(cr, name2);

                    } else {

                        char *text_int = (char *) calloc(1, sizeof(char) * y + 1);

                        memset(text_int, 0, y);
                        strncpy(text_int, name2, y);
                        counter = (int) y;
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

void writing_interrupts(cairo_t *cr, double font_size, double length, int i, const char *name1, const char *name2,
                        const char *name3, const char *name4) {


    if (name4[0] != '\0') {

        writing_interrupts2(cr, font_size, length, i, name3, name4);
    }

    if (name4[0] == '\0') {

        if (name3[0] != '\0') {
            writing_interrupts2(cr, font_size, length, i, name2, name3);

        }

        if (name3[0] == '\0') {

            writing_interrupts2(cr, font_size, length, i, name1, name2);

        }
    }


}

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


void draw_interrupts(cairo_t *cr, int i, Interrupts *peak, double height, double font_size, __uint64_t max_num,
                     double length) {

    double percentage;

    cairo_set_line_width(cr, 1);


    percentage = ((height - font_size) / max_num) * peak->CPU0;


    writing_interrupts(cr, font_size, length, i, peak->ime1, peak->ime2, peak->ime3, peak->ime4);


    cairo_rectangle(cr, 5 * font_size + 5 * length * i, height - font_size, length - 1, -percentage);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_stroke_preserve(cr);
    cairo_set_source_rgba(cr, .7, .7, .7, 0.5);
    cairo_move_to(cr, 5 * font_size + 5 * length * i, height - font_size);
    cairo_line_to(cr, 5 * font_size + 5 * length * i, 0);
    cairo_stroke_preserve(cr);
    cairo_set_source_rgb(cr, 1, 0, 0);
    cairo_fill(cr);
    percentage = ((height - font_size) / max_num) * peak->CPU1;
    cairo_rectangle(cr, 5 * font_size + length * (5 * i + 1), height - font_size, length - 1, -percentage);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_stroke_preserve(cr);
    cairo_set_source_rgb(cr, 0, 1, 0);
    cairo_fill(cr);
    percentage = ((height - font_size) / max_num) * peak->CPU2;
    cairo_rectangle(cr, 5 * font_size + length * (5 * i + 2), height - font_size, length - 1, -percentage);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_stroke_preserve(cr);
    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_fill(cr);
    percentage = ((height - font_size) / max_num) * peak->CPU3;
    cairo_rectangle(cr, 5 * font_size + length * (5 * i + 3), height - font_size, length - 1, -percentage);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_stroke_preserve(cr);
    cairo_set_source_rgb(cr, 1, .5, 0);
    cairo_fill(cr);


}

void
draw_graph(cairo_t *cr, int r, int i, double width, double height, double font_size, double time_step, float max_num,
           Cpu_list *array) {

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
    }else if (r == 4 ) {

        cairo_set_source_rgb(cr, 0, 1, 0.5);
    } else if (r == 5 ) {

        cairo_set_source_rgb(cr, 1, 0, 0.5);
    }  else if (r == 6 ) {

        cairo_set_source_rgb(cr, 0.5, 0, 1);
    }
    else if (r == 7 ) {

        cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    }else if (r == 8 ) {

        cairo_set_source_rgb(cr, 0.5, 1, 1);
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


        cairo_move_to(cr, i * font_size, prev);


        if (i==5) {


            percentage = ((height - font_size) / max_num) * peak;
            step = (width - 5 * font_size - 5 * font_size) / time_step;
        } else {
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


    cairo_line_to(cr, i * font_size, height - font_size);// the last line always touches the floor

    cairo_stroke(cr);


    cairo_translate(cr, -step * g, 0);  //return to start of graph


}
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


        if (i==5) {


            percentage = ((height - font_size) / max_num) * peak;
            step = (width - 5 * font_size - 5 * font_size) / time_step;
        } else {
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


    cairo_line_to(cr, i * font_size, height - font_size);// the last line always touches the floor

    cairo_stroke(cr);


    cairo_translate(cr, -step * g, 0);  //return to start of graph



}


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


    //frame
    draw_frame(cr, width, height, font_size, 3);



    //Percentage
    draw_percentages(cr, height, font_size);

    //seconds
    writing_seconds(cr, width, height, font_size, 3);


    draw_graph_net_mem(cr, 0, 3, width, height, font_size, time_step, 0, array);
    draw_graph_net_mem(cr, 1, 3, width, height, font_size, time_step, 0, array);

    if (graph_surface != NULL) {
        cairo_set_source_surface(cr, graph_surface, 0.0, 0.0);
        cairo_paint(cr);
        cairo_surface_destroy(graph_surface);
    }


}

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


    for(__int32_t i=0;i<CPU_NUM;i++){

        if((*temp_bool)==true){
            draw_graph(cr, i, 3, width, height, font_size, time_step, 0, array1);
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



