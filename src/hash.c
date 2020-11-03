#include "../hdr/includes.h"

int hashTableSize;

HashBucket **initHashTable(int specSum)
{
    int bucketsToAlloc = specSum / 0.7; // we need a load factor of at least 70%, to minimize collisions
    HashBucket **newHashTable = (HashBucket **)safe_calloc(bucketsToAlloc, sizeof(HashBucket *));

    for (int i = 0; i < bucketsToAlloc; i++)
    {
        newHashTable[i] = NULL;
    }
    hashTableSize = bucketsToAlloc;

    return newHashTable;
}

// Java's java.lang.String hashCode() method
int hashFunction(char *specId)
{
    int h = 0;
    int offset = 0;
    int len = strlen(specId);
    for (int i = 0; i < len; i++)
    {
        h = 31 * h + specId[offset++];
    }

    return h;
}

void addToHashTable(HashBucket **hashTable, SpecInfo *specToAdd)
{
    // call hash function, allocate specnode and cliqueNode and store them in hashArray
    int hash = hashFunction(specToAdd->specId);
    int posInHashTable = hash % hashTableSize;
    SpecNode *newSpecNode = initSpecNode();
    CliqueNode *newCliqueNode = initCliqueNode();

    newCliqueNode->specInfo = specToAdd;
    newSpecNode->cliquePtr = newCliqueNode;
    insertInChain(hashTable[posInHashTable], newSpecNode);
}

void insertInChain(HashBucket *bucketDst, SpecNode *newSpecNode)
{
    if (bucketDst->specList == NULL)
    {
        bucketDst->specList = newSpecNode;
        return;
    }
    SpecNode *dstPtr = bucketDst->specList;
    while (dstPtr->nextSpec != NULL)
    {
        dstPtr = dstPtr->nextSpec;
    }
    dstPtr->nextSpec = newSpecNode;
}

SpecNode *initSpecNode()
{
    SpecNode *newSpecNode = (SpecNode *)safe_malloc(sizeof(SpecNode));
    newSpecNode->cliquePtr = NULL;
    newSpecNode->nextSpec = NULL;

    return newSpecNode;
}

HashBucket *searchHashTable(HashBucket **hashTable, char *specId)
{
    int posInHashTable = hashFunction(specId) % hashTableSize;

    return hashTable[posInHashTable];
}