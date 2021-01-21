#include "../hdr/includes.h"

HashTable_gen *initHashTable_gen(int initItemsN)
{
    int bucketsToAlloc = initItemsN / 0.5; // we need a load factor of at least 70%, to minimize collisions
                                           // let's double the size here
    bucketsToAlloc = closestPrime(bucketsToAlloc);

    HashTable_gen *newHashTable = (HashTable_gen *)safe_malloc(sizeof(HashTable_gen));
    newHashTable->size = bucketsToAlloc;
    newHashTable->hashArray = (HashBucket_gen **)safe_calloc(bucketsToAlloc, sizeof(HashBucket_gen *));

    for (int i = 0; i < bucketsToAlloc; i++)
    {
        newHashTable->hashArray[i] = NULL;
    }
    return newHashTable;
}

PairInfo_w *initPairInfo_w(char *leftSpecId, char *rightSpecId, int isMatch)
{
    PairInfo_w *newPairInfo = safe_malloc(sizeof(PairInfo_w));
    newPairInfo->leftSpecId = createString(leftSpecId);
    newPairInfo->rightSpecId = createString(rightSpecId);
    newPairInfo->isMatch = isMatch;
    newPairInfo->nextPair = NULL;

    return newPairInfo;
}

void freeHashBucket_gen(HashBucket_gen *hashBucket)
{
    if (hashBucket == NULL)
    {
        return;
    }
    free(hashBucket);
}

void freeHashTable_gen(HashTable_gen *hashTable)
{
    if (hashTable == NULL)
        return;
    for (int i = 0; i < hashTable->size; i++)
    {
        freeHashBucket_w(hashTable->hashArray[i]);
    }
    free(hashTable->hashArray);
    free(hashTable);
}