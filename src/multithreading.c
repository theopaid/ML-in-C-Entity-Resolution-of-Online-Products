#include "../hdr/includes.h"

static void cleanup_handler(void *arg)
{

    (void)pthread_mutex_unlock(((thread_args_t *)arg)->mt);
}

void *thread_func(void *arg)
{
    pthread_cleanup_push(cleanup_handler, arg);

    pthread_barrier_wait(((thread_args_t *)arg)->barrier);

    while (1)
    {
        //pthread_testcancel();
        Job *job;
        QueueNode *qn;

        pthread_mutex_lock(((thread_args_t *)arg)->mt);
        if (isempty(((thread_args_t *)arg)->q))
        {
            //printf("block on cv: %d\n", ((thread_args_t*)arg)->cv);
            pthread_cond_signal(((thread_args_t *)arg)->cv);
            pthread_mutex_unlock(((thread_args_t *)arg)->mt);
            return NULL;
        }

        qn = queue_pop(((thread_args_t *)arg)->q);
        pthread_mutex_unlock(((thread_args_t *)arg)->mt);

        job = qn->data;
        job->function_execute(job->any_parameter);
        free(qn);
    }

    pthread_cleanup_pop(true);
}

JobScheduler *scheduler_init(int no_of_threads)
{
    JobScheduler *scheduler = (JobScheduler *)malloc(sizeof(JobScheduler));
    ec_nzero(pthread_mutex_init(&scheduler->mt, NULL), "failed mutex init");
    ec_nzero(pthread_cond_init(&scheduler->cv, NULL), "failed cond init");
    ec_nzero(pthread_barrier_init(&scheduler->barrier, NULL, no_of_threads + 1), "failed barrier init");
    scheduler->q = initialize_queue();
    scheduler->threads = no_of_threads;
    scheduler->tids = (pthread_t *)malloc(sizeof(pthread_t) * no_of_threads);
    scheduler->thread_args = (thread_args_t *)malloc(sizeof(thread_args_t));
    scheduler->thread_args->q = scheduler->q;
    scheduler->thread_args->cv = &(scheduler->cv);
    scheduler->thread_args->mt = &(scheduler->mt);
    scheduler->thread_args->barrier = &(scheduler->barrier);
    for (int i = 0; i < no_of_threads; i++)
    {
        ec_nzero(pthread_create(&scheduler->tids[i], NULL, thread_func, scheduler->thread_args), "failed create");
    }
    return scheduler;
}

void scheduler_submit_job(JobScheduler *sch, Job *j)
{
    QueueNode *nn = (QueueNode *)malloc(sizeof(QueueNode));
    nn->data = j;
    nn->next = NULL;
    pthread_mutex_lock(&sch->mt);
    add(sch->q, nn);
    pthread_cond_signal(&sch->cv);
    pthread_mutex_unlock(&sch->mt);
}

void scheduler_destroy(JobScheduler *sch)
{
    ec_nzero(pthread_barrier_destroy(&sch->barrier), "failed barrier destroy");
    ec_nzero(pthread_mutex_destroy(&sch->mt), "failed mutex destroy");
    ec_nzero(pthread_cond_destroy(&sch->cv), "failed cond destroy");
    for (int i = 0; i < sch->threads; i++)
    {
        ec_nzero(pthread_cancel(sch->tids[i]), "failed pthread cancel");
        ec_nzero(pthread_join(sch->tids[i], NULL), "failed pthread join");
    }
    free(sch->thread_args);
    free(sch->tids);
    destroy_queue(sch->q);
    free(sch);
}

void scheduler_execute_all(JobScheduler *sch)
{
    pthread_barrier_wait(&sch->barrier);
}

void scheduler_wait_finish(JobScheduler *sch)
{
    pthread_cond_wait(&sch->cv, &sch->mt);
}

Queue *initialize_queue()
{
    Queue *nq = (Queue *)malloc(sizeof(Queue));
    nq->head = NULL;
    nq->size = 0;
    return nq;
}

void destroy_queue(Queue *q)
{
    if (q == NULL)
        return;
    if (q->head == NULL)
    {
        free(q);
        return;
    }
    QueueNode *temp = q->head;
    QueueNode *next = q->head->next;
    while (next != NULL)
    {
        //free(temp->data);
        free(temp);
        temp = next;
        next = next->next;
    }
    //free(temp->data);
    free(temp);
    free(q);
}

void add(Queue *q, QueueNode *nn)
{
    if (q == NULL)
        return;
    if (q->head == NULL)
    {
        q->head = nn;
        q->size++;
        return;
    }
    QueueNode *temp = q->head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = nn;
    q->size++;
}

int isempty(Queue *q)
{
    if (q == NULL || q->head == NULL)
        return 1;
    return 0;
}

QueueNode *queue_pop(Queue *q)
{
    if (q == NULL || q->head == NULL)
        return NULL;
    QueueNode *nj = (QueueNode *)q->head;
    q->head = q->head->next;
    return nj;
}

void make_model_sets(HashTable *all_set, Vector *set1, Vector *set2, Vector *set3)
{
    return;
}

Vector *train_weights_testing(HashTable_w *W1, HashTable_w *T, Vector *weights)
{
    return NULL;
}

double *train_weights(HashTable *hash_table, HashTable_w *W1)
{
    double *b = weight_array_init(TF_IDF_SIZE * 2); // init array based on tf-idfs size
    Vector *full_W_pairs = vectorize_all_pairs(hash_table, W1);
    float threshold = THRESHOLD_VALUE;


    while ( threshold < 0.5 ) {
        b = thrd_model_training_wghts(full_W_pairs, b); // !THREADS

        Observation *new_pair_not_in_W;
        for (int i = 0; i < NEW_PAIRS_SIZE; i++)
        {
            new_pair_not_in_W = get_pair_in_x_excl_set(hash_table, W1);
            double px = p_logistic_function_full(new_pair_not_in_W, b);
            if (px < threshold || px > 1 - threshold)
            {
                vectorPushBack(full_W_pairs, new_pair_not_in_W);
                PairInfo_w *new_pair = (PairInfo_w *)safe_malloc(sizeof(PairInfo_w));
                new_pair->leftSpecId = new_pair_not_in_W->left_spec_id;
                new_pair->rightSpecId = new_pair_not_in_W->right_spec_id;
                new_pair->isMatch = new_pair_not_in_W->is_match;
                addToHashTable_w(W1, new_pair);
            }
        }
        resolve_transitivity(hash_table, new_pair_not_in_W, W1); //
        threshold += THRESHOLD_STEP * THRESHOLD_SLOPE;
    }

    print_positive_set(W1);
    return b;
}

double *dj;
pthread_mutex_t dj_access;

void calculate_dj(void *param) {
    int items_end = ((CalculateDJ*)param)->place + BATCH_SIZE;
    if ( items_end > ((CalculateDJ*)param)->pairs->itemsInserted) items_end = ((CalculateDJ*)param)->pairs->itemsInserted;
    for (int i = ((CalculateDJ*)param)->place ; i < items_end ; i++ ) {
        double px = p_logistic_function_full(((CalculateDJ*)param)->pairs->items[i], ((CalculateDJ*)param)->b);
        ec_nzero( pthread_mutex_lock(&dj_access),  "failed mutex lock");
        if ( i < TF_IDF_SIZE ) {
            //dj[i] = (px - ((Observation*)((CalculateDJ*)param)->pairs->items[i])->is_match) * ((Observation*)((CalculateDJ*)param)->pairs->items[i])->left_tf_idf->items
        } else {
            //dj[i] = 
            // i - TF_IDF_SIZE sta xj, tou right spec
        }
        ec_nzero( pthread_mutex_unlock(&dj_access), "failed mutex unlock");
    }
}

double *thrd_model_training_wghts(Vector *pairs, double *b) {
    dj = (double*)safe_calloc(TF_IDF_SIZE*2, sizeof(double));
    for ( int i = 0; i < TF_IDF_SIZE*2 ; i++ ) {
        dj[i] = 0;
    }
    JobScheduler *sch = scheduler_init(THREADS_NUM);
    int times_inserted, flag = 0;
    if ( pairs->itemsInserted <= BATCH_SIZE ) times_inserted = 1;
    else times_inserted = pairs->itemsInserted % BATCH_SIZE + 1;
    Job *new_job = (Job*)safe_malloc(sizeof(Job));
    new_job->function_execute = calculate_dj;
    CalculateDJ *to_pass = (CalculateDJ*)safe_malloc(sizeof(CalculateDJ));
    to_pass->pairs = pairs;
    while ( flag == 0 ) {
        to_pass->b = b;
        for ( int i = 1 ; i < times_inserted ; i++ ) {
            to_pass->place = (i-1)*BATCH_SIZE;
            new_job->any_parameter = to_pass;
            scheduler_submit_job(sch, new_job);
        }
        scheduler_execute_all(sch);
        scheduler_wait_finish(sch);
        for ( int i = 0 ; i < TF_IDF_SIZE*2 ; i++ ) {
            b[i] = b[i] - LEARNING_RATE*dj[i];
        }
        flag = 1;
        for ( int i = 0; i < TF_IDF_SIZE*2 ; i++ ) {
            if ( -LEARNING_RATE*dj[i] < E_VALUE ) {
                flag = 0;
                break;
            }
        }
    }
    scheduler_destroy(sch);
    free(to_pass);
    free(new_job);
    free(dj);
    return b;
}

void print_positive_set(HashTable_w *W1) {
    if ( W1 == NULL || W1->hashArray == NULL || W1->size == 0 ) return;

    printf(" ==> Printing Positive Matches... \n");
    for (int i = 0; i < W1->size; i++)
    {
        //print_chain_positive(W1->hashArray[i]->pairsList);
        PairInfo_w *list = W1->hashArray[i]->pairsList;
        while ( list != NULL ) {
            if ( list->isMatch > 0.5 ) {

                printf(" %s, %s, %d\n", list->leftSpecId, list->rightSpecId, list->isMatch);
            }
            list = list->nextPair;
        }
    }
    return;
}

void resolve_transitivity(HashTable *hash_table, Observation *pair, HashTable_w *W1)
{
    return;
}

double p_logistic_function_full(Observation *pair, double *weights_array)
{
    double f_x = weights_array[0];
    for ( int i = 0 ; i < TF_IDF_SIZE ; i++ ) {
        //f_x = f_x + ((tf_idfInfo*)pair->left_tf_idf->items[i])->tf_idfValue * weights_array[i];
    }
    for ( int i = TF_IDF_SIZE ; i < TF_IDF_SIZE*2 ; i++ ) {
        //f_x = f_x + ((tf_idfInfo*)pair->right_tf_idf->items[i-TF_IDF_SIZE])->tf_idfValue * weights_array[i];
    }

    double e_x = exp(-f_x);
    double p_x = 1.0 / (1.0 + e_x);

    printf("+++ VALUE FOUND : %f\n", p_x);
    return p_x;
}

double *weight_array_init(int size)
{
    double *new_array = (double *)safe_calloc(size, sizeof(double));
    for (int i = 0; i < size; i++)
    {
        new_array[i] = WEIGHTS_START_VAL;
    }
    return new_array;
}

Observation *get_pair_in_x_excl_set(HashTable *hash_table, HashTable_w *W1)
{
    srand(time(NULL));
    int count = 0;

    SpecInfo *randm_spc1;
    SpecInfo *randm_spc2;
    PairInfo_w *test_pair = get_first_pair_w(W1);
    if (test_pair == NULL)
    {
        printf("Error: Unable to find pairs W1 is not initialized.");
        exit(1);
    }
    while (test_pair != NULL)
    {
        randm_spc1 = hashtable_find_random_spec(hash_table);
        randm_spc2 = hashtable_find_random_spec(hash_table);
        while (strcmp(randm_spc1->specId, randm_spc2->specId) == 0)
        {
            if (count % 3 == 0 || count % 7 == 0)
            {
                randm_spc2 = hashtable_find_random_spec(hash_table);
            }
            else
            {
                randm_spc1 = hashtable_find_random_spec(hash_table);
            }
            count++;
        }
        test_pair = searchHashTable_w(W1, randm_spc1->specId, randm_spc2->specId);
        if ( test_pair == NULL ) {
            test_pair = searchHashTable_w(W1, randm_spc2->specId, randm_spc1->specId);
        }
        if ( count == 50000) {
            printf("Error: Unable to find pairs.");
            exit(1);
        }
    }

    Observation *new_pair = (Observation *)safe_malloc(sizeof(Observation));
    new_pair->left_spec_id = randm_spc1->specId;
    new_pair->left_tf_idf = randm_spc1->tfidf_vector;
    new_pair->right_spec_id = randm_spc2->specId;
    new_pair->right_tf_idf = randm_spc2->tfidf_vector;
    new_pair->is_match = -1;

    return new_pair;
}

PairInfo_w *get_first_pair_w(HashTable_w *W1)
{
    if (W1 == NULL || W1->hashArray == NULL || W1->size == 0)
        return NULL;
    for (int i = 0; i < W1->size; i++)
    {
        if (W1->hashArray[i]->pairsList != NULL)
        {
            return W1->hashArray[i]->pairsList;
        }
    }
    return NULL;
}

SpecInfo *hashtable_find_random_spec(HashTable *hash_table)
{
    int randm_pos = rand() % hash_table->size;

    while (hash_table->hashArray[randm_pos] == NULL || hash_table->hashArray[randm_pos]->specList == NULL)
    {
        randm_pos = rand() % hash_table->size;
    }

    return hash_table->hashArray[randm_pos]->specList->cliquePtr->specInfo;
}

void validate_model(HashTable_w *V, Vector *weights)
{
    return;
}

Vector *vectorize_all_pairs(HashTable *hash_table, HashTable_w *W1)
{
    Vector *full_batch = vectorInit();

    int hash_buckets = W1->size;
    for (int i = 0; i < hash_buckets; i++)
    {
        PairInfo_w *pair_list = W1->hashArray[i]->pairsList;
        while (pair_list != NULL)
        {
            char *left_id = pair_list->leftSpecId;
            char *right_id = pair_list->rightSpecId;
            int is_match = pair_list->isMatch;

            SpecNode *spec_node_l = searchHashTable(hash_table, left_id);
            SpecNode *spec_node_r = searchHashTable(hash_table, right_id);

            Vector *left_vector = get_spec_tf_idf_vector(spec_node_l);
            Vector *right_vector = get_spec_tf_idf_vector(spec_node_r);

            Observation *new_observation = (Observation *)safe_malloc(sizeof(Observation));
            new_observation->left_spec_id = left_id;
            new_observation->left_tf_idf = left_vector;
            new_observation->right_spec_id = right_id;
            new_observation->right_tf_idf = right_vector;
            new_observation->is_match = is_match;

            vectorPushBack(full_batch, new_observation);

            pair_list = pair_list->nextPair;
        }
    }
    return full_batch;
}

// TODO : observation_vector_free(full_batch) : observations are not freed automat.

Vector *get_spec_tf_idf_vector(SpecNode *spec_node)
{
    if (spec_node == NULL)
        return NULL;
    return spec_node->cliquePtr->specInfo->tfidf_vector;
}