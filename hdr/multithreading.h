#ifndef _MULTI_THRD_
#define _MULTI_THRD_

#include <pthread.h>

typedef struct JobScheduler JobScheduler;
typedef struct Queue Queue;
typedef struct QueueNode QueueNode;
typedef struct Job Job;

struct JobScheduler {
    int no_of_threads; // number of execution threads
    Queue *q;   // Queue that holds submitted jobs / tasks
    pthread_t *tids;   // Execution threads

    // mutex, condition variable, ...
};

struct Queue {
    QueueNode *head;
    int size;
};

struct QueueNode {
    void *data;
    QueueNode *next;
};

struct Job {
    void *function_to_execute;
    int type;   // 1 - for training in stochastic descend, 2 - for testing
};


#include "../hdr/hash.h"
#include "../hdr/vectorImpl.h"

/**
 * @brief Calculates the word vector for all items in the hash_table, using specified stopwords.
 * @param hash_table The HashTable with the items.
 * @param stopwords The vector with the stopwords.
 * @returns the word vector for all items in the HashTable.
 */
Vector *create_words_vector(HashTable *hash_table, Vector* stopwords);

/**
 * @brief Reduce the dimension of the word_vector given to a specified new_dimension and produce a new word vector. If the dimension given is bigger than the words in the vector, the vector will remain unchanged.
 * @param word_vector The vector with the words to be used.
 * @param new_dimension The number of words the new vector will have.
 * @returns The new word vector produced.
 */
Vector *reduce_vector_dimension(Vector *word_vector, int new_dimension);

/**
 * @brief Read the pairs in a csv dataset file and vectorize them.
 * @param dataset_path The path to the dataset csv file.
 * @returns The vector with all pairs in the csv file.
 */
Vector *read_csv_pairs_dataset(char *dataset_path);

/**
 * @brief Shuffle the vector given and produce a new vector.
 * @param vector The vector to be shuffled.
 * @returns The new shuffled vector.
 */
Vector *shuffle_vector(Vector *vector);

/**
 * @brief Make 3 new vectors using items in a specified vector. Percentages are 60%, 20% and 20%.
 * @param[in] all_set The vector with all the items to be separated.
 * @param[out] set1 60% of all_set.
 * @param[out] set2 20% of all_set.
 * @param[out] set3 20% of all_set.
 */
void make_model_sets(Vector *all_set, Vector *set1, Vector *set2, Vector *set3);

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
 * @brief Test the model using pairs in V, given weights and test_values and print the resulting accuracy.
 * @param V The validation set.
 * @param weights The weights vector that has been calculated by training.
 * @param test_values The test_values of the model.
 */
void test_model(Vector *V, Vector *weights, Vector *test_values);

#endif