#include "threads.h"

pthread_barrier_t barrier;
#define NTHREADS 4
void *routine(){
    printf("Thread %ld waiting on barrier\n", pthread_self());
    pthread_barrier_wait(&barrier);
    printf("Thread %ld after barrier\n", pthread_self());
    return NULL;
}

int main(){
    pthread_t threads[NTHREADS];
    pthread_barrier_init(&barrier, NULL, NTHREADS);
    for(int i = 0; i < NTHREADS; i++){
        pthread_create(&threads[i], NULL, routine, NULL);
    }
    for(int i = 0; i < NTHREADS; i++){
        pthread_join(threads[i], NULL);
    }
    pthread_barrier_destroy(&barrier);
    return 0;
}
