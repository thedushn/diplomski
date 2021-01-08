//
// Created by dushn on 7.1.21..
//

#include <stdio.h>
#include <zconf.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "time_managment.h"
#include "common.h"
#include "task_manager.h"
#include "functions.h"


#define BUFFER_SIZE 1024
int uptime() {


    FILE *fp;
    int errnum;
    int uptime1 = 0;
    time_t time1;
    struct tm tm2;
    int sec0, hr0, min0, t0;
    struct tm1 stop_time;
    char buffer[BUFFER_SIZE];

    fp = fopen("/proc/uptime", "r"); /*information on when the kernel started working*/
    if (fp != NULL) {

        while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {


            sscanf(buffer, "%d", &uptime1);
        }
    } else {

        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file: %s\n", strerror(errnum));


        return -1;
    }


    fclose(fp);

    time1 = time(NULL);
    tm2 = *localtime(&time1);




    hr0 = uptime1 / 3600;
    t0 = uptime1 % 3600;
    min0 = t0 / 60;
    sec0 = t0 % 60;
    stop_time.tm_hour = (__uint32_t) hr0; /*when did the computer start running*/
    stop_time.tm_min = (__uint32_t) min0;
    stop_time.tm_sec = (__uint32_t) sec0;

    differenceBetweenTimePeriod(tm2, stop_time, &begin_time);// time when linux started

    return 0;
}