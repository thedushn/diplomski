//
// Created by dushn on 23.7.20..
//

#ifndef DIPLOMSKI_DRAWING_H
#define DIPLOMSKI_DRAWING_H




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
#include "main_header.h"
#include "window.h"

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr);

void do_drawing_net(GtkWidget *widget, cairo_t *cr, guint time_step, NetMem_list *array);

void do_drawing_cpu(GtkWidget *widget, cairo_t *cr, guint time_step, Cpu_list *collection);

void do_drawing_mem(GtkWidget *widget, cairo_t *cr, guint time_step, NetMem_list *array);

void do_drawing_int(GtkWidget *widget, cairo_t *cr, Interrupts *interrupts1);

void writing_seconds(cairo_t *cr, double width, double height, double font_size, int i);

void draw_frame(cairo_t *cr, double width, double height, double font_size, int i);

void draw_percentages(cairo_t *cr, double height, double font_size);

void draw_interrupts(cairo_t *cr, int position, Interrupts *peak, double height, double font_size, __uint64_t max_num,
                     double length);

void writing_interrupt_names(cairo_t *cr, double font_size, double length, int position, const gchar *name1,
                             const gchar *name2);

void checking_interrupt_names(cairo_t *cr, double font_size, double length, int position, const char *ime1,
                              const char *ime2,
                              const char *name3, const char *name4);


void
draw_graph(cairo_t *cr, int r, double width, double height, double font_size, double time_step, Cpu_list *array);

void
draw_graph_net_mem(cairo_t *cr, int r, int i, double width, double height, double font_size, double time_step,
                   float max_num, NetMem_list *array);

#endif //DIPLOMSKI_DRAWING_H

