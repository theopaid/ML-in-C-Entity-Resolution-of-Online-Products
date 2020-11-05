#include "../hdr/includes.h"

HashTable *initHashTable(int specSum)
{
    int bucketsToAlloc = specSum / 0.7; // we need a load factor of at least 70%, to minimize collisions
    //HashBucket **newHashTable = (HashBucket **)safe_calloc(bucketsToAlloc, sizeof(HashBucket *));
    HashTable *hashTable = (HashTable *)safe_malloc(sizeof(HashTable));
    hashTable->size = bucketsToAlloc;
    hashTable->hashArray = (HashBucket **)safe_calloc(bucketsToAlloc, sizeof(HashBucket *));

    for (int i = 0; i < bucketsToAlloc; i++)
    {
        hashTable->hashArray[i] = NULL;
        //newHashTable[i] = (HashBucket *)safe_malloc(sizeof(HashBucket));
        //newHashTable[i]->specList = NULL;
    }
    printf("table size init: %ld\n", hashTable->size);
    return hashTable;
}

// Java's java.lang.String hashCode() method
unsigned long long hashFunction(char *specId)
{
    unsigned long long h = 0;
    //int offset = 0;
    long len = strlen(specId);
    //char val[] = specId;
    for (int i = 0; i < len; i++)
    {
        //printf("h: %llu, specid: %d\n", h, specId[i]);
        h = 31 * h + specId[i];
    }

    return h;
}

void addToHashTable(HashTable *hashTable, SpecInfo *specToAdd)
{
    // call hash function, allocate specnode and cliqueNode and store them in hashArray
    unsigned long long hash = hashFunction(specToAdd->specId);
    printf("hash: %llu, size: %ld\n", hash, hashTable->size);
    long posInHashTable = hash % hashTable->size;
    SpecNode *newSpecNode = initSpecNode();
    CliqueNode *newCliqueNode = initCliqueNode();

    newCliqueNode->specInfo = specToAdd;
    newSpecNode->cliquePtr = newCliqueNode;
    printf("Inserting in pos: %ld\n", posInHashTable);
    if (hashTable->hashArray[posInHashTable] == NULL)
    {
        hashTable->hashArray[posInHashTable] = (HashBucket *)safe_malloc(sizeof(HashBucket));
        hashTable->hashArray[posInHashTable]->specList = NULL;
    }
    insertInChain(hashTable->hashArray[posInHashTable], newSpecNode);
}

void insertInChain(HashBucket *bucketDst, SpecNode *newSpecNode)
{
    //puts("HERE ");
    //puts(" ");
    if (bucketDst->specList == NULL)
    {
        //puts("HERE 1");
        //puts(" ");
        bucketDst->specList = newSpecNode;
        return;
    }
    //puts("HERE 1.5");
    SpecNode *dstPtr = bucketDst->specList;
    //puts("HERE 2");
    while (dstPtr->nextSpec != NULL)
    {
        //puts("HERE 3");
        dstPtr = dstPtr->nextSpec;
    }
    dstPtr->nextSpec = newSpecNode;
    //puts("HERE 4");
}

SpecNode *initSpecNode()
{
    SpecNode *newSpecNode = (SpecNode *)safe_malloc(sizeof(SpecNode));
    newSpecNode->cliquePtr = NULL;
    newSpecNode->nextSpec = NULL;

    return newSpecNode;
}

HashBucket *searchHashTable(HashTable *hashTable, char *specId)
{
    int posInHashTable = hashFunction(specId) % hashTable->size;

    return hashTable->hashArray[posInHashTable];
}

void freeSpecNode(SpecNode *specNode)
{
    if (specNode == NULL)
        return;
    freeCliqueNode(specNode->cliquePtr);
    freeSpecNode(specNode->nextSpec);
    free(specNode);
}

void freeHashBucket(HashBucket *hashBucket)
{
    if (hashBucket == NULL)
    {
        return;
    }
    freeSpecNode(hashBucket->specList);
    free(hashBucket);
}

void freeHashTable(HashTable *hashTable)
{
    if (hashTable == NULL)
        return;
    for (int i = 0; i < hashTable->size; i++)
    {
        freeHashBucket(hashTable->hashArray[i]);
    }
    free(hashTable->hashArray);
    free(hashTable);
}
