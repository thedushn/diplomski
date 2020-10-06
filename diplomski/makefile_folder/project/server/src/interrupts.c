//
// Created by dushn on 23.7.20..
//

#include "interrupts.h"
#include "functions.h"
#include "testing.h"

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

Interrupts *interrupts = NULL;
Interrupts *interrupts_main = NULL;
Interrupts *interrupts_send = NULL;

/*
 * function clean_interrupts(): checks if any of the pointers are NULL if not it frees the allocated memory
 * input    : none.
 * output   : none.
 * */
void clean_interrupts(){

    if (interrupts != NULL) {

        free(interrupts);
        interrupts = NULL;
    }
    if (interrupts_main != NULL) {

        free(interrupts_main);
        interrupts_main = NULL;
    }
    if (interrupts_send != NULL) {

        free(interrupts_send);
        interrupts_send = NULL;
    }

};
/*
 * function send_interrupts(): send data about the the top 10 interrupts  to client
 * input    : socket to send data to
 * output   : returns a non zero value if something goes wrong
 * */
void * send_interrupts(void *socket){

    int         sockfd=(*(int*)socket);
    ssize_t     ret;
    __int32_t   h = 0;
    int         result;
    Data        data={0};

    pthread_mutex_lock(&mutex_send);
    while (thread_break == false) {/*if other threads have failed close this thread before it allocates any memory*/
        ret = -100;
        pthread_mutex_unlock(&mutex_send);
        pthread_exit(&ret);
    }
    pthread_mutex_unlock(&mutex_send);

    result = interrupt_usage(&interrupts, &h);
    if (result != 0) {

        clean_interrupts();

        ret = -100;
        pthread_exit(&ret);
    }



    if (interrupts_main == NULL) {

        interrupts_main = calloc((size_t) h, sizeof(Interrupts));
        if(interrupts_main==NULL){

            printf("calloc error %d \n", errno);
            free(interrupts_main);
            ret = -100;
            pthread_exit(&ret);

        }
        for (int r = 0; r < h; r++) {

            interrupts_main[r] = interrupts[r];
        }


    }

    interrupts_send = calloc((size_t)  h, sizeof(Interrupts));
    if(interrupts_send==NULL){
        printf("calloc error %d \n", errno);
        free(interrupts_send);
        ret = -100;
        pthread_exit(&ret);
    }
    compare_interrupts(interrupts, interrupts_main, interrupts_send, h);

  /*  char *filename = "interrupts_server.data";
   interrupts_write(interrupts, interrupts_send, filename, h);*/


    qsort(interrupts_send, (size_t) h, sizeof(Interrupts), myCompare);

    for (int r = h - 10; r < h; r++) { /*sending only the top 10 interrupts */

        memset(&data,0,sizeof(Data));

        data.size=INTERRUPTS;
        data.unification.interrupts=interrupts_send[r];
        pthread_mutex_lock(&mutex_send);
        ret = send(sockfd, &data, sizeof(Data), 0);
        pthread_mutex_unlock(&mutex_send);
        if (ret < 0) {
            printf("Error sending data\n return = %d\n", (int) ret);
            pthread_exit(&ret);
        }
        if (ret == 0) {
            printf("Error sending data\n return = %d\n", (int) ret);
            printf("socket closed\n");
            ret = -1;
            pthread_exit(&ret);
        }


    }

    free(interrupts);
    free(interrupts_send);



    interrupts = NULL;
    interrupts_send = NULL;
    pthread_exit(&ret);

}
/*
 * function interrupt_usage(): gathers all the interrupts to a list and counts how many there are
 * input    : double pointer to a list of structure of data about interrupts and a pointer to the number of interrupts.
 * output   : returns a non zero value if something goes wrong
 * */

int interrupt_usage(Interrupts **array, __int32_t *num) {


    FILE *file;
    char *filename = "/proc/interrupts";
    char buffer[1024];


    Interrupts *temp=NULL;



    *array=malloc(sizeof(Interrupts));

    if (*array == NULL) {

        fprintf(stderr, "malloc failed\n");
        free(*array);
        return -1;

    }
    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, 1024, file) == NULL)
        return 1;


    while (fgets(buffer, 1024, file) != NULL) {


        memset(*array, 0, sizeof(Interrupts));



        sscanf(buffer, "%s %" SCNu64 " %" SCNu64 " %" SCNu64 " %" SCNu64 " %s %s %s %s",
              (*array)->irq,
              &(*array)->CPU0,
               &(*array)->CPU1,
               &(*array)->CPU2,
               &(*array)->CPU3,
               (*array)->ime1,
               (*array)->ime2,
               (*array)->ime3,
               (*array)->ime4);

        char  *p;
       if( (p=strchr((*array)->irq,':'))){ /*after every IRQ name there is a : */
           *p= '\0';
       }



        (*num)++;
            if(temp!=NULL){
                *array=temp;
            }
        temp = realloc(*array, (*num + 1) * sizeof(Interrupts));/*adding another interrupts sturcture to the array*/

        if (temp != NULL) {

            (*array)= temp;

                (*array)=(*array)+(*num); /*moving to the newly allocated element of the array*/


        }
        else {

            free(*array);
            fclose(file);
            printf("reallocate error %d", errno);
            return 1;

        }


    }


    fclose(file);
    *array = temp;


    return 0;
}

/*
 * function myCompare(): compares all the elements of the the list and sorts them from lowest to highest,
 * by adding their collective number of interrupts and comparing them to a others  collective number of interrupts
 * input    : two void pointers that represent elements of the array
 * output   : returns a non zero value if something goes wrong
 * */
int myCompare(const void *a, const void *b) {

    Interrupts interrupts1 = *(Interrupts *) a;
    Interrupts interrupts2 = *(Interrupts *) b;


    int CPUa ;
    int CPUb ;


    CPUa = (int) (interrupts1.CPU0 + interrupts1.CPU1 + interrupts1.CPU2 + interrupts1.CPU3);
    CPUb = (int) (interrupts2.CPU0 + interrupts2.CPU1 + interrupts2.CPU2 + interrupts2.CPU3);


    return CPUa - CPUb;

}
/*
 * function compare_interrupts(): checks for differences between the new list of interrupts and the old
              and saves to differences to a new list, if the differences are less then 0
              that means the that proc/interrupts file has restarted
 * input    : Interrupts pointer pointing to Interrupts the new list;Interrupts pointer pointing to the old list ,
 *            pointer pointing to the list that is going to get sent to the client;
 * output   : none.
 * */
void compare_interrupts(Interrupts *new_interrupts, Interrupts *old_interrupts, Interrupts *send_interrupts, int n) {


    for (int i = 0; i < n; i++) {


        strcpy(send_interrupts->irq, new_interrupts->irq);

        __int64_t temp = new_interrupts->CPU0 - old_interrupts->CPU0;
        if (temp < 0) {
            temp = 0;
        }
        send_interrupts->CPU0 = (__uint64_t) temp;

        temp = new_interrupts->CPU1 - old_interrupts->CPU1;
        if (temp < 0) {
            temp = 0;
        }
        send_interrupts->CPU1 = (__uint64_t) temp;


        temp = new_interrupts->CPU2 - old_interrupts->CPU2;
        if (temp < 0) {
            temp = 0;
        }
        send_interrupts->CPU2 = (__uint64_t) temp;

        temp =new_interrupts->CPU3 - old_interrupts->CPU3;
        if (temp < 0) {
            temp = 0;
        }
        send_interrupts->CPU3 = (__uint64_t) temp;


        strcpy(send_interrupts->ime1, new_interrupts->ime1);
        strcpy(send_interrupts->ime2, new_interrupts->ime2);
        strcpy(send_interrupts->ime3, new_interrupts->ime3);
        strcpy(send_interrupts->ime4, new_interrupts->ime4);

        *old_interrupts=*new_interrupts;

        send_interrupts++;
        new_interrupts++;
        old_interrupts++;
    }



}

