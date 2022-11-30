#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// mutex for each segment
pthread_mutex_t ab, bc, cd, bf, ce;

void wait(int minutes) {
    int ms = 60000 * minutes;
    clock_t start_time = clock();
    while (clock() < start_time + ms);
}

// metro a to b
void *a_to_d(void *arg) {
    // traversing segment ab
    pthread_mutex_lock(&ab);
    printf("Metro %ld is traversing segment ab\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&ab);
    printf("Metro %ld has traversed segment ab\n", (intptr_t) arg);
    // traversing segment bc
    pthread_mutex_lock(&bc);
    printf("Metro %ld is traversing segment bc\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&bc);
    printf("Metro %ld has traversed segment bc\n", (intptr_t) arg);
    // traversing segment cd
    pthread_mutex_lock(&cd);
    printf("Metro %ld is traversing segment cd\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&cd);
    printf("Metro %ld has traversed segment cd\n", (intptr_t) arg);
    pthread_exit(NULL);
}

// metro e to b
void *e_to_b(void *arg) {
    // traversing segment ce
    pthread_mutex_lock(&ce);
    printf("Metro %ld is traversing segment ce\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&ce);
    printf("Metro %ld has traversed segment ce\n", (intptr_t) arg);
    // traversing segment bc
    pthread_mutex_lock(&bc);
    printf("Metro %ld is traversing segment bc\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&bc);
    printf("Metro %ld has traversed segment bc\n", (intptr_t) arg);
    pthread_exit(NULL);
}

// metro f to a
void *f_to_a(void *arg) {
    // traversing segment bf
    pthread_mutex_lock(&bf);
    printf("Metro %ld is traversing segment bf\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&bf);
    printf("Metro %ld has traversed segment bf\n", (intptr_t) arg);
    // traversing segment ab
    pthread_mutex_lock(&ab);
    printf("Metro %ld is traversing segment ab\n", (intptr_t) arg);
    wait(6);
    pthread_mutex_unlock(&ab);
    printf("Metro %ld has traversed segment ab\n", (intptr_t) arg);
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
    pthread_t a, b, c, d, e, f;
    pthread_create(&a, NULL, a_to_d, (void *) (intptr_t) 1);
    pthread_create(&b, NULL, a_to_d, (void *) (intptr_t) 2);
    pthread_create(&c, NULL, a_to_d, (void *) (intptr_t) 3);
    pthread_create(&d, NULL, a_to_d, (void *) (intptr_t) 4);
    pthread_create(&e, NULL, e_to_b, (void *) (intptr_t) 5);
    pthread_create(&f, NULL, f_to_a, (void *) (intptr_t) 6);

    // wait for threads to finish
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    pthread_join(c, NULL);
    pthread_join(d, NULL);
    pthread_join(e, NULL);
    pthread_join(f, NULL);

    // destroy mutex
    pthread_mutex_destroy(&ab);
    pthread_mutex_destroy(&bc);
    pthread_mutex_destroy(&cd);
    pthread_mutex_destroy(&bf);
    pthread_mutex_destroy(&ce);
}
