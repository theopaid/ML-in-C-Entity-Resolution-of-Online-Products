#include "../hdr/includes.h"

static void cleanup_handler(void *arg) {
    
    (void) pthread_mutex_unlock(((thread_args_t*)arg)->mt);

}

void *thread_func(void *arg) {
    pthread_cleanup_push(cleanup_handler, arg);

    pthread_barrier_wait(((thread_args_t*)arg)->barrier);

    while( 1 ) {
        //pthread_testcancel();
        Job *job;
        QueueNode *qn;

        pthread_mutex_lock(((thread_args_t*)arg)->mt);
        if ( isempty(((thread_args_t*)arg)->q) ) {
            //printf("block on cv: %d\n", ((thread_args_t*)arg)->cv);
            pthread_cond_signal(((thread_args_t*)arg)->cv);
            pthread_mutex_unlock(((thread_args_t*)arg)->mt);
            return NULL;
        }
        
        qn = queue_pop(((thread_args_t*)arg)->q);
        pthread_mutex_unlock(((thread_args_t*)arg)->mt);

        job = qn->data;
        job->function_execute(NULL);
        free(qn);
    }

    pthread_cleanup_pop(false);
}

JobScheduler *scheduler_init(int no_of_threads) {
    JobScheduler *scheduler = (JobScheduler*)malloc(sizeof(JobScheduler));
    ec_nzero( pthread_mutex_init(&scheduler->mt, NULL), "failed mutex init");
    ec_nzero( pthread_cond_init(&scheduler->cv, NULL), "failed cond init");
    ec_nzero( pthread_barrier_init(&scheduler->barrier, NULL, no_of_threads+1), "failed barrier init");
    scheduler->q = initialize_queue();
    scheduler->threads = no_of_threads;
    scheduler->tids = (pthread_t*)malloc(sizeof(pthread_t)*no_of_threads);
    scheduler->thread_args = (thread_args_t*)malloc(sizeof(thread_args_t));
    scheduler->thread_args->q = scheduler->q;
    scheduler->thread_args->cv = &(scheduler->cv);
    scheduler->thread_args->mt = &(scheduler->mt);
    scheduler->thread_args->barrier = &(scheduler->barrier);
    for ( int i = 0 ; i < no_of_threads ; i++ ) {
        ec_nzero( pthread_create(&scheduler->tids[i], NULL, thread_func,  scheduler->thread_args), "failed create");
    }
    return scheduler;
}

void scheduler_submit_job(JobScheduler *sch, Job *j) {
    QueueNode *nn = (QueueNode*)malloc(sizeof(QueueNode));
    nn->data = j;
    nn->next = NULL;
    pthread_mutex_lock(&sch->mt);
    add(sch->q, nn);
    pthread_cond_signal(&sch->cv);
    pthread_mutex_unlock(&sch->mt);
}

void scheduler_destroy(JobScheduler *sch) {
    ec_nzero( pthread_barrier_destroy(&sch->barrier), "failed barrier destroy");
    ec_nzero( pthread_mutex_destroy(&sch->mt), "Mutex destroyed too soon");
    ec_nzero( pthread_cond_destroy(&sch->cv), "failed cond destroy");
    for(int i = 0 ; i < sch->threads ; i++ ) {
        ec_nzero( pthread_cancel(sch->tids[i]), "failed pthread cancel");
        ec_nzero( pthread_join(sch->tids[i], NULL), "failed pthread join");
    }
    free(sch->thread_args);
    free(sch->tids);
    destroy_queue(sch->q);
    free(sch);
}

void scheduler_execute_all(JobScheduler *sch) {
    pthread_barrier_wait(&sch->barrier);
}

void scheduler_wait_finish(JobScheduler *sch) {
    pthread_cond_wait(&sch->cv, &sch->mt);
}

Queue* initialize_queue() {
    Queue *nq = (Queue*)malloc(sizeof(Queue));
    nq->head = NULL;
    nq->size = 0;
    return nq;
}

void destroy_queue(Queue *q) {
    if ( q == NULL ) return;
    if ( q->head == NULL ) {
        free(q);
        return;
    }
    QueueNode *temp = q->head;
    QueueNode *next = q->head->next;
    while ( next != NULL ) {
        //free(temp->data);
        free(temp);
        temp = next;
        next = next->next;
    }
    //free(temp->data);
    free(temp);
    free(q);
}

void add(Queue *q, QueueNode *nn) {
    if ( q == NULL ) return;
    if ( q->head == NULL ) {
        q->head = nn;
        q->size++;
        return;
    }
    QueueNode *temp = q->head;
    while ( temp->next != NULL ) {
        temp = temp->next;
    }
    temp->next = nn;
    q->size++;
}

int isempty(Queue *q) {
    if ( q == NULL || q->head == NULL ) return 1;
    return 0;
}

QueueNode *queue_pop(Queue *q) {
    if ( q == NULL || q->head == NULL ) return NULL;
    QueueNode *nj = (QueueNode*)q->head;
    q->head = q->head->next;
    return nj;
}



void make_model_sets(HashTable *all_set, Vector *set1, Vector *set2, Vector *set3) {
    return ;
}

Vector *init_test_values() {
    return NULL;
}

Vector *train_weights_testing(Vector *W1, Vector *T, Vector *test_values) {
    return NULL;
}


Vector *train_weights(Vector *W1, char *datasetX_path, Vector *test_values) {
    return NULL;
}

void validate_model(Vector *V, Vector *weights, Vector *test_values) {
    return ;
}