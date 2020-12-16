#ifndef _ML_H
#define _ML_H

typedef struct MLInfo MLInfo;
typedef struct idfInfo idfInfo;
typedef struct tfInfo tfInfo;
typedef struct tf_idfInfo tf_idfInfo;

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
    double tfSum;
    double tf_idfValue;
};

struct tfInfo
{
    char *word;
    double tfValue;
};

struct tf_idfInfo
{
    char *word;
    double tf_idfValue;
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

void addWordToIDF(char *wordToBeInserted, Vector *tfVector);

void computeIDFvalues(Vector *idfVector);

void addSentenceToIDF(char *sentence, Vector *tfVector);

void processWordAddToIDF(char *word, Vector *tfVector);

void traverseInfoListToIDF(InfoList *infoList, Vector *tfVector);

void printIDFvector(Vector *idfVector);

void freeIDFinfo(Vector *idfVector);

void resetIDFSeenFlags(Vector *idfVector);

tfInfo *initTFinfo(char *string);

void freeTFinfo(Vector *tfVector);

void computeTFvalue(Vector *tfVector);

void addWordToTF(char *wordToBeInserted, Vector *tfVector);

void computeTF_IDFvalues(HashTable *hashTable, Vector *idfVector);

void addTFvectorToIDF(Vector *tfVector, Vector *idfVector);

tf_idfInfo *initTF_IDFinfo(char *string);

void trimNitemsFromTF_IDF(Vector *idfVector, int nItems);

void selectionSort(Vector *idfVector, int nItems);

#endif