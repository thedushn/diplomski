//
// Created by dushn on 23.7.20..
//


#include"stdio.h"
#include"stdlib.h"

#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"
#include"pthread.h"

#include "functions.h"
#include "cpu_usage.h"
#include "task_manager.h"
#include "network_bandwith.h"
#include "time_managment.h"


#include <arpa/inet.h>


#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <wait.h>



#define BUFFER_SIZE 1024

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
/*
 * function main(): creates a TPC socket and waits for a connection of the client,if that was successful it gathers
 * information about when the kernel started working and creates threads to send information to the client and receive
 * commands from him, after the client wants to stop getting information about the servers host, the server closes the
 * sockets and free the allocated memory from the gathering of the data
 * input  : port number
 * output : returns a non zero value if something goes wrong
 * */

int main(int argc, char *argv[]) {

    hash_size     = 0;
    task_details  = NULL;
    hash_network  = NULL;
    net_hash_size = 0;
    cpu_Number=0;




    pthread_t thr1, thrd2;

    int sockfd  = 0;
    int new_fd  = 0;
    int new_fd1 = 0;  // listen on sock_fd, new connection on new_fd
    int yes     = 1;
    int ret;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;

    char s[INET6_ADDRSTRLEN];

    char buffer[BUFFER_SIZE];


    struct DataItem     *temp;
    struct DataItem_net *temp_net;

    if (argc < 2) {

        printf("no port provided");
        return -1;
    }

    cpu_Number=cpu_number();


//TODO make a function that does the connections
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP


    if ((ret = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
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
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (new_fd == -1) {
            close(sockfd);
            perror("accept");
            return -1;
        }



    inet_ntop(their_addr.ss_family,
              get_in_addr((struct sockaddr *) &their_addr),
              s, sizeof s);


      // main accept() loop
        sin_size = sizeof their_addr;
        new_fd1 = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (new_fd1 == -1) {
            close(new_fd);
            close(sockfd);
            perror("accept");
            return -1;
        }



    inet_ntop(their_addr.ss_family,
              get_in_addr((struct sockaddr *) &their_addr),
              s, sizeof s);

    close(sockfd);//close socket so no more clients can connect


    if(cpu_data_allocation()){
        close(new_fd);
        close(sockfd);
        return -1;
    }

   if(uptime()){
       close(new_fd);
       close(sockfd);
       free_cpu_memory();
   }



    if ((ret=pthread_create(&thr1, NULL, &sending, &new_fd)) != 0) {/*creating thread for sending data to client*/

        printf("ERROR: Return Code from pthread_create() is %d\n", ret);

        close(new_fd1);
        close(new_fd);
        free_cpu_memory();
        return -1;

    }

    /*creating thread for receiving commands from client*/

    if ((ret = pthread_create(&thrd2, NULL, accept_command, &new_fd1)) != 0) {

        printf("ERROR: Return Code from pthread_create() is %d\n", ret);

        close(new_fd1);
        close(new_fd);
        free_cpu_memory();
        return -1;

    }
    void *status=NULL;

    if((ret= pthread_join(thr1, &status))){

        strerror_r(ret,buffer,sizeof(buffer));
        fprintf(stderr,"error = %d (%s)\n",ret,buffer);
        pthread_mutex_lock(&mutex_send);
        thread_break = false;
        pthread_mutex_unlock(&mutex_send);

    }


    if((ret= pthread_join(thrd2, &status))){

        strerror_r(ret,buffer,sizeof(buffer));
        fprintf(stderr,"error = %d (%s)\n",ret,buffer);
        pthread_mutex_lock(&mutex_send);
        thread_break = false;
        pthread_mutex_unlock(&mutex_send);
    }






//    for(int k=0;k<hash_size;k++){ /*freeing the list of task details*/
    while(task_details){
        temp         = task_details;
        task_details = task_details->next;
        free(temp);

    }

    for(int k=0;k<net_hash_size;k++){/*freeing the list of network details*/

        temp_net     = hash_network;
        hash_network = hash_network->next;
        free(temp_net);

    }


    close(new_fd1);
    close(new_fd);
    free_cpu_memory();
    printf("we exited");

    return 0;
}
