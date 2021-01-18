#include "../hdr/includes.h"

HashTable_w *initHashTable_w(int pairsSum)
{
    int bucketsToAlloc = pairsSum / 0.5; // we need a load factor of at least 70%, to minimize collisions
                                         // let's double the size here
    bucketsToAlloc = closestPrime(bucketsToAlloc);

    HashTable_w *newHashTable = (HashTable_w *)safe_malloc(sizeof(HashTable_w));
    newHashTable->size = bucketsToAlloc;
    newHashTable->hashArray = (HashBucket_w **)safe_calloc(bucketsToAlloc, sizeof(HashBucket_w *));

    for (int i = 0; i < bucketsToAlloc; i++)
    {
        newHashTable->hashArray[i] = NULL;
    }
    return newHashTable;
}

unsigned long long hashFunction_w(char *valueToHash)
{
    unsigned long long h = 0;
    // to be implemented

    return h;
}

void addToHashTable_w(HashTable_w *hashTable, PairInfo_w *newPairInfo)
{
    unsigned long long hash = hashFunction_w(NULL);
    long posInHashTable = hash % hashTable->size;

    if (hashTable->hashArray[posInHashTable] == NULL)
    {
        hashTable->hashArray[posInHashTable] = (HashBucket_w *)safe_malloc(sizeof(HashBucket_w));
        hashTable->hashArray[posInHashTable]->pairsList = NULL;
    }
    insertInChain_w(hashTable->hashArray[posInHashTable], newPairInfo);
}

void insertInChain_w(HashBucket_w *bucketDst, PairInfo_w *newPairInfo)
{
    if (bucketDst->pairsList == NULL)
    {
        bucketDst->pairsList = newPairInfo;
        return;
    }

    PairInfo_w *pairsListPtr = bucketDst->pairsList;
    while (pairsListPtr != NULL)
    {
        pairsListPtr = pairsListPtr->nextPair;
    }
    pairsListPtr->nextPair = newPairInfo;
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

PairInfo *searchHashTable_w(HashTable_w *hashTable, char *valueToSearch)
{
    // to be implemented
    return NULL;
}

PairInfo_w *searchChain_w(PairInfo_w *pairsListHead, char *valueToSearch)
{
    // to be implemented
    return NULL;
}

void freePairInfo_w(PairInfo_w *pairInfoNode)
{
    if (pairInfoNode == NULL)
        return;
    freePairInfoNode_w(pairInfoNode->nextPair);
    free(pairInfoNode->leftSpecId);
    free(pairInfoNode->rightSpecId);
    free(pairInfoNode);
}

void freeHashBucket_w(HashBucket_w *hashBucket)
{
    if (hashBucket == NULL)
    {
        return;
    }
    freePairInfo_w(hashBucket->pairsList);
    free(hashBucket);
}

void freeHashTable_w(HashTable_w *hashTable)
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