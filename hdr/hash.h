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

HashBucket **initHashTable(int);

void addToHashTable(HashBucket **, SpecInfo *);

#endif