//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_DRAWING_H
#define DIPLOMSKI_DRAWING_H



//
// Created by dushn on 8.9.17..
//



#include <math.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <cairo.h>


#include "common.h"

typedef struct _Collection Collection;
struct _Collection {

    float data[8];
    Collection *next;
};

void do_drawing_net(GtkWidget *widget, cairo_t *cr, guint time_step);

void do_drawing_cpu(GtkWidget *widget, cairo_t *cr, guint time_step, const gboolean CPU0_line, const gboolean CPU1_line,
                    const gboolean CPU2_line, const gboolean CPU3_line);

void do_drawing_mem(GtkWidget *widget, cairo_t *cr, guint time_step);

void do_drawing_int(GtkWidget *widget, cairo_t *cr);

void writing_seconds(cairo_t *cr, double width, double height, double font_size, int i, int j);

void draw_frame(cairo_t *cr, double width, double height, double font_size, int i);

void draw_percentages(cairo_t *cr, double height, double font_size);

void draw_interrupts(cairo_t *cr, int i, Interrupts *peak, double height, double font_size, __uint64_t max_num,
                     double length);

void writing_interrupts2(cairo_t *cr, double font_size, double length, int i, const gchar *name1, const gchar *name2);

void writing_interrupts(cairo_t *cr, double font_size, double length, int i, const char *ime1, const char *ime2,
                        const char *name3, const char *name4);


void draw_graph(cairo_t *cr, int r, int i, double width, double height, double font_size, double time_step,
                float max_num);

#endif //DIPLOMSKI_DRAWING_H

