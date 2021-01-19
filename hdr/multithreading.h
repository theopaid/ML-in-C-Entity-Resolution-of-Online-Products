#ifndef _MULTI_THRD_
#define _MULTI_THRD_

#include "../hdr/hash.h"
#include "../hdr/vectorImpl.h"

#define ec_nzero(call, msg) {if ( (call) < 0 ) {perror(msg); exit(1);}}

void *thread_func(void *arg);

typedef struct thread_args thread_args_t;
typedef struct QueueNode QueueNode;
typedef struct Queue Queue;
typedef struct JobScheduler JobScheduler;
typedef struct Job Job;
typedef struct thread_args thread_args_t;

struct JobScheduler {
    int threads;
    Queue *q;
    pthread_t *tids;
    pthread_cond_t cv;  // q_has_item
    pthread_mutex_t mt; // queue lock mutex
    pthread_barrier_t barrier; // thread barrier for simultaneous execution
    thread_args_t *thread_args;
};

struct thread_args {
    pthread_mutex_t *mt;
    pthread_cond_t *cv;
    pthread_barrier_t *barrier;
    Queue *q;
};

struct Job {
    void (*function_execute)(void *p); // can point to any function (task) with parameters or NULL
};

struct Queue {
    QueueNode *head;
    int size;
};

struct QueueNode {
    void *data; // can point to any data type, in this case we want an initialized Job pointer passed to it
    QueueNode *next;
};

/* Job Scheduler Operations */
JobScheduler *scheduler_init(int no_of_threads);
void scheduler_submit_job(JobScheduler *sch, Job *j);
void scheduler_destroy(JobScheduler *sch);
void scheduler_execute_all(JobScheduler *sch);
void scheduler_wait_finish(JobScheduler *sch);

/* queue operations */
Queue* initialize_queue();
void destroy_queue(Queue *q);
void add(Queue *q, QueueNode *nn);
int isempty(Queue *q);
QueueNode *queue_pop(Queue *q); // !Used with queue mutex locked


/**
 * @brief Make 3 new vectors using items in a specified vector. Percentages are 60%, 20% and 20%.
 * @param[in] all_set The vector with all the items to be separated.
 * @param[out] set1 60% of all_set.
 * @param[out] set2 20% of all_set.
 * @param[out] set3 20% of all_set.
 */
void make_model_sets(HashTable *all_set, Vector *set1, Vector *set2, Vector *set3);

/**
 * @brief Make a vector with initial test values for: learing rate, #of threads, batch size, threshold value, threshold step and threshold slep slope.
 * @returns The vector with initial test values.
 */
Vector *init_test_values();

/**
 * @brief Train the model using the pairs in T to calculate and return the weights and improve the test_values.
 * @param[in] W1 The training set.
 * @param[in] T The testing set.
 * @param[out] test_values The vector with the values that will be improved based on T.
 * @returns The vector with the weights calculated by the training.
 */
Vector *train_weights_testing(Vector *W1, Vector *T, Vector *test_values);

/**
 * @brief Train the model using all pairs of json files specified in datasetX_path and the test_values given.
 * @param W1 The training set.
 * @param datasetX_path The path that the json files are stored.
 * @param test_values The test_values that will define the models' learning rate, #of threads, batch size, threshold value, threshold step and threshold slep slope.
 * @returns The vector with the weights calculated by the training.
 */
Vector *train_weights(Vector *W1, char *datasetX_path, Vector *test_values);

/**
 * @brief Validate the model using pairs in V, given weights and test_values and print the resulting accuracy.
 * @param V The validation set.
 * @param weights The weights vector that has been calculated by training.
 * @param test_values The test_values of the model.
 */
void validate_model(Vector *V, Vector *weights, Vector *test_values);

#endif