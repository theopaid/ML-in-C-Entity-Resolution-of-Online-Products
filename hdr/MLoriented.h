#ifndef _ML_H
#define _ML_H

typedef struct MLInfo MLInfo;
typedef struct idfInfo idfInfo;

struct MLInfo
{
    char *word;
    int bow;
    double tf_idf;
    double rog_reg;
};

struct idfInfo
{
    char *word;
    double timesSeen;
    int alreadySeenInSpecFlag;
    double idfValue;
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

idfInfo *initIDFinfo(char *string);

void createIDFvector(HashTable *hashTable, Vector *stopwords);

void initIDFinChain(SpecNode *head);

void addSpecToIDF(SpecNode *specNode);

void addWordToIDF(char *wordToBeInserted);

void computeIDFvalues(Vector *idfVector);

void addSentenceToIDF(char *sentence);

void processWordAddToIDF(char *word);

void traverseInfoListToIDF(InfoList *infoList);

void printIDFvector(Vector *idfVector);

void freeIDFinfo(Vector *idfVector);

void resetIDFSeenFlags(Vector *idfVector);

#endif