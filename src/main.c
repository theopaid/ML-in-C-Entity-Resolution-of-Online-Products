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

    //  1.  Analyze all Json in X and pairs in W to form Cliques. W+ is the new set with Clique relationships.
    
    HashTable *hashTable = initHashTable(count_datafiles(datasetX));
    puts("==> Reading Dataset X ...");
    read_from_dir(datasetX, hashTable);

    puts("==> Reading Dataset W and making Cliques / Anti-cliques ...");
    readDictionary(datasetW, hashTable);

    FILE *fptr = open_file("./output/matches.txt");
    FILE *fptr_miss = open_file("./output/miss_matches.txt");

    puts("==> Extracting matches from Cliques ...");
    printAllMatches(hashTable, fptr);
    puts("==> Extracting miss-matches from Cliques ...");
    printAllMissMatches(hashTable, fptr_miss);

    resetAllPrintedStatus(hashTable);

    fclose(fptr);
    fclose(fptr_miss);

    //  2.  Get the 60% of W+ as the initial training set W1+, 20% as testing set T and 20% as validation set V
    
    puts("==> Creating from W+ : Training, Evaluation and Test datasets ...");
    createPairDatasets(); //  result in global trainingSet_HTable, evaluationSet_HTable, testSet_HTable

    HashTable_gen *stopwordsHTable = saveStopwords("./Datasets/stopwords.csv");
    
    //  3.  Calculate tf-idf and reduce dimensions to 1000 top words for each Json.
       
    createTFIDFvectors(hashTable, stopwordsHTable);
    freeHashTable_gen(stopwordsHTable);
    //printTFIDFvectors(hashTable);

    //  4.  Train the model with W1+ and all the pairs in X that don't belong to the set W1+ using a defined threshhold.
    //      This way, the training set W?+ will be enhanced with new pairs that satisfy the threshhold condition.
    //      The threshold will be tested with stable increase, or with increased increasement.
    //      What we get is the b (weight vector) values for each word (dimension) of the WN+ training set.
    //      The training of each W?+ set will be done with batches in THREADS using a Job Scheduler (on stochastic gradient descend).
    //  4.1.    In need to define the best values for:
    //              (learing rate, #of threads, batch size, threshold value/step)
    //          The pairs that will be checked and added in W?+ will be pairs only in the testing set T (20% of W+).

    HashTable_w *W1 = getTrainingSet();
    HashTable_w *T = getTestSet();
    HashTable_w *V = getEvaluationSet();
    
    double *b = train_weights(hashTable, W1);
    int threads = 10, batch_size = 1000;
    float threshold = 0.3;
    double acc = model_testing_testing(hashTable, T, b, threads, threshold, batch_size);
    puts("==> (*)=======================================(*)");
    printf("==> (+) Model testing accuracy : [%f]%% \n", acc*100);
    printf("==> (|) Threads : %d\n", threads);
    printf("==> (|) Batch size : %d\n", batch_size);
    printf("==> (|) Threshold : %f\n", threshold);
    puts("==> (*)=======================================(*)");

    threads = 10;
    batch_size = 1500;
    acc = model_testing_testing(hashTable, T, b, threads, threshold, batch_size);
    puts("==> (*)=======================================(*)");
    printf("==> (+) Model testing accuracy : [%f]%% \n", acc*100);
    printf("==> (|) Threads : %d\n", threads);
    printf("==> (|) Batch size : %d\n", batch_size);
    printf("==> (|) Threshold : %f\n", threshold);
    puts("==> (*)========================================(*)");

    threads = 10;
    batch_size = 500;
    threshold = 0.3;
    acc = model_testing_testing(hashTable, T, b, threads, threshold, batch_size);
    puts("==> (*)=======================================(*)");
    printf("==> (+) Model testing accuracy : [%f]%% \n", acc*100);
    printf("==> (|) Threads : %d\n", threads);
    printf("==> (|) Batch size : %d\n", batch_size);
    printf("==> (|) Threshold : %f\n", threshold);
    puts("==> (*)=======================================(*)");

    threads = 10;
    batch_size = 1000;
    threshold = 0.2;
    acc = model_testing_testing(hashTable, T, b, threads, threshold, batch_size);
    puts("==> (*)=======================================(*)");
    printf("==> (+) Model testing accuracy : [%f]%% \n", acc*100);
    printf("==> (|) Threads : %d\n", threads);
    printf("==> (|) Batch size : %d\n", batch_size);
    printf("==> (|) Threshold : %f\n", threshold);
    puts("==> (*)=======================================(*)");

    //  5.  We use these b values to validate the model and estimate the possibility (accuracy) of the model.
    //      This time we pass the pairs in the V set to the model and we use the threads to separate the V set in batches.
    //      We calculate the prediction of our model (using b) and check correnspondence with the actual values in V to find the accuracy.

    acc = model_testing(hashTable, V, b);
    printf("==> (+) Model validation accuracy : [%f]%% \n", acc*100);

    freeHashTable(hashTable);
    free(b);
    clock_t end = clock();
    timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
    printInsights();
    return 0;
}

void printExecTime()
{
    printf("Execution time: %f\n", timeSpent);
}
