//
// Created by dushn on 23.7.20..
//

#include "cpu_usage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>



static struct DataItem *hash_cpu_user = NULL;
static struct DataItem *hash_cpu_system = NULL;

static __uint64_t jiffies_total_delta[5] = {0, 0, 0, 0, 0};

int cpu_number() {

    int c = 1; //cpu number must be at least 1
    FILE *file;
    char *filename = "/proc/cpuinfo";
    char buffer[1024];
    char *buffer2 = "processor";

    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, 1024, file) == NULL)
        exit(1);
    while (fgets(buffer, 1024, file) != NULL) {
        if (strncmp(buffer2, buffer, strlen(buffer2)) == 0) {

            c++;
        }


    }
    fclose(file);


    return c;
}

void cpu_percentage(int cpu_count, Cpu_usage *cpu_usage) {


    float cpu_user[4];
    float cpu_system[4];
    __uint64_t user[4] = {0, 0, 0, 0};
    __uint64_t user_nice[4] = {0, 0, 0, 0};
    __uint64_t idle[4] = {0, 0, 0, 0};
    __uint64_t system[4] = {0, 0, 0, 0};
    float percentage[4] = {0, 0, 0, 0};


    int cpu_number1[4] = {0, 0, 0, 0};
    static __uint64_t jiffies_system[4] = {0, 0, 0, 0};
    static __uint64_t jiffies_total[4] = {0, 0, 0, 0};
    static __uint64_t jiffies_user[4] = {0, 0, 0, 0};
    static __uint64_t jiffies_user_old[4] = {0, 0, 0, 0};
    static __uint64_t jiffies_system_old[4] = {0, 0, 0, 0};
    static __uint64_t jiffies_total_old[4] = {0, 0, 0, 0};

    FILE *file;
    char *filename = "/proc/stat";
    char *name_test = "cpu";
    char buffer[1024];
    if ((file = fopen(filename, "r")) == NULL || fgets(buffer, 1024, file) == NULL)

        exit(1);


    static int j = 0;

    while (fgets(buffer, 1024, file) != NULL) {

        if (strncmp(buffer, name_test, strlen(name_test)) != 0) {

            break;

        }


        sscanf(buffer, "cpu%4d %" SCNu64 " %" SCNu64 " %" SCNu64 " %" SCNu64 "", &cpu_number1[j], &user[j],
               &user_nice[j], &system[j], &idle[j]
        );

        j++;
        if (j == cpu_count) {
            j = 0;
            break;
        }

    }
    fclose(file);


    for (int i = 0; i < cpu_count; i++) {


        jiffies_user_old[i] = jiffies_user[i];
        jiffies_system_old[i] = jiffies_system[i];
        jiffies_total_old[i] = jiffies_total[i];

        jiffies_user[i] = user[i] + user_nice[i];
        jiffies_system[i] = system[i];
        jiffies_total[i] = jiffies_user[i] + jiffies_system[i] + idle[i];


        cpu_user[i] = cpu_system[i] = (float)0.0;
        if (jiffies_total[i] > jiffies_total_old[i]) {


            jiffies_total_delta[i] = jiffies_total[i] - jiffies_total_old[i];
            cpu_user[i] = (float)(jiffies_user[i] - jiffies_user_old[i]) * 100 / (float) (jiffies_total_delta[i]);
            cpu_system[i] =(float) (jiffies_system[i] - jiffies_system_old[i]) * 100 / (float) (jiffies_total_delta[i]);

        }

        percentage[i] = cpu_user[i] + cpu_system[i];


    }


    if (sprintf(cpu_usage->percentage0, "%f", percentage[0]) < 0) {

        printf("converting didnt work %s \n", cpu_usage->percentage0);
        exit(1);
    }
    if (sprintf(cpu_usage->percentage1, "%f", percentage[1]) < 0) {

        printf("converting didnt work %s \n", cpu_usage->percentage1);
        exit(1);
    }
    if (sprintf(cpu_usage->percentage2, "%f", percentage[2]) < 0) {

        printf("converting didnt work %s \n", cpu_usage->percentage2);
        exit(1);
    }
    if (sprintf(cpu_usage->percentage3, "%f", percentage[3]) < 0) {

        printf("converting didnt work %s \n", cpu_usage->percentage3);
        exit(1);
    }


    jiffies_total_delta[4] =
            jiffies_total_delta[0] + jiffies_total_delta[1] + jiffies_total_delta[2] + jiffies_total_delta[3];


}


__uint64_t search(unsigned int key, struct DataItem *hashArray, int hash_size, bool *ima, __uint64_t data) {


    for (int hashIndex = 0; hashIndex < hash_size; hashIndex++) {

        if (hashArray[hashIndex].key == key) {
            *ima = true;

            __uint64_t temp = hashArray[hashIndex].data;
            hashArray[hashIndex].data = data;
            return temp;

        }


    }

    return 0;
}


int get_cpu_percent(unsigned int pid, __uint64_t jiffies_user, float *cpu_user, __uint64_t jiffies_system,
                float *cpu_system) {

    __uint64_t jiffies_user_old = 0;
    __uint64_t jiffies_system_old = 0;
    static int hash_size = 0;

    bool ima = false;
    if (hash_cpu_user == NULL) {
        hash_cpu_user = (struct DataItem *) calloc(1, sizeof(struct DataItem));
        hash_cpu_system = (struct DataItem *) calloc(1, sizeof(struct DataItem));
        hash_size++;
    }

    jiffies_user_old = search(pid, hash_cpu_user, hash_size, &ima, jiffies_user);
    jiffies_system_old = search(pid, hash_cpu_system, hash_size, &ima, jiffies_system);


    if (ima == false) {


        struct DataItem *temp = realloc(hash_cpu_user, (hash_size + 1) * sizeof(struct DataItem));
        struct DataItem *temp1 = realloc(hash_cpu_system, (hash_size + 1) * sizeof(struct DataItem));

        hash_size++;
        if (temp != NULL) {
            hash_cpu_user = temp;
        } else {
            free(hash_cpu_user);
            return -1;

        }
        if (temp1 != NULL) {
            hash_cpu_system = temp1;


        } else {
            free(hash_cpu_system);
            return -1;
        }

        memset(&hash_cpu_user[hash_size - 1], 0, sizeof(struct DataItem));
        memset(&hash_cpu_system[hash_size - 1], 0, sizeof(struct DataItem));

        hash_cpu_user[hash_size - 1].data = jiffies_user;
        hash_cpu_system[hash_size - 1].data = jiffies_system;
        hash_cpu_user[hash_size - 1].key = pid;
        hash_cpu_system[hash_size - 1].key = pid;

    }


    if (jiffies_user < jiffies_user_old || jiffies_system < jiffies_system_old) {

        return 0;
    }

    if (jiffies_total_delta[4] > 0) {

        *cpu_user = (float) (( jiffies_user) - (jiffies_user_old))* 100 /
                             (float) (jiffies_total_delta[4]);
        *cpu_system =(float) ((jiffies_system - jiffies_system_old) * 100) / (float) jiffies_total_delta[4];

    } else {

        *cpu_user = *cpu_system = 0;

    }
    return 0;
}

void do_check(bool clean) {

    if (clean == true) {

        if (hash_cpu_user != NULL) {
            free(hash_cpu_user);

        }
        if (hash_cpu_system != NULL) {
            free(hash_cpu_system);

        }

    }

}