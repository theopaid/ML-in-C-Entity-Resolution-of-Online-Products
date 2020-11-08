#ifndef _READCSV_H
#define _READCSV_H

#include "../hdr/hash.h"

char *getField(char *line, int fieldNum);

void readDictionary(char *fileName, HashTable *hashTable);

#endif