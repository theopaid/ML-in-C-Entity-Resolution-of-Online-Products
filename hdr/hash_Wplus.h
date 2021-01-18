#ifndef _HASHWPLUS_H
#define _HASHWPLUS_H

#include "../hdr/clique.h"
#include "../hdr/readCSV.h"

typedef struct HashTable_w HashTable_w;
typedef struct HashBucket_w HashBucket_w;
typedef struct PairInfo_w PairInfo_w;

struct HashTable_w
{
    long size;
    HashBucket_w **hashArray;
};

struct HashBucket_w
{
    PairInfo_w *pairsList;
};

struct PairInfo_w
{
    char *leftSpecId;
    char *rightSpecId;
    int isMatch;
    PairInfo_w *nextPair;
};

HashTable_w *initHashTable_w(int pairsSum);

unsigned long long hashFunction_w(char *valueToHash);

void addToHashTable_w(HashTable_w *hashTable, PairInfo_w *newPairInfo);

void insertInChain_w(HashBucket_w *bucketDst, PairInfo_w *newPairInfo);

PairInfo_w *initPairInfo_w(char *leftSpecId, char *rightSpecId, int isMatch);

PairInfo *searchHashTable_w(HashTable_w *hashTable, char *valueToSearch);

PairInfo_w *searchChain_w(PairInfo_w *pairsListHead, char *valueToSearch);

void freePairInfo_w(PairInfo_w *pairInfoNode);

void freeHashBucket_w(HashBucket_w *hashBucket);

void freeHashTable_w(HashTable_w *hashTable);

#endif