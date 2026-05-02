#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NUM_CPUS 10
#define THRESHOLD 1024
#define ITERS_PER_THREAD 1000000
#define CACHE_LINE 128

typedef struct {
    int value;
    char pad[CACHE_LINE - sizeof(int)];
} padded_int_t;

typedef struct {
    pthread_mutex_t lock;
    char pad[CACHE_LINE - sizeof(pthread_mutex_t)];
} padded_lock_t;

typedef struct __counter_t {
    int global_value;
    pthread_mutex_t global_lock;
    padded_int_t local_values[NUM_CPUS];
    padded_lock_t local_locks[NUM_CPUS];
    int threshold;
} counter_t;

void init(counter_t *c, int threshold) {
    c->threshold = threshold;
    c->global_value = 0;
    pthread_mutex_init(&c->global_lock, NULL);

    for (int i = 0; i < NUM_CPUS; i++) {
        c->local_values[i].value = 0;
        pthread_mutex_init(&c->local_locks[i].lock, NULL);
    }
}

double get_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

void update(counter_t *c, int threadID, int amt) {
    int cpu = threadID % NUM_CPUS;
    pthread_mutex_lock(&c->local_locks[cpu].lock);

    c->local_values[cpu].value += amt;

    if (c->local_values[cpu].value >= c->threshold) {
        pthread_mutex_lock(&c->global_lock);
        c->global_value += c->local_values[cpu].value;
        pthread_mutex_unlock(&c->global_lock);
        c->local_values[cpu].value = 0;
    }

    pthread_mutex_unlock(&c->local_locks[cpu].lock);
}

int get(counter_t *c) {
    pthread_mutex_lock(&c->global_lock);
    int global_counter_value = c->global_value;
    pthread_mutex_unlock(&c->global_lock);
    return global_counter_value;
}

typedef struct {
    counter_t *c;
    int threadID;
} arg_t;

void *worker(void *args) {
    arg_t *a = (arg_t *)args;
    for (int i = 0; i < ITERS_PER_THREAD; i++) {
        update(a->c, a->threadID, 1);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <num_threads>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);

    pthread_t threads[n];
    arg_t args[n];
    counter_t counter;
    init(&counter, THRESHOLD);

    double start = get_time();
    for (int j = 0; j < n; j++) {
        args[j].c = &counter;
        args[j].threadID = j;
        pthread_create(&threads[j], NULL, worker, &args[j]);
    }
    for (int j = 0; j < n; j++) {
        pthread_join(threads[j], NULL);
    }
    double end = get_time();

    printf("threads: %d  value: %d  time: %.4f s\n",
           n, get(&counter), end - start);
    return 0;
}