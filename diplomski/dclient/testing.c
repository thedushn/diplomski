//
// Created by dushn on 23.9.20..
//

#include "testing.h"

#include "main_header.h"

#include <inttypes.h>
#include <memory.h>
#include <stdlib.h>



int interrupts_write(Interrupts *array) {


    FILE *fp;
    char *filename = "interrupts_client.data";
    char buffer[1024];
    if (array == NULL) {

        fprintf(stderr, "array is NULL failed\n");
        return 1;

    }
    if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
        return 1;


    fprintf(fp, "%d", t);
    for (int i = 0; i < 10; i++) {

        fprintf(fp, " \n %3s %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %s %s %s %s \n", array->irq, array->CPU0,
                array->CPU1, array->CPU2, array->CPU3, array->ime1, array->ime2, array->ime3, array->ime4);


        memset(buffer, 0, sizeof(buffer));
        array++;

    }



    fclose(fp);
    return 0;
}

int cpu_write(Cpu_usage cpu_usage) {


    FILE *fp;
    char *filename = "cpu.data";


    if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
        return 1;


    fprintf(fp, "%d\n", t);


    fprintf(fp, "%s %s %s %s \n",
            cpu_usage.percentage[0],
            cpu_usage.percentage[1],
            cpu_usage.percentage[2],
            cpu_usage.percentage[3]

    );




    fclose(fp);
    return 0;

}