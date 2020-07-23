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


void do_drawing_net(GtkWidget *widget, cairo_t *cr, int bjorg2, guint time_step);

void do_drawing_cpu(GtkWidget *widget, cairo_t *cr, int bjorg, guint time_step, gboolean CPU0_line, gboolean CPU1_line,
                    gboolean CPU2_line, gboolean CPU3_line);

void do_drawing_mem(GtkWidget *widget, cairo_t *cr, int bjorg, guint time_step);

void do_drawing_int(GtkWidget *widget, cairo_t *cr);

void crtaj_sekunde(cairo_t *cr, double width, double height, double font_size, int i, int j);

void crtaj_okvir(cairo_t *cr, double width, double height, double font_size, int i);

void crtaj_procente(cairo_t *cr, double height, double font_size);

void crtaj_interrupte(cairo_t *cr, int i, Interrupts *peak, double height, double font_size, __uint64_t max_broj,
                      double duzina);

void ispis_interrupta2(cairo_t *cr, double font_size, double duzina, int i, const gchar *ime1, const gchar *ime2);

void ispis_interrupta(cairo_t *cr, double font_size, double duzina, int i, const char *ime1, const char *ime2,
                      const char *ime3, const char *ime4);

void crtanje_graph(cairo_t *cr, GArray *history, int r, int y, int i, double height, double font_size, double step,
                   float max_broj3
);

#endif //DIPLOMSKI_DRAWING_H

