#ifndef _HASH_H
#define _HASH_H

#include "../hdr/clique.h"

typedef struct HashTable HashTable;
typedef struct HashBucket HashBucket;
typedef struct SpecNode SpecNode;

struct HashTable
{
    long size;
    HashBucket **hashArray;
};

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

HashTable *initHashTable(int);

unsigned long long hashFunction(char *);

void addToHashTable(HashTable *, SpecInfo *);

void insertInChain(HashBucket *bucketDst, SpecNode *newSpecNode);

HashBucket *searchHashTable(HashTable *hashTable, char *specId);

void freeSpecNode(SpecNode *specNode);

void freeHashBucket(HashBucket *hashBucket);

void freeHashTable(HashTable *hashTable);

#endif