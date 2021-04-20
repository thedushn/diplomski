//
// Created by dushn on 23.7.20..
//

#include "functions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "signal.h"

#include <wait.h>
#include <errno.h>




#include "interrupts.h"
#include "cpu_usage.h"
#include "memory_usage.h"
#include "task_manager.h"
#include "devices.h"
#include "network_bandwith.h"

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)


/*
 * function sigchld_handler(): handles error signal
 * input: none
 * output:none
 * */
static void sigchld_handler() {

    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}
ssize_t send_data(int socket, Data data){
    ssize_t ret;
    pthread_mutex_lock(&mutex_send);
    ret = send(socket, &data, sizeof(Data), 0);
    pthread_mutex_unlock(&mutex_send);
    if (ret < 0) {
      //  printf("Error sending data\n return = %d\n", (int) ret);
        printf("Error sending data\n return = %d %s\n", (int)  ret,__FUNCTION__ );
        return ret;
    }
    if (ret == 0) {
        printf("Error sending data\n return = %d\n", (int) ret);
        printf("socket closed\n");
        ret = -1;
        return ret;
    }
    return ret;
}

/*
 * function test_send(): receives a packet and checks if its match a string
 * input : socket
 * output : returns a value equal or lower then zero if something is not correct else it return a value higher then 0
 * */

ssize_t test_send(int socket) {



    ssize_t ret = 0;
    Data data;

    memset(&data, 0, sizeof(Data));
    printf("entered test send\n");
    fflush(stdout);
    ret = recv(socket, &data, sizeof(Data), 0);
    printf("received test send\n");
    fflush(stdout);
    if (ret < 0) {

        printf("error receiving data\n %d", (int) ret);
        return ret;
    }
    if (ret == 0) {

        printf("socket closed\n");
        return ret;
    }
    if (ret < sizeof(Data)) {
        size_t length = sizeof(Data);
        length -= ret;
        while (length > 0 || length < 0) {


            ret = (int) recv(socket, &data, length, 0);
            length -= ret;

            if (ret < 0) {

                printf("error receiving data\n %d", (int) ret);
                return ret;
            }
            if (ret == 0) {

                printf("socket closed\n");
                return ret;
            }

        }
    }
    if (strcmp(data.unification.conformation, "everything came") != 0) {

        printf("conformation didn't get received  \n");

        return -1;
    }

    return sizeof(Data);
}




/*
 * function test_recv():  sends a conformation packet that has a specific string in it
 * input : socket
 * output : returns a value equal or lower then zero if something is not correct else it return a value higher then 0
 * */
ssize_t test_recv(int socket) {



    ssize_t ret ;

    Data data={0};
    data.size=TEXT;
    memset(data.unification.conformation,0,sizeof(data.unification.conformation));
    strcpy(data.unification.conformation,"everything came");

    ret = send(socket, &data, sizeof(Data), 0);


    if (ret < 0) {

        printf("Error sending data\n return = %d\n", (int) ret);
        return ret;
    }
    if (ret == 0) {

        printf("Error sending data\n return = %d\n", (int) ret);
        printf("socket closed\n");
        return ret;
    }


    return ret;

}
/*
 * function send_prio_to_task():  sets tasks priority, they can span from -15 to 15
 * input : pointer to a string that contains the name of the task and a pointer to a string that contains the type of
 * priority we want to set
 * output : none.
 * */
void send_prio_to_task(char *task_id, char *signal) {

    int prio = 0;

    if (strcmp(signal, "VERY_LOW") == 0) {
        prio = 15;

    }
    if (strcmp(signal, "LOW") == 0) {
        prio = 5;

    }
    if (strcmp(signal, "NORMAL") == 0) {
        prio = 0;

    }
    if (strcmp(signal, "VERY_HIGH") == 0) {
        prio = -15;

    }
    if (strcmp(signal, "HIGH") == 0) {
        prio = -5;

    }


    char str[4];
    /*these are some of the commands that need to be implemented if we want to change a tasks priority*/
    sprintf(str, "%d", prio);
    char command[64] = "renice -n ";
    strncat(command, str, sizeof command-1);
    strncat(command, " -p ", sizeof command-1);
    strncat(command, task_id, sizeof command-1);
    if (system(command) != 0) {

        printf("command failed\n");
    }


}
/*
 * function send_signal_to_task():  sends signal to task, they can be KILL,STOP,CONTINUE,TERMINATE
 * input : pointer to a string that contains the name of the task and a pointer to a string that contains the type of
 * signal we want to send
 * output : none.
 * */
void send_signal_to_task(char *task_id, char *signal) {

    if (task_id != NULL && signal != NULL) {
        char command[64] = "kill -";
        strncat(command, signal, sizeof(command) -1);
        strncat(command, " ", sizeof(command) -1);
        strncat(command, task_id, sizeof(command) -1);

        if (system(command) != 0)
            printf("command failed\n");
    }
}
/*
 * function accept_command():  accepts commands from client
 * input : pointer to socket
 * output : if the socket gets closed or it breaks SIGPIPE the function exits with the value of ret
 * */
void *accept_command(void *socket) {

    int         sockfd = *(int *) socket;

    Commands    commands;
    char        buffer[1500];
    char        text[256];
    char        *text1;
    ssize_t     ret;
    int         g;
   int s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_setcancelstate");
   s= pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
    if (s != 0)
        handle_error_en(s, "pthread_setcanceltype");
    while (1) {

        memset(buffer,0,sizeof(buffer));
        memset(&commands,0,sizeof(commands));
        memset(&text,0,sizeof(text));
        memset(&text1,0,sizeof(text1));


        if ((ret = recv(sockfd, &buffer, sizeof(buffer), MSG_WAITALL)) <= 0) {
            printf("error condition didn't get received\n");
            printf("Error receiving data\n return = %d\n", (int) ret);
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);
            ret=-100;
            pthread_exit(NULL);
        }


        sscanf(buffer, "%s %s %s",text,commands.command,commands.task_id);
        if(strcmp(text,"COMMAND")!=0){ /*if its a command to implement on a  task*/

           g=atoi(text);
           if(g==1){
               devices_show=true;
           }else{
               devices_show=false;
           }
            if (strcmp(commands.task_id, "") != 0 && strcmp(commands.command, "") != 0) {
                if (strcmp(commands.command, "STOP") == 0 ||
                    strcmp(commands.command, "CONT") == 0 ||
                    strcmp(commands.command, "KILL") == 0 ||
                    strcmp(commands.command, "TERM") == 0) {
                    send_signal_to_task(commands.task_id, commands.command);
                } else {

                    send_prio_to_task(commands.task_id, commands.command);
                }

            }
        }

        else {/*if its a textual command not involving tasks*/
            text1=strchr(buffer,' ');
                strcat(text1, " &");

            if (system(text1) != 0){
                printf("command failed\n");
            }

        }

    }

}
/*
 * function sending():  gets request from client to send it data from the proc file system,it runs multiple threads to
 * accomplish this task, after it has sent all the data it sends a conformation message back to the client that all the
 * data has been sent
 * input : pointer to socket
 * output : if there is any error in the execution of the code the function waits for all of the threads to notice
 * that there is an error and lets them finish their execution,after it frees the memory and exits.
 * */

void *sending(void *socket) {

   // ssize_t ret = 0;
    time_t time1;
    //void *status=NULL;

    devices_show=false;/*bool that tells the server only to send Block devices*/

    Thread_pack thr_pack=*(Thread_pack*)(socket);

    Thread_pack thr_pack_m[6]={0};

    for(int i=0;i<6;i++){
        thr_pack_m[i].socket=thr_pack.socket;
    }


    pthread_attr_t attr;
    pthread_mutex_init(&mutex_jiff,NULL);


    pthread_cond_init(&cpu_cond,NULL);
    int return_value;
    char buffer[128];
    printf("entered sending\n");
    fflush(stdout);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    while (1) {

        time1 = time(NULL);

        local_time = *localtime(&time1);

        //if(test_send((*(int *) socket))<0)
        if(test_send(thr_pack.socket )<0)
            break;
       /*wait for client to ask for data*/



        printf("inside the transfer \n");
        fflush(stdout);

        if ((return_value=pthread_create(&thr_pack_m[0].thread_id, &attr, send_memory,&thr_pack_m[0])) != 0) {/*if the server fails to create the thread we tell the other threads to exit*/

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);

            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);


        }



        if ((return_value=pthread_create(&thr_pack_m[1].thread_id, &attr, send_cpu,&thr_pack_m[1])) != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);


        }



        if ((return_value=pthread_create(&thr_pack_m[2].thread_id, &attr, send_network,&thr_pack_m[2])) != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);


        }



//        if ((return_value=pthread_create(&thr[3], &attr, send_interrupts,socket)) != 0) {
//
//            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
//            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
//            pthread_mutex_lock(&mutex_send);
//            thread_break = false;
//            pthread_mutex_unlock(&mutex_send);
//
//
//        }
        if ((return_value=pthread_create(&thr_pack_m[3].thread_id, &attr, send_interrupts2,&thr_pack_m[3])) != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);


        }



        if ((return_value=pthread_create(&thr_pack_m[4].thread_id, &attr, send_devices,&thr_pack_m[4])) != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);

        }



        if ((return_value=pthread_create(&thr_pack_m[5].thread_id, &attr, send_task,&thr_pack_m[5])) != 0) {

            printf("ERROR: Return Code from pthread_create() is %d\n", return_value);
            fprintf(stderr,"Error = %d (%s)\n",return_value,strerror(return_value));
            pthread_mutex_lock(&mutex_send);
            thread_break = false;
            pthread_mutex_unlock(&mutex_send);


        }


        for(int i=0;i<6;i++){


           if((return_value= pthread_join(thr_pack_m[i].thread_id, NULL))){/*checking if threads had problems gathering data*/

               strerror_r(return_value,buffer,sizeof(buffer));
               fprintf(stderr,"error = %d (%s)\n",return_value,buffer);
               pthread_mutex_lock(&mutex_send);
               thread_break = false;
               pthread_mutex_unlock(&mutex_send);

           }




            if (thr_pack_m[i].ret_val < 0 ) {/*if a thread encountered a problem*/
                pthread_mutex_lock(&mutex_send);
                thread_break = false;
                pthread_mutex_unlock(&mutex_send);
            }




        }


        if (thread_break == false) {
            printf("%d\n",__LINE__);
            fflush(stdout);
            break;

        }

       /*telling the client that we sent all the data*/
        if ((thr_pack.ret_val = test_recv(thr_pack.socket)) <= 0) {

            printf("Error sending data\n return = %d\n", (int) thr_pack.ret_val);
            thr_pack.ret_val=-1;
            break;
        }




    }

    pthread_attr_destroy(&attr);

    pthread_mutex_destroy(&mutex_jiff);

    clean_interrupts2();
//    printf("%d\n",__LINE__);
//    fflush(stdout);
//  int  s =  pthread_cancel(thr_p_main[1].thread_id);
//    printf("%d\n",__LINE__);
//    fflush(stdout);
//    if (s != 0)
//        handle_error_en(s, "pthread_cancel");
//    printf("%d\n",__LINE__);
//    fflush(stdout);

    pthread_exit(NULL);
    printf("%d\n",__LINE__);
}

int connection(char *port, int *socket_send, int *socket_command){
    int sockfd=0;
    int yes     = 1;
    int ret;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;

    char s[INET6_ADDRSTRLEN];
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP


    if ((ret = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        freeaddrinfo(servinfo);
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            freeaddrinfo(servinfo);
            return -1;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");

        return -1;
    }

    if (listen(sockfd, 10) == -1) {
        perror("listen");
        return -1;
    }

    sa.sa_handler =sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        return -1;
    }


    // main accept() loop
    sin_size = sizeof their_addr;
    *socket_send = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
    if (*socket_send == -1) {
        close(sockfd);
        perror("accept");
        return -1;
    }



    inet_ntop(their_addr.ss_family,
              get_in_addr((struct sockaddr *) &their_addr),
              s, sizeof s);


    // main accept() loop
    sin_size = sizeof their_addr;
    *socket_command = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
    if (*socket_command == -1) {
        close(*socket_command);
        close(*socket_send);
        close(sockfd);
        perror("accept");
        return -1;
    }



    inet_ntop(their_addr.ss_family,
              get_in_addr((struct sockaddr *) &their_addr),
              s, sizeof s);
    close(sockfd);//close socket so no more clients can connect
    return 0;
}


/*
 * function get_in_addr(): gets the type of socket address IP4 or IP6
 * input  : pointer to Structure describing an Internet socket address
 * output : none.
 * */
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

ssize_t send_cpu_num(int socket,int cpu_number) {



    ssize_t ret ;

    Data data={0};
    data.size=TEXT;
    memset(data.unification.conformation,0,sizeof(data.unification.conformation));
   // strcpy(data.unification.conformation,"everything came");
    sprintf(data.unification.conformation,"%d",cpu_number);
    ret = send(socket, &data, sizeof(Data), 0);


    if (ret < 0) {

        printf("Error sending data\n return = %d\n", (int) ret);
        return ret;
    }
    if (ret == 0) {

        printf("Error sending data\n return = %d\n", (int) ret);
        printf("socket closed\n");
        return ret;
    }


    return ret;

}