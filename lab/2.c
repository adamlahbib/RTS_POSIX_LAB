#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// mutex for each segment
pthread_mutex_t ab, bc, cd, bf, ce;

//function to wait in minutes
void wait(int minutes) {
    int ms = 60000 * minutes;
    clock_t start_time = clock();
    while (clock() < start_time + ms);
}

// MR1: metro a to d
void *mr1(void *arg) {
    printf("Metro %ld is departing from terminal A\n", (intptr_t) arg);

    // traversing segment ab
    pthread_mutex_lock(&ab);
    printf("Metro %ld is traversing segment AB\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&ab);
    printf("Metro %ld is at station B\n", (intptr_t) arg);
    // traversing segment bc
    pthread_mutex_lock(&bc);
    printf("Metro %ld is traversing segment BC\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&bc);
    printf("Metro %ld is at station C\n", (intptr_t) arg);
    // traversing segment cd
    pthread_mutex_lock(&cd);
    printf("Metro %ld is traversing segment CD\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&cd);

    printf("Metro %ld has arrived at terminal D\n", (intptr_t) arg);
    pthread_exit(NULL);
}

// MR2: metro f to a
void *mr2(void *arg) {
    printf("Metro %ld is departing from terminal F\n", (intptr_t) arg);

    // traversing segment bf
    pthread_mutex_lock(&bf);
    printf("Metro %ld is traversing segment BF\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&bf);
    printf("Metro %ld is at station B\n", (intptr_t) arg);
    // traversing segment ab
    pthread_mutex_lock(&ab);
    printf("Metro %ld is traversing segment AB\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&ab);

    printf("Metro %ld has arrived at terminal A\n", (intptr_t) arg);
    pthread_exit(NULL);
}

//MR3: metro e to b
void *mr3(void *arg) {
    printf("Metro %ld is departing from terminal E\n", (intptr_t) arg);
    
    // traversing segment ce
    pthread_mutex_lock(&ce);
    printf("Metro %ld is traversing segment CE\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&ce);
    printf("Metro %ld is at station C\n", (intptr_t) arg);
    // traversing segment bc
    pthread_mutex_lock(&bc);
    printf("Metro %ld is traversing segment BC\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&bc);

    printf("Metro %ld has arrived at station B\n", (intptr_t) arg);
    pthread_exit(NULL);
}


void main(){
    // initialize mutex
    pthread_mutex_init(&ab, NULL);
    pthread_mutex_init(&bc, NULL);
    pthread_mutex_init(&cd, NULL);
    pthread_mutex_init(&bf, NULL);
    pthread_mutex_init(&ce, NULL);

    // create threads
    pthread_t m1,m2,m3,m4,m5,m6,m7,m8;
    pthread_create(&m1, NULL, mr1, (void *) (intptr_t) 1);
    pthread_create(&m2, NULL, mr1, (void *) (intptr_t) 2);
    pthread_create(&m3, NULL, mr1, (void *) (intptr_t) 3);
    pthread_create(&m4, NULL, mr1, (void *) (intptr_t) 4);
    pthread_create(&m5, NULL, mr3, (void *) (intptr_t) 5);
    pthread_create(&m6, NULL, mr2, (void *) (intptr_t) 6);
    pthread_create(&m7, NULL, mr3, (void *) (intptr_t) 7);
    pthread_create(&m8, NULL, mr2, (void *) (intptr_t) 8);


    // wait for threads to finish
    pthread_join(m1, NULL);
    pthread_join(m2, NULL);
    pthread_join(m3, NULL);
    pthread_join(m4, NULL);
    pthread_join(m5, NULL);
    pthread_join(m6, NULL);
    pthread_join(m7, NULL);
    pthread_join(m8, NULL);

    // destroy mutex
    pthread_mutex_destroy(&ab);
    pthread_mutex_destroy(&bc);
    pthread_mutex_destroy(&cd);
    pthread_mutex_destroy(&bf);
    pthread_mutex_destroy(&ce);
}
