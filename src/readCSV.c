#include "../hdr/includes.h"

#define MAXLINE 1024

int datasetWlinesRead;
Vector *trainingPairsVector;
Vector *evaluationPairsVector;

char *getField(char *line, int fieldNum)
{
    char *tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        if (!--fieldNum)
        {
            return tok;
        }
    }
    return NULL;
}

void readDictionary(char *fileName, HashTable *hashTable)
{
    int linesCount = countCSVlines(fileName);
    int sixtyPercent = linesCount * 0.6;
    int twentyPercentMore = linesCount * 0.2 + sixtyPercent;
    int loopCount = 0;
    FILE *stream = fopen(fileName, "r");
    if (stream == NULL)
    {
        printf("Could not create %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    trainingPairsVector = vectorInit();
    evaluationPairsVector = vectorInit();

    char line[MAXLINE], *leftSpecId, *rightSpecId;
    fgets(line, MAXLINE, stream); // for the first csv line
    while (fgets(line, MAXLINE, stream))
    {
        loopCount++;
        char *tmp1 = strdup(line), *tmp2 = strdup(line), *tmp3 = strdup(line);
        int isMatching = atoi(getField(tmp1, 3));
        leftSpecId = getField(tmp2, 1);
        rightSpecId = getField(tmp3, 2);
        PairInfo *newPairInfo = initPairInfo(leftSpecId, rightSpecId, isMatching);
        if (loopCount <= sixtyPercent)
        {
            vectorPushBack(trainingPairsVector, newPairInfo);
        }
        else if (loopCount <= twentyPercentMore)
        {
            vectorPushBack(evaluationPairsVector, newPairInfo);
        }
        else
        {
        }

        if (isMatching)
        {
            updateCliques(leftSpecId, rightSpecId, hashTable);
        }
        else
        {
            updateMissMatchCliques(leftSpecId, rightSpecId, hashTable);
        }

        free(tmp1);
        free(tmp2);
        free(tmp3);
    }

    fclose(stream);
}

Vector *readCsvToVector(char *fileName)
{
    FILE *stream = fopen(fileName, "r");
    if (stream == NULL)
    {
        printf("Could not create %s\n", fileName);
        exit(EXIT_FAILURE);
    }

    Vector *vector = vectorInit();
    char line[MAXLINE];
    char *nextField = NULL;
    while (fgets(line, MAXLINE, stream))
    {
        for (nextField = strtok(line, ","); nextField != NULL; nextField = strtok(NULL, ","))
        {
            vectorPushBack(vector, createString(nextField));
        }
    }

    fclose(stream);
    return vector;
}

int countCSVlines(char *fileName)
{
    FILE *stream = fopen(fileName, "r");
    if (stream == NULL)
    {
        printf("Could not create %s\n", fileName);
        exit(EXIT_FAILURE);
    }

    int count = 0;
    char line[MAXLINE];
    fgets(line, MAXLINE, stream); // for the first csv line
    while (fgets(line, MAXLINE, stream))
    {
        count++;
    }
    fclose(stream);

    return count;
}

PairInfo *initPairInfo(char *leftSpecId, char *rightSpecId, int isMatch)
{
    PairInfo *newPairInfo = safe_malloc(sizeof(PairInfo));
    newPairInfo->leftSpecId = createString(leftSpecId);
    newPairInfo->rightSpecId = createString(rightSpecId);
    newPairInfo->isMatch = isMatch;

    return newPairInfo;
}
