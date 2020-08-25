//
// Created by dushn on 23.7.20..
//

#include "interrupts.h"

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int interrupt_usage2(Interrupts **array2, __int32_t *j) {


    FILE *file;
    char *filename = "/proc/interrupts";
    char buffer[BUFFER_SIZE];


    Interrupts *temp=NULL;



    *array2=malloc(sizeof(Interrupts));

    if (*array2 == NULL) {

        fprintf(stderr, "malloc failed\n");
        return 1;

    }
    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, BUFFER_SIZE, file) == NULL)
        return 1;


    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {


        memset(*array2, 0, sizeof(Interrupts));



        sscanf(buffer, "%s %" SCNu64 " %" SCNu64 " %" SCNu64 " %" SCNu64 " %s %s %s %s",
              (*array2)->irq,
              &(*array2)->CPU0,
               &(*array2)->CPU1,
               &(*array2)->CPU2,
               &(*array2)->CPU3,
               (*array2)->ime1,
               (*array2)->ime2,
               (*array2)->ime3,
               (*array2)->ime4);

        char  *p;
       if( (p=strchr((*array2)->irq,':'))){
           *p= '\0';
       }



        (*j)++;
            if(temp!=NULL){
                *array2=temp;
            }
        temp = realloc(*array2, (*j + 1) * sizeof(Interrupts));

        if (temp != NULL) {

            (*array2)= temp;

                (*array2)=(*array2)+(*j);


        } else {

            free(*array2);
            fclose(file);
            printf("reallocate error %d", errno);
            return 1;

        }


    }


    fclose(file);
    *array2 = temp;


    return 0;
}


static int myCompare(const void *a, const void *b) {

    Interrupts interrupts1 = *(Interrupts *) a;
    Interrupts interrupts2 = *(Interrupts *) b;


    int CPUa ;
    int CPUb ;


    CPUa = (int) (interrupts1.CPU0 + interrupts1.CPU1 + interrupts1.CPU2 + interrupts1.CPU3);
    CPUb = (int) (interrupts2.CPU0 + interrupts2.CPU1 + interrupts2.CPU2 + interrupts2.CPU3);


    return CPUa - CPUb;

}

void sort2(Interrupts *new_interrupts, Interrupts *old_interrupts, Interrupts **send_interrupts, int n) {


    (*send_interrupts) = calloc((size_t) n, sizeof(Interrupts));
    Interrupts *temp_send=(*send_interrupts);
    Interrupts *temp_old=old_interrupts;
    Interrupts *temp_new=new_interrupts;
    for (int i = 0; i < n; i++) {


        strcpy(temp_send->irq, temp_new->irq);

        __int64_t temp = temp_new->CPU0 - temp_old->CPU0;
        if (temp < 0) {
            temp = 0;
        }
        temp_send->CPU0 = (__uint64_t) temp;

        temp = temp_new->CPU1 - temp_old->CPU1;
        if (temp < 0) {
            temp = 0;
        }
        (*send_interrupts)->CPU1 = (__uint64_t) temp;


        temp = temp_new->CPU2 - temp_old->CPU2;
        if (temp < 0) {
            temp = 0;
        }
        temp_send->CPU2 = (__uint64_t) temp;

        temp =temp_new->CPU3 - temp_old->CPU3;
        if (temp < 0) {
            temp = 0;
        }
        temp_send->CPU3 = (__uint64_t) temp;


        strcpy(temp_send->ime1, temp_new->ime1);
        strcpy(temp_send->ime2, temp_new->ime2);
        strcpy(temp_send->ime3, temp_new->ime3);
        strcpy(temp_send->ime4, temp_new->ime4);
            *temp_old=*temp_new;
        temp_send++;
        temp_new++;
        temp_old++;
    }



}

void sort(Interrupts *array, int n) {

    qsort(array, (size_t) n, sizeof(Interrupts), myCompare);
}