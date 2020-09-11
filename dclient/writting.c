//
// Created by dushn on 4.9.20..
//

#include <inttypes.h>
#include <memory.h>
#include <stdlib.h>
#include "writting.h"

int interrupts_write(Interrupts *array) {


    FILE *fp;
    char *filename = "interrupts.data";
    char buffer[1024];
    if (array == NULL) {

        fprintf(stderr, "array is NULL failed\n");
        return 1;

    }
    if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
        return 1;


    fprintf(fp, "%d", t);
    for (int i = 0; i < 10; i++) {

        fprintf(fp, " \n %3s %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %s %s %s %s \n", array->name, array->CPU0,
                array->CPU1, array->CPU2, array->CPU3, array->ime1, array->ime2, array->ime3, array->ime4);


        memset(buffer, 0, sizeof(buffer));
        array++;

    }

    fseek(fp, 0, 0);

    fclose(fp);
    return 0;
}

int cpu_write(Cpu_usage cpu_usage) {


    FILE *fp;
    char *filename = "cpu.data";


    if ((fp = fopen(filename, "a+")) == NULL) //create a file if it doesnt exist
        return 1;

    //  fseek(fp,0,0);
    fprintf(fp, "%d\n", t);


    fprintf(fp, "%s %s %s %s \n",
            cpu_usage.percentage0,
            cpu_usage.percentage1,
            cpu_usage.percentage2,
            cpu_usage.percentage3

    );


    fseek(fp, 0, 0);

    fclose(fp);
    return 0;

}

int cpu_read(Collection **array, Time_Managment **time_managment, int *number, int *step_counter) {


    char *string = "24.889627 26.100063 25.005514 25.736818";

    float f[4];
    sscanf(string, "%f %f %f %f ", &f[0], &f[1], &f[2], &f[3]);
    FILE *fp;
    char *filename = "cpu.data";
    char buffer[1024];

    Collection *temp = NULL;
    Time_Managment *temp_time;

    if ((fp = fopen(filename, "r")) == NULL)
        return 1;


    while (fgets(buffer, 1024, fp) != NULL) {

        temp_time = calloc(1, sizeof(Time_Managment));
        if (temp_time == NULL) {

            for (int i = 0; i < *number; i++) {
                // save reference to first link
                temp_time = *time_managment;

                //mark next to first link as first
                *time_managment = (*time_managment)->next;

                //return the deleted link
                free(temp_time);

            }
            for (int i = 0; i < *number; i++) {
                // save reference to first link
                temp = *array;

                //mark next to first link as first
                *array = (*array)->next;

                //return the deleted link
                free(temp);

            }
            return 1;
        }


        sscanf(buffer, "%lf", &temp_time->time);


        if (fgets(buffer, 1024, fp) != NULL) {

            temp = calloc(1, sizeof(Collection));
            if (temp == NULL) {

                for (int i = 0; i < *number; i++) {
                    // save reference to first link
                    temp_time = *time_managment;

                    //mark next to first link as first
                    *time_managment = (*time_managment)->next;

                    //return the deleted link
                    free(temp_time);

                }
                for (int i = 0; i < *number; i++) {
                    // save reference to first link
                    temp = *array;

                    //mark next to first link as first
                    *array = (*array)->next;

                    //return the deleted link
                    free(temp);

                }
                return 1;
            }


            sscanf(buffer, "%f %f %f %f", &temp->data[0], &temp->data[1], &temp->data[2], &temp->data[3]);


            (*number) += 1;
            (*step_counter) += temp_time->time;

            temp->next = (*array);
            (*array) = temp;

            temp_time->next = (*time_managment);
            (*time_managment) = temp_time;

        } else {
            break;//end of file
        }


        (*number) += 1;
        (*step_counter) += temp_time->time;

        temp->next = (*array);
        (*array) = temp;

        temp_time->next = (*time_managment);
        (*time_managment) = temp_time;
    }


    fclose(fp);
    return 0;
}

