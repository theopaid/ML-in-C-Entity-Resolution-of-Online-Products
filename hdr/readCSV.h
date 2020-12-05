#ifndef _READCSV_H
#define _READCSV_H

#include "../hdr/hash.h"
#include "../hdr/vectorImpl.h"

char *getField(char *line, int fieldNum);

void readDictionary(char *fileName, HashTable *hashTable);

Vector *readCsvToVector(char *fileName);

#endif