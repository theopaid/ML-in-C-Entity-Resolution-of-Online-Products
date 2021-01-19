#include "../hdr/includes.h"

double timeSpent;

int main(int argc, char **argv)
{
    clock_t begin = clock();

    if (!validArgs(argc, argv))
    {
        printf("The arguments given are invalid. Try again.\n");
        return -1;
    }

    char *datasetX, *datasetW;
    getArgs(&datasetX, &datasetW, argv);
    puts(datasetW);
    puts(datasetX);

    HashTable *hashTable = initHashTable(count_datafiles(datasetX));

    read_from_dir(datasetX, hashTable);

    readDictionary(datasetW, hashTable);

    FILE *fptr = open_file("./output/matches.txt");
    FILE *fptr_miss = open_file("./output/miss_matches.txt");

    puts("------ MATCHES ------");
    printAllMatches(hashTable, fptr);
    puts("------ MISS MATCHES ------");
    printAllMissMatches(hashTable, fptr_miss);

    resetAllPrintedStatus(hashTable);

    fclose(fptr);
    fclose(fptr_miss);

    Vector *stopwords = readCsvToVector("./Datasets/stopwords.csv");

    createIDFvector(hashTable, stopwords);
    double *w_array = model_fittingV2(hashTable, getTrainingPairsVector());
    puts("==> Model training done...");
    // for (int i = 0; i < 3000; i++)
    // {
    //     printf("w[%d] : %f || ", i, w_array[i]);
    // }
    modelEvaluation(hashTable, getEvaluationPairsVector(), w_array);

    puts("==> De-allocating structures...");
    freePairVector((Vector *)getTrainingPairsVector);
    freePairVector((Vector *)getEvaluationPairsVector);

    //  From here on the part 3 will be implemented

    //  1.  Analyze and vectorize all Json in X with tf-idf
    //  2.  Reduce dimensions to ex. 1000, 500, ..., most significant values (words with highest average tf-idf)
    
    HashTable *hash_table = initHashTable(count_datafiles(datasetX));
    read_from_dir(datasetX, hash_table); // Read datasetX to hashTable

    createIDFvector(hash_table, stopwords);

    //  3.  Read the dataset W, making cliques and anticliques. This way we get set W+.
    
    readDictionary(datasetW, hash_table);
    
    //  4.  Get the 60% of W+ as the initial training set W1+, 20% as testing set T and 20% as validation set V

    make_model_sets(hash_table, W1, T, V);

    //  5.  Train the model with W1+ and all the pairs in X that don't belong to the set W1+ using a defined threshhold.
    //      This way, the training set W?+ will be enhanced with new pairs that satisfy the threshhold condition.
    //      The threshold will be tested with stable increase, or with increased increasement.
    //      What we get is the b (weight vector) values for each word (dimension) of the WN+ training set.
    //      The training of each W?+ set will be done with batches in THREADS using a Job Scheduler (on stochastic gradient descend).
    //  5.1.    In need to define the best values for:
    //              (learing rate, #of threads, batch size, threshold value/step)
    //          The pairs that will be checked and added in W?+ will be pairs only in the testing set T (20% of W+), 
    //          after that the training set will run for all pairs in X.

    Vector *test_values = init_test_values();
    Vector *b;
    
    b = train_weights_testing(W1, T, test_values);

    b = train_weights(W1, datasetX, test_values);

    //  6.  We use these b values to validate the model and estimate the possibility (accuracy) of the model.
    //      This time we pass the pairs in the V set to the model and we use the threads to separate the V set in batches.
    //      We calculate the prediction of our model (using b) and check correnspondence with the actual values in V to find the accuracy.

    test_model(V, b, test_values);



    freeHashTable(hashTable);
    freeVector(stopwords);

    clock_t end = clock();
    timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
    printInsights();
    return 0;
}

void printExecTime()
{
    printf("Execution time: %f\n", timeSpent);
}
