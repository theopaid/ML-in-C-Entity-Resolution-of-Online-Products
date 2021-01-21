#ifndef _TFIDFHASH_H
#define _TFIDFHASH_H

#define TF_IDF_SIZE 1000
#define AVG_WORDS_IN_JSON 400
#define AVG_TOTAL_WORDS 60000

//typedef struct MLInfo MLInfo;
typedef struct idfInfo idfInfo;
//typedef struct tfInfo tfInfo;
typedef struct tf_idfInfo tf_idfInfo;

struct tf_idfInfo
{
    char *word;
    double tf_idfValue;
    tf_idfInfo *next;
};

struct idfInfo
{
    char *word;
    double idfValue;
    idfInfo *next;
};

tf_idfInfo *initTFIDFinfo(char *word)
{
    tf_idfInfo *newInfo = (tf_idfInfo *)safe_malloc(sizeof(tf_idfInfo));
    newInfo->word = createString(word);
    newInfo->tf_idfValue = 0.0;
    newInfo->next = NULL;

    return newInfo;
}

idfInfo *initIDFinfo(char *word)
{
    idfInfo *newInfo = (idfInfo *)safe_malloc(sizeof(idfInfo));
    newInfo->word = createString(word);
    newInfo->idfValue = 0.0;
    newInfo->next = NULL;

    return newInfo;
}

tf_idfInfo *searchTFIDF_htable(HashTable_gen *hashTable, char *word);

tf_idfInfo *searchTFIDF_htableChain(tf_idfInfo *listHead, char *word);

idfInfo *searchIDF_htable(HashTable_gen *hashTable, char *word);

idfInfo *searchIDF_htableChain(idfInfo *listHead, char *word);

void addToTFIDF_htable(HashTable_gen *hashTable, char *word);

int insertInTFIDFchain(HashBucket_gen *bucketDst, char *word);

void addToIDF_htable(HashTable_gen *hashTable, char *word);

int insertInIDFchain(HashBucket_gen *bucketDst, char *word);

void calculateFinalIDF(HashTable_gen *hashTable);

Vector *calculateFinalTFIDFforSpec(HashTable_gen *hashTable);

void *sortTFIDFvector(Vector *tfidfVector);

void selectionSort(Vector *idfVector);

// struct idfInfo
// {
//     char *word;
//     double timesSeen;
//     int alreadySeenInSpecFlag;
//     double idfValue;
//     double tfSum;
//     double tf_idfValue;
// };

// struct tfInfo
// {
//     char *word;
//     double tfValue;
// };

#endif