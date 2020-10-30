#include "../hdr/includes.h"

HashBucket **initHashTable(int specSum)
{
    HashBucket **newHashTable = (HashBucket **)safe_malloc(sizeof(HashBucket *) * specSum / 5);
    return newHashTable;
}

// void addToHashTable(HashBucket **hashTable, SpecInfo *specToAdd) // ! commented to pass unity tests
// {
//     //todo: call hash function, allocate specnode and cliqueNode and store them in hashArray
// }