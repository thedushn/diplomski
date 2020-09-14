//
// Created by dushn on 23.7.20..
//

#include "interrupts.h"
#include "functions.h"

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

Interrupts *interrupts = NULL;
Interrupts *interrupts_main = NULL;
Interrupts *interrupts_send = NULL;

void clean_interrupts(){

    if (interrupts != NULL) {

        free(interrupts);
    }
    if (interrupts_main != NULL) {

        free(interrupts_main);
    }
    if (interrupts_send != NULL) {

        free(interrupts_send);
    }

};
void * send_interrupts(void *socket){

    int sockfd=(*(int*)socket);

    ssize_t ret;
    __int32_t h = 0;
    int result;
    Data data={0};
    result = interrupt_usage2(&interrupts, &h);
    if (result != 0) {

        clean_interrupts();
        exit(1);
    }

    if (interrupts_main == NULL) {

        interrupts_main = calloc((size_t) h, sizeof(Interrupts));
        if(interrupts_main==NULL){

            printf("calloc error %d \n", errno);
            free(interrupts_main);


        }
        for (int r = 0; r < h; r++) {

            interrupts_main[r] = interrupts[r];
        }


    }



    sort2(interrupts, interrupts_main, &interrupts_send, h);


    sort(interrupts_send, h);


    for (int r = h - 10; r < h; r++) {

        memset(&data,0,sizeof(Data));

        data.size=INTERRUTPS;
        data.unification.interrupts=interrupts_send[r];
        pthread_mutex_lock(&mutex_send);
        ret = send(sockfd, &data, sizeof(Data), 0);
        pthread_mutex_unlock(&mutex_send);
        if (ret < 0) {
            printf("Error sending data!\n\t");
            break;

        }
        if (ret == 0) {

            printf("socket closed\n");
            break;
        }

//        pthread_mutex_lock(&mutex_send);
//        if( test_send(sockfd)<=0){
//
//            break;
//        }
//        pthread_mutex_unlock(&mutex_send);



    }

    free(interrupts);
    free(interrupts_send);



    interrupts = NULL;
    interrupts_send = NULL;
    pthread_exit(NULL);

}
int interrupt_usage2(Interrupts **array2, __int32_t *j) {


    FILE *file;
    char *filename = "/proc/interrupts";
    char buffer[1024];


    Interrupts *temp=NULL;



    *array2=malloc(sizeof(Interrupts));

    if (*array2 == NULL) {

        fprintf(stderr, "malloc failed\n");
        return 1;

    }
    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, 1024, file) == NULL)
        return 1;


    while (fgets(buffer, 1024, file) != NULL) {


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