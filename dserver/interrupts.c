//
// Created by dushn on 23.7.20..
//

#include "interrupts.h"
#include "functions.h"
#include "testing.h"
#include "cpu_usage.h"

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




Interrupts2 *interrupts2      = NULL;
Interrupts2 *interrupts_main2 = NULL;
Interrupts2 *interrupts_send2 = NULL;

void clean_interrupts2(){
    Interrupts2 *temp;
    while (interrupts2 != NULL) {
        temp            = interrupts2;
        interrupts2     =interrupts2->next;
        if(temp->CPU!=NULL)
            free(temp->CPU);
        temp->CPU=NULL;
        free(temp);
        temp=NULL;

    }
    while (interrupts_main2 != NULL) {
        temp                = interrupts_main2;
        interrupts_main2    = interrupts_main2->next;
        if(temp->CPU!=NULL)
            free(temp->CPU);
        temp->CPU=NULL;
        free(temp);
        temp=NULL;

    }
    while (interrupts_send2 != NULL) {
        temp                = interrupts_send2;
        interrupts_send2    = interrupts_send2->next;
        if(temp->CPU!=NULL)
            free(temp->CPU);
        temp->CPU=NULL;
        free(temp);
        temp=NULL;

    }

}


void * send_interrupts2(void *socket){

 //   int         sockfd      = (*(int*)socket);
  //  ssize_t     *ret        = NULL;
    __int32_t   cpu_indx    = 0      ;
   Thread_pack thr_p=*(Thread_pack*)socket;
    int s=0;
    Data        data        = {0};
    Interrupts2 *temp       = NULL;
    printf("inside the %s\n",__func__);
    fflush(stdout);
   if( (s=  pthread_mutex_lock(&mutex_send))!=0)
       handle_error_en(s,"pthread_mutex_lock");

    while (thread_break == false) { /*if other threads have failed close this thread before it allocates any memory*/

        pthread_mutex_unlock(&mutex_send);
        printf("inside the thread_break %s\n",__func__);
        fflush(stdout);
        pthread_exit(NULL);
    }

    if( (s=  pthread_mutex_unlock(&mutex_send))!=0)
        handle_error_en(s,"pthread_mutex_unlock");

    printf("inside the %s\n",__func__);
    fflush(stdout);



    if((thr_p.ret_val = interrupt_usage2(&interrupts2)) != 0){   //get data

        clean_interrupts2();

        thr_p.ret_val=-1;
        pthread_exit(&thr_p.ret_val);
    }

    refresh_interrupt_data(interrupts2,interrupts_main2); //refresh data


    delete_old_interrupts2(&interrupts_main2);
    
    if(insert_new_interrupts(&interrupts_main2, interrupts2) < 0){ //insert new interrupts

        printf("calloc error %d \n", errno);
        clean_interrupts2();
        thr_p.ret_val = -1;
        pthread_exit(&thr_p.ret_val);

    }








   if(compare_interrupts2( interrupts_main2,interrupts2, &interrupts_send2)<0) {
       printf("calloc error %d \n", errno);
       clean_interrupts2();
       thr_p.ret_val = -1;
       pthread_exit(&thr_p.ret_val);
   }

   sort_interrupts2(&interrupts_send2);





    temp=interrupts_send2;
    while(temp){
        cpu_indx=0;
        memset(&data,0,sizeof(Data));



        data.size=INTERRUPTS;
        strcpy(data.unification.interrupts_send.irq,temp->irq);
        strcpy(data.unification.interrupts_send.name,temp->name);
        data.unification.interrupts_send.total=temp->total;
        if(send_data(thr_p.socket,data)<=0){
            clean_interrupts2();
            thr_p.ret_val = -1;
            pthread_exit(&thr_p.ret_val);
        }
        memset(&data,0,sizeof(Data));
        data.size=INT_PACK;

        while(cpu_indx!=cpu_Number){

            if(buffer_interrupts(temp,data.unification.data_pack,&cpu_indx)<0){

                clean_interrupts2();
                thr_p.ret_val = -1;
                pthread_exit(NULL);
            }
               // printf("%s\n",data.unification.data_pack);
            if(send_data(thr_p.socket,data)<0){
                clean_interrupts2();
                thr_p.ret_val = -1;
                pthread_exit(NULL);
            }
           memset(data.unification.data_pack,0,sizeof(data.unification.data_pack));
        }




        temp=temp->next;
    }

    while (interrupts2 != NULL) {
        temp            = interrupts2;
        interrupts2     =interrupts2->next;
        if(temp->CPU!=NULL)
            free(temp->CPU);
        temp->CPU=NULL;
        free(temp);
        temp=NULL;

    }
  
    while (interrupts_send2 != NULL) {
        temp                = interrupts_send2;
        interrupts_send2    = interrupts_send2->next;
        if(temp->CPU!=NULL)
            free(temp->CPU);
        temp->CPU=NULL;
        free(temp);
        temp=NULL;

    }




    pthread_exit(&thr_p.ret_val);

}

int buffer_interrupts(Interrupts2 *array, char *cp_array, int *cpu_indx){



    size_t j;
    int    mov;


    while((array)){
        j=1024-1;
      //  printf("%d %d %d %d \n",(array)->CPU[0],(array)->CPU[1],(array)->CPU[2],(array)->CPU[3]);
        for (int i = *cpu_indx; i < cpu_Number; i++) {




            if ((mov=snprintf(cp_array,j, "%"SCNu64"", (array)->CPU[i])) < 0) {


                printf("converting didn't work %s \n", cp_array);


                return -1;
            }

            if(j<mov){

                break;
            }

            cp_array=cp_array+mov;

            j-=mov;
            *cp_array++=' ';
            (*cpu_indx)++;

            if(j>0){
                j--;
            }
            else{
                break;
            }



        }
        *cp_array='\n';

        if( (*cpu_indx)==cpu_Number)
        return 0;
        else
            return 1;
    }


   return -1;
}
Interrupts2* MergeSortedList(Interrupts2* lst1, Interrupts2* lst2)
{
    Interrupts2* result = NULL;

    // Base Cases
    if (lst1 == NULL)
        return (lst2);
    else if (lst2 == NULL)
        return (lst1);

    // recursively merging two lists
    if (lst1->total < lst2->total) {
        result = lst1;
        result->next = MergeSortedList(lst1->next, lst2);
    }
    else {
        result = lst2;
        result->next = MergeSortedList(lst1, lst2->next);
    }
    return result;
}

// Splitting two into halves.
// If the size of the list is odd, then extra element goes in the first list.
void SplitList(Interrupts2* source, Interrupts2** front, Interrupts2** back)
{
    Interrupts2* ptr1;
    Interrupts2* ptr2;
    ptr2 = source;
    ptr1 = source->next;

    // ptr1 is incremented twice and ptr2 is incremented once.
    while (ptr1 != NULL) {
        ptr1 = ptr1->next;
        if (ptr1 != NULL) {
            ptr2 = ptr2->next;
            ptr1 = ptr1->next;
        }
    }

    // ptr2 is at the midpoint.
    *front = source;
    *back = ptr2->next;
    ptr2->next = NULL;
}
void MergeSort(Interrupts2** thead)
{
    Interrupts2* head = *thead;
    Interrupts2* ptr1;
    Interrupts2* ptr2;

    // Base Case
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    // Splitting list
    SplitList(head, &ptr1, &ptr2);

    // Recursively sorting two lists.
    MergeSort(&ptr1);
    MergeSort(&ptr2);

    // Sorted List.
    *thead = MergeSortedList(ptr1, ptr2);
}

void sort_interrupts2(Interrupts2 **array){
    MergeSort(array);
}

bool scan_numbers(__uint64_t *CPU, char **ptr, const char *endp, int  *index, int *cpu_index, bool end_of_string){

    char num_buffer[64];
    char *str_temp;
    memset(num_buffer,0,sizeof(num_buffer));

    if(end_of_string){
        for(int i=*cpu_index;i<cpu_Number;i++) {
            while (**ptr == ' ') { //find the first number

                (*ptr)++;
            }
            while (**ptr != ' ' && **ptr != '\n') {
                num_buffer[(*index)++] = **ptr;
                (*ptr)++;
            }
            *index = 0;

        //    sscanf(num_buffer, " %" SCNu64 "", &CPU[i]);
            CPU[i]=  strtoul(num_buffer,NULL,10);
        (*cpu_index)++;

            memset(num_buffer, 0, sizeof(num_buffer));
            if ((str_temp = strchr((*ptr), ' ')) == NULL) {//jump over the numbers

                break; //if there isn't a space after the number it means that there is no name
            } else {
                (*ptr) = str_temp;
            }

        }
        (*cpu_index)=0;
        return true;

    }else{
        for(int i=(*cpu_index);i<cpu_Number;i++) {
            while(**ptr==' ' && *ptr!=endp ){ //find the first number

                (*ptr)++;
            }
            if(*ptr!=endp ){

                while(**ptr!=' ' && *ptr!=endp ){
                    num_buffer[(*index)++]=**ptr;
                    (*ptr)++;
                }
                if(**ptr==' '){
                    CPU[i]=  strtoul(num_buffer,NULL,10);
                   // sscanf(num_buffer, " %" SCNu64 "", &CPU[i]);
                    (*cpu_index)++;
                    memset(num_buffer, 0, sizeof(num_buffer));
                    *index=0;
                }else{
                    break;
                }
            }else{
                break;
            }




        }

        if((*cpu_index)==cpu_Number)
            return true;
    }

    return false;
}


bool irq_scan(Interrupts2 *element,char **ptr,const char *endp,int  *index,bool end_of_string){


        if(end_of_string){
            while((**ptr)!=':'){//insert the IRQ into storage

                (element)->irq[(*index)++]=(**ptr);
                (*ptr)++;
            }
                *index=0;
                (*ptr)++;
                return true;

        }else{
            while(**ptr!=':' && *ptr!=endp){//insert the IRQ into storage

                (element)->irq[(*index)++]=(**ptr);
                (*ptr)++;
            }
            if(*ptr!=endp ){


                *index=0;
                (*ptr)++;
                return true;

            }
            else{
                return false;
        }

    }
}
bool scan_name(Interrupts2 *array,char *string,const char *endptr,int *i,bool end_of_string){
    if(end_of_string){
        while(*string!='\n'){ //write the name into storage

            while(*string==' '){

                string++;
            }
            while(*string!=' '){

                if(*string=='\n'){
                    (*i)=0;
                    return true;
                }



                array->name[(*i)++]=*string++;

            }
            if(*string=='\n'){
                (*i)=0;
               return true;
            }
            array->name[(*i)++]=' ';

        }
        (*i)=0;
        return true;

    }else{
        while(string!=endptr){
            while(*string==' '){

                string++;
            }
            while(*string!=' '){

                if(string==endptr)
                    return false;

                array->name[(*i)++]=*string++;

            }
            array->name[(*i)++]=' ';

        }
        return false;
    }

}


int interrupt_usage2(Interrupts2 **array) {


    FILE *file;
    char *filename = "/proc/interrupts";
   // char *filename = "interrupts.data";
    char buffer[1024];
    char *string            = NULL;


    int irq_idx=0;
    int number_index=0;
    Interrupts2 *temp       = NULL;

    bool irq_inserted       = false;
    bool number_inserted    = false;
    bool memory_allocated   = false;

    bool end_of_string;




    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, 1024, file) == NULL)
        return -1;

    while ((string=fgets(buffer, 1024, file)) != NULL) {
     //  printf("%d %s\n",strlen(&buffer),&buffer);
        if((strchr(string,'\n')==NULL) ? (end_of_string=false) :(end_of_string=true))

        if(!memory_allocated)  {
            temp=calloc(1,sizeof(Interrupts2)); //allocate memory for storage
            if(temp==NULL){
                fprintf(stderr, "malloc failed\n");
                free(temp);
                while((*array)){
                    temp         = (*array);
                    (*array)=(*array)->next;
                    free(temp->CPU);
                    free(temp);
                }
                fclose(file);
                return -1;
            }

            temp->next=(*array); //link the lists

            if ((*array) != NULL) {
                (*array)->prev = temp;
            }
            (*array)=temp;

            (*array)->total=0;






            (*array)->CPU=calloc((size_t)cpu_Number,sizeof(__uint64_t)); //allocate the interrupt numbers
            if ((*array)->CPU == NULL) {

                fprintf(stderr, "malloc failed\n");
                free((*array)->CPU);
                while((*array)){
                    temp  = (*array);
                    (*array)=(*array)->next;
                    free(temp->CPU);
                    free(temp);
                }
                fclose(file);
                return -1;


            }
            memory_allocated=true;
        }




            while(*string==' '){//find the first occurrence of a letter or number

            string++;
            }

                if(!irq_inserted){
                    if(!(irq_inserted= irq_scan((*array),&string,&buffer[1023],&irq_idx,end_of_string))){
                 continue;
                 }
                }



     if(!number_inserted) {
        if(!(number_inserted= scan_numbers((*array)->CPU,&string,&buffer[1023],&number_index,&irq_idx,end_of_string))){
            continue;
        }

     }

        if (!(scan_name((*array), string, &buffer[1023], &number_index, end_of_string)))
            continue;




        memory_allocated = false;
        number_inserted  = false;
        irq_inserted     = false;






    }


    fclose(file);



    return 0;
}


int compare_interrupts2(Interrupts2 *new_interrupts, Interrupts2 *old_interrupts, Interrupts2 **array){

    Interrupts2 *   temp_new= NULL;
    Interrupts2 *   temp    = NULL;
    __uint64_t      total         ;

    while(old_interrupts){
        temp_new=new_interrupts;
        while(temp_new) {

            if (strcmp(old_interrupts->name, temp_new->name) == 0 && strcmp(old_interrupts->irq, temp_new->irq) == 0) {


                temp = calloc(1, sizeof(Interrupts2)); //allocate memory for storage
                if (temp == NULL) {
                    fprintf(stderr, "malloc failed\n");
                    free(temp);

                    return -1;
                }

                temp->next = (*array); //link the lists

                if ((*array) != NULL) {
                    (*array)->prev = temp;
                }
                (*array) = temp;
                (*array)->checked = false;


                (*array)->CPU = calloc((size_t) cpu_Number, sizeof(__uint64_t)); //allocate the interrupt numbers
                if ((*array)->CPU == NULL) {

                    fprintf(stderr, "malloc failed\n");
                    free((*array)->CPU);

                    return -1;


                }

                strcpy((*array)->name, temp_new->name);
                strcpy((*array)->irq, temp_new->irq);
                total = 0;

                for (int i = 0; i < cpu_Number; i++) {
                  //  printf("%lu ",temp_new->CPU[i] - old_interrupts->CPU[i]);
                    (*array)->CPU[i] = temp_new->CPU[i] - old_interrupts->CPU[i];
//                    if (((*array)->CPU[i] = temp_new->CPU[i] - old_interrupts->CPU[i])>temp_new->CPU[i]) {
//                        (*array)->CPU[i] = (*array)->CPU[i]-1 ; //overflow
//                    }
                    total += (*array)->CPU[i];
                }

               // printf("%d %d %d %d \n",(*array)->CPU[0],(*array)->CPU[1],(*array)->CPU[2],(*array)->CPU[3]);

                (*array)->total = total;

            }
            temp_new = temp_new->next;
        }
        old_interrupts=old_interrupts->next;
    }
    return 0;
}


void refresh_interrupt_data(Interrupts2 *new_interrupts, Interrupts2 *old_interrupts) {
    Interrupts2 *temp=new_interrupts;
    __uint64_t   *cpu_temp;

    while(old_interrupts) {

        old_interrupts->checked=false;

        while(new_interrupts) {

            if(strcmp(old_interrupts->irq, new_interrupts->irq)==0 && strcmp(old_interrupts->name, new_interrupts->name)==0){

                cpu_temp                = old_interrupts->CPU;
                old_interrupts->CPU     = new_interrupts->CPU;
                old_interrupts->total   = new_interrupts->total;
                new_interrupts->CPU     = cpu_temp;
                old_interrupts->checked = true;
                new_interrupts->checked = true;

                break;
            }
            new_interrupts->checked = false;
                new_interrupts          =  new_interrupts->next;
        }
        new_interrupts  =   temp;
        old_interrupts  =   old_interrupts->next;
    }

}
void delete_old_interrupts2(Interrupts2 **array){



    Interrupts2 *rem_old=(*array);
    Interrupts2 *temp=NULL;
    while ((*array)) {

        if(!(*array)->checked){

            if ((*array)->prev==NULL) {
                temp = (*array);
                if ((*array)->next != NULL) {
                    (*array) = (*array)->next;
                    (*array)->prev=NULL;
                }

                free(temp->CPU);
                temp->CPU=NULL;
                free(temp);
                temp=NULL;
                rem_old = (*array); //setting the first node
            } else {

                temp=(*array); // the one that we don't want anymore
                if((*array)->next){
                    (*array)->next->prev=(*array)->prev; //remember the last prev
                   if((*array)->prev)
                       (*array)->prev->next=(*array)->next;

                }
                else{
                    if((*array)->prev)
                        (*array)->prev->next=(*array)->next;
                    free(temp->CPU);
                    temp->CPU=NULL;
                    free(temp);
                    temp=NULL;
                    break;
                }


                free(temp->CPU);
                temp->CPU=NULL;
                free(temp);
                temp=NULL;

                //(*array) = rem_old;
            }

        }

            (*array) = (*array)->next;



    }
    (*array) = rem_old;
}

int insert_new_interrupts(Interrupts2 **array, Interrupts2 *new_interrupts) {
    Interrupts2 *temp;



    while(new_interrupts) {

        if(!new_interrupts->checked){




            temp=calloc(1,sizeof(Interrupts2));
            if (temp == NULL) {

                printf("calloc error %d \n", errno);
                free(temp);

                return -1;
            }
            temp->CPU=calloc((size_t)cpu_Number,sizeof(__uint64_t));
              memcpy(temp->CPU,new_interrupts->CPU,cpu_Number*sizeof(__uint64_t)) ;

                temp->checked=true;


            strcpy(temp->name,new_interrupts->name);
            strcpy(temp->irq,new_interrupts->irq);
            temp->total=new_interrupts->total;
            temp->next = (*array);
            temp->prev = NULL;
            if((*array)!=NULL){
                (*array)->prev=temp;
            }

            (*array) = temp;



        }
        new_interrupts=new_interrupts->next;
    }
    return 0;
}



