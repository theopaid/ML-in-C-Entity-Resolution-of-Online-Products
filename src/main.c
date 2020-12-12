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

    //Vector *stopwords = readCsvToVector("./Datasets/stopwords.csv");
    //printf("stopwords: %s\n", (char *)vectorGet(stopwords, 22));

    //initML(hashTable, stopwords);

    freeHashTable(hashTable);
    //freeVector(stopwords);

    clock_t end = clock();
    timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
    printInsights();
    return 0;
}

void printExecTime()
{
    printf("Execution time: %f\n", timeSpent);
}
