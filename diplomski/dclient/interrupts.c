//
// Created by dushn on 23.7.20..
//


#include "interrupts.h"
#include "common.h"


void upis(GArray *array, GArray *array2) {


    while (array2->len > 0) {

        g_array_remove_index(array2, 0);
    }

    for (guint r = 0; r < array->len; r++) {

        Interrupts interrupts3;


        interrupts3 = g_array_index(array, Interrupts, r);

        g_array_insert_val(array2, r, interrupts3);

        while (array2->len > 10) {
            g_array_remove_index(array2, array2->len - 11);

        }

    }


};

#include "interrupts.h"
