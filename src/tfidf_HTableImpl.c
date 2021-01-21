#include "../hdr/includes.h"

HashTable_gen *idfHTable;

double totalSpecs;

////////// INITIALIZATIONS //////////////////
tf_idfInfo *initTFIDFinfo(char *word)
{
    tf_idfInfo *newInfo = (tf_idfInfo *)safe_malloc(sizeof(tf_idfInfo));
    newInfo->word = createString(word);
    newInfo->tf_idfValue = 0.0;
    newInfo->next = NULL;

    return newInfo;
}

idfInfo *initIDFinfo(char *word)
{
    idfInfo *newInfo = (idfInfo *)safe_malloc(sizeof(idfInfo));
    newInfo->word = createString(word);
    newInfo->idfValue = 0.0;
    newInfo->next = NULL;

    return newInfo;
}

////////// SEARCH FUNCTIONS FOR TF_IDF AND IDF HTABLES //////////////////
tf_idfInfo *searchTFIDF_htable(HashTable_gen *hashTable, char *word)
{
    int posInHTable = hashFunction(word) % hashTable->size;
    if (hashTable->hashArray[posInHTable] == NULL)
        return NULL;
    return searchTFIDF_htableChain(hashTable->hashArray[posInHTable]->bucketList, word);
}

tf_idfInfo *searchTFIDF_htableChain(tf_idfInfo *listHead, char *word)
{
    tf_idfInfo *listPtr = listHead;
    while (listPtr != NULL)
    {
        char *currentWord = listPtr->word;
        if (same_string(currentWord, word))
        { // found
            break;
        }
        listPtr = listPtr->next;
    }
    return listPtr;
}

idfInfo *searchIDF_htable(HashTable_gen *hashTable, char *word)
{
    int posInHTable = hashFunction(word) % hashTable->size;
    if (hashTable->hashArray[posInHTable] == NULL)
        return NULL;
    return searchIDF_htableChain(hashTable->hashArray[posInHTable]->bucketList, word);
}

idfInfo *searchIDF_htableChain(idfInfo *listHead, char *word)
{
    idfInfo *listPtr = listHead;
    while (listPtr != NULL)
    {
        char *currentWord = listPtr->word;
        if (same_string(currentWord, word))
        { // found
            break;
        }
        listPtr = listPtr->next;
    }
    return listPtr;
}

////////// INSERT FUNCTIONS FOR TF_IDF AND IDF HTABLES //////////////////
void addToTFIDF_htable(HashTable_gen *hashTable, char *word)
{
    unsigned long long hash = hashFunction(word);
    long posInHTable = hash % hashTable->size;
    if (hashTable->hashArray[posInHTable] == NULL)
    {
        hashTable->hashArray[posInHTable] = (HashBucket_gen *)safe_malloc(sizeof(HashBucket_gen));
        hashTable->hashArray[posInHTable]->bucketList = NULL;
    }
    int insertedNewFlag = insertInTFIDFchain(hashTable->hashArray[posInHTable], word);
    if (insertedNewFlag == 1)
        hashTable->itemsInserted++;
}

int insertInTFIDFchain(HashBucket_gen *bucketDst, char *word)
{
    tf_idfInfo *listPtr = bucketDst->bucketList;
    if (listPtr == NULL)
    {
        tf_idfInfo *newInfo = initTFIDFinfo(word);
        newInfo->tf_idfValue++;
        bucketDst->bucketList = newInfo;
        addToIDF_htable(idfHTable, word);
        return 1;
    }
    tf_idfInfo *prevPtr = NULL;
    while (listPtr != NULL)
    {
        char *currentWord = listPtr->word;
        if (same_string(currentWord, word))
        { // found in list
            listPtr->tf_idfValue++;
            return 0;
        }

        prevPtr = listPtr;
        listPtr = listPtr->next;
    }
    // not found in list
    tf_idfInfo *newInfo = initTFIDFinfo(word);
    newInfo->tf_idfValue++;
    prevPtr->next = newInfo;
    addToIDF_htable(idfHTable, word);
    return 1;
}

void addToIDF_htable(HashTable_gen *hashTable, char *word)
{
    unsigned long long hash = hashFunction(word);
    long posInHTable = hash % hashTable->size;
    if (hashTable->hashArray[posInHTable] == NULL)
    {
        hashTable->hashArray[posInHTable] = (HashBucket_gen *)safe_malloc(sizeof(HashBucket_gen));
        hashTable->hashArray[posInHTable]->bucketList = NULL;
    }
    int insertedNewFlag = insertInIDFchain(hashTable->hashArray[posInHTable], word);
    if (insertedNewFlag == 1)
        hashTable->itemsInserted++;
}

int insertInIDFchain(HashBucket_gen *bucketDst, char *word)
{
    idfInfo *listPtr = bucketDst->bucketList;
    if (listPtr == NULL)
    {
        idfInfo *newInfo = initIDFinfo(word);
        newInfo->idfValue++;
        bucketDst->bucketList = newInfo;
        return 1;
    }
    idfInfo *prevPtr = NULL;
    while (listPtr != NULL)
    {
        char *currentWord = listPtr->word;
        if (same_string(currentWord, word))
        { // found in list
            listPtr->idfValue++;
            return 0;
        }

        prevPtr = listPtr;
        listPtr = listPtr->next;
    }
    // not found in list
    idfInfo *newInfo = initIDFinfo(word);
    newInfo->idfValue++;
    prevPtr->next = newInfo;
    return 1;
}

///////////////// CALCULATIONS FOR FINAL TF_IDF VALUES ///////////////////////
void calculateFinalIDF(HashTable_gen *hashTable)
{
    int totalWords = hashTable->itemsInserted;
    int htableSize = hashTable->size;
    idfInfo *listPtr = NULL;
    for (int i = 0; i < htableSize; i++)
    {
        if (hashTable->hashArray[i] == NULL)
            continue;
        listPtr = hashTable->hashArray[i]->bucketList;
        while (listPtr != NULL)
        {
            listPtr->idfValue = log(totalSpecs / listPtr->idfValue);

            listPtr = listPtr->next;
        }
    }
}

Vector *calculateFinalTFIDFforSpec(HashTable_gen *hashTable)
{
    long totalWords = hashTable->itemsInserted;
    int htableSize = hashTable->size;
    tf_idfInfo *listPtr = NULL;
    Vector *tfidfVector = vectorInit();
    for (int i = 0; i < htableSize; i++)
    {
        if (hashTable->hashArray[i] == NULL)
            continue;
        listPtr = hashTable->hashArray[i]->bucketList;
        while (listPtr != NULL)
        {
            listPtr->tf_idfValue = listPtr->tf_idfValue / totalWords;
            idfInfo *idfNodeForWord = searchIDF_htable(idfHTable, listPtr->word);
            vectorPushBack(tfidfVector, listPtr);
            if (idfNodeForWord == NULL)
                continue;
            listPtr->tf_idfValue = listPtr->tf_idfValue * idfNodeForWord->idfValue;

            listPtr = listPtr->next;
        }
    }
    // calculations done. Let's get the best 1.000 words from tf_idf
    sortTFIDFvector(tfidfVector);
    return tfidfVector;
}

void *sortTFIDFvector(Vector *tfidfVector)
{ // let's do a selection sort
    int i, j, min_idx;
    int itemsCount = vectorItemsCount(tfidfVector);

    // One by one move boundary of unsorted subarray
    for (i = 0; i < itemsCount - 1; i++)
    {
        if (i > TF_IDF_SIZE)
            break;
        // Find the maximum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < itemsCount; j++)
            if (((tf_idfInfo *)tfidfVector->items[j])->tf_idfValue > ((tf_idfInfo *)tfidfVector->items[min_idx])->tf_idfValue)
                min_idx = j;

        // Swap the found maximum element with the first element
        tf_idfInfo *tfidfInfoToSwap = (tf_idfInfo *)tfidfVector->items[i];
        tfidfVector->items[i] = (tf_idfInfo *)tfidfVector->items[min_idx];
        tfidfVector->items[min_idx] = tfidfInfoToSwap;
    }
}

void selectionSort(Vector *idfVector)
{
    int i, j, min_idx;
    int itemsCount = vectorItemsCount(idfVector);

    // One by one move boundary of unsorted subarray
    for (i = 0; i < itemsCount - 1; i++)
    {
        // Find the maximum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < itemsCount; j++)
            if (((idfInfo *)idfVector->items[j])->tf_idfValue > ((idfInfo *)idfVector->items[min_idx])->tf_idfValue)
                min_idx = j;

        // Swap the found maximum element with the first element
        idfInfo *idfInfoToSwap = (idfInfo *)idfVector->items[i];
        idfVector->items[i] = (idfInfo *)idfVector->items[min_idx];
        idfVector->items[min_idx] = idfInfoToSwap;
    }
}