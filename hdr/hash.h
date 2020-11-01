#ifndef _HASH_H
#define _HASH_H

#include "../hdr/clique.h"

typedef struct HashBucket HashBucket;
typedef struct SpecNode SpecNode;

struct HashBucket
{
    SpecNode *specList;
};

struct SpecNode
{
    CliqueNode *cliquePtr;
    SpecNode *nextSpec;
};

SpecNode *initSpecNode();

HashBucket **initHashTable(int);

int hashFunction(char *);

void addToHashTable(HashBucket **, SpecInfo *);

void insertInChain(HashBucket *bucketDst, SpecNode *newSpecNode);

HashBucket *searchHashTable(HashBucket **hashTable, char *specId);

#endif