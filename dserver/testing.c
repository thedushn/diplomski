//
// Created by dushn on 23.9.20..
//

#include <stdio.h>
#include <inttypes.h>
#include <memory.h>
#include "testing.h"

#define BUFFER_SIZE 104
int interrupts_write(Interrupts *array, Interrupts *array2, char *filename, int size) {


    FILE *fp;
   // char *filename = "interrupts_server.data";
    char buffer[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    size_t t=0;
    if (array == NULL) {

        fprintf(stderr, "array is NULL failed\n");
        return 1;

    }
    if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
        return 1;

    fprintf(fp, "IRQ CPU0 CPU1 CPU2 CPU3 NAME  \n");
    for (int i = 0; i < size; i++) {
        memset(buffer, 0, sizeof(BUFFER_SIZE));
        memset(buffer2, 0, sizeof(BUFFER_SIZE));
        char CPU0[10];
        char CPU1[10];
        char CPU2[10];
        char CPU3[10];
        memset(CPU0, 0, sizeof(CPU0));
        memset(CPU1, 0, sizeof(CPU0));
        memset(CPU2, 0, sizeof(CPU0));
        memset(CPU3, 0, sizeof(CPU0));

        sprintf(CPU0,"%" PRIu64 "",array->CPU0);
        sprintf(CPU1,"%" PRIu64 "",array->CPU1);
        sprintf(CPU2,"%" PRIu64 "",array->CPU2);
        sprintf(CPU3,"%" PRIu64 "",array->CPU3);

        sprintf(buffer,"%3s %9s %9s %9s %9s %s %s %s %s ", array2->irq,CPU0,CPU1,CPU2,CPU3,
                array->ime1, array->ime2, array->ime3, array->ime4);

        memset(CPU0, 0, sizeof(CPU0));
        memset(CPU1, 0, sizeof(CPU0));
        memset(CPU2, 0, sizeof(CPU0));
        memset(CPU3, 0, sizeof(CPU0));
        sprintf(CPU0,"%" PRIu64 "",array2->CPU0);
        sprintf(CPU1,"%" PRIu64 "",array2->CPU1);
        sprintf(CPU2,"%" PRIu64 "",array2->CPU2);
        sprintf(CPU3,"%" PRIu64 "",array2->CPU3);

        sprintf(buffer2,"%3s %9s %9s %9s %9s %s %s %s %s ", array2->irq,CPU0,CPU1,CPU2,CPU3 , array2->ime1, array2->ime2, array2->ime3, array2->ime4);
        fprintf(fp,"%s",buffer);

        if((t=strlen(buffer))>BUFFER_SIZE|| (t=strlen(buffer)) <0){
            fclose(fp);
            return 0;

        }


        while((BUFFER_SIZE-t)>0){

            fprintf(fp," ");
            t++;
        }

        fprintf(fp,"%s \n",buffer2);





        array++;
        array2++;

    }



    fclose(fp);
    return 0;
}