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


#include <arpa/inet.h>


#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <wait.h>
#include <unistd.h>


#define BUF_SIZE 1024


void sigchld_handler(int s) {

    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

int main(int argc, char *argv[]) {


    pthread_t t2, t3;
    int sockfd = 0;
    int new_fd = 0;
    int new_fd1 = 0;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv, ret, ret2;
    char buffer[BUF_SIZE];
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if (argc < 2) {

        printf("no port provided");
        exit(1);
    }


    if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
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
            exit(1);
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
        exit(1);
    }

    if (listen(sockfd, 10) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }


    {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (sockfd == -1) {
            perror("accept");

        }
    }


    inet_ntop(their_addr.ss_family,
              get_in_addr((struct sockaddr *) &their_addr),
              s, sizeof s);


    {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd1 = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (new_fd1 == -1) {
            perror("accept");

        }
    }


    inet_ntop(their_addr.ss_family,
              get_in_addr((struct sockaddr *) &their_addr),
              s, sizeof s);


    FILE *fp;

    int uptime1 = 0;
    fp = fopen("/proc/uptime", "r");
    if (fp != NULL) {

        while (fgets(buffer, 1024, fp) != NULL) {


            sscanf(buffer, "%d", &uptime1);
        }
    } else {
        int errnum;
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file: %s\n", strerror(errnum));
        close(sockfd);
        close(new_fd1);
        close(new_fd);
        exit(1);
    }

    fclose(fp);


    time_t time1 = time(NULL);
    struct tm tm2 = *localtime(&time1);

    int sec0, hr0, min0, t0;

    struct tm1 stop_time;


    hr0 = uptime1 / 3600;
    t0 = uptime1 % 3600;
    min0 = t0 / 60;
    sec0 = t0 % 60;
    stop_time.tm_hour = (__uint32_t) hr0; //pre koliko sati se upalio komp
    stop_time.tm_min = (__uint32_t) min0;
    stop_time.tm_sec = (__uint32_t) sec0;

    differenceBetweenTimePeriod(tm2, stop_time, &pocetno);// vreme kada je poceo da radi linux



    ret2 = pthread_create(&t2, NULL, sending, &new_fd);
    if (ret2 != 0) {

        printf("ERROR: Return Code from pthread_create() is %d\n", ret2);
        exit(1);

    }

    ret = pthread_create(&t3, NULL, accept_c, &new_fd1);

    if (ret != 0) {

        printf("ERROR: Return Code from pthread_create() is %d\n", ret);
        exit(1);

    }


    pthread_join(t2, NULL);
    pthread_join(t3, NULL);


    close(sockfd);
    do_check(true);
    clean();


    return 0;
}
