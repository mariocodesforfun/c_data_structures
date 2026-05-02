#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> 
#include <sys/time.h>

#define ITERS_PER_THREAD 1000000


// this is a lock counter
// it is accurate, but not scalable

typedef struct __counter_t{
    int value;
    pthread_mutex_t lock;
} counter_t;


void init(counter_t *c){
    c->value = 0;
    pthread_mutex_init(&c->lock, 0);
}

void increment(counter_t *c){
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}


void *worker(void *args){
    int i;
    counter_t *counter = (counter_t *)args;
    for (i=0; i<ITERS_PER_THREAD; i++){
        increment(counter);
    }
    return NULL;
}


double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

int main(int argc, char *argv[]){

    if (argc != 2) {
        fprintf(stderr, "usage: %s <num_threads>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);    


    pthread_t threads[n];
    counter_t counter;
    init(&counter);



    double start = get_time();

    for (int j=0; j<n; j++){
        pthread_create(&threads[j], NULL, worker, &counter);
    }

    for (int j=0; j<n; j++){
        pthread_join(threads[j], NULL);
    }

    double end = get_time();
    printf("%d\n", counter.value);
    printf("time:  %.4f seconds\n", end - start);
    return 0;
}