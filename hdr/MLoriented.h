#ifndef _ML_H
#define _ML_H

typedef struct MLInfo MLInfo;

struct MLInfo
{
    char *word;
    int bow;
    double tf_idf;
    double rog_reg;
};

MLInfo *initMLinfo(char *string);

void initML(HashTable *hashTable, Vector *stopWords);

void initMLinChain(SpecNode *head);

void initMLforSpec(SpecNode *specNode);

void populateBOW(SpecInfo *specInfo);

void extractWordAndProcess(Vector *vectorMLinfo, char *sentence);

void traverseInfoList(Vector *vectorMLinfo, InfoList *infoList);

void processWordAddToBow(Vector *vectorMLinfo, char *word);

int isInStopwords(char *word);

void addWordToBOW(Vector *vectorMLinfo, char *word);

int incrementIfAlreadyInBOW(Vector *vectorMLinfo, char *word);

void copyToEmptyVector(Vector *dstVector, Vector *srcVector);

void printBOW(SpecInfo *specInfo);

void freeMLinfo(Vector *vectorMLinfo);

#endif