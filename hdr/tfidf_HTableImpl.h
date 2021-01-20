#ifndef _TFIDFHASH_H
#define _TFIDFHASH_H

#define TF_IDF_SIZE 3000

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

#endif