#include "../hdr/includes.h"

Vector *lastMLVector;
Vector *stopWords;

MLInfo *initMLinfo(char *string)
{
    MLInfo *newMLinfo = safe_malloc(sizeof(MLInfo));
    newMLinfo->word = createString(string);
    newMLinfo->bow = 0;
    newMLinfo->tf_idf = 0.0;
    newMLinfo->rog_reg = 0.0;

    return newMLinfo;
}

void initML(HashTable *hashTable, Vector *stopWords)
{
    if (hashTable == NULL)
        return;
    stopWords = stopWords;
    lastMLVector = NULL;
    for (int i = 0; i < hashTable->size; i++)
    {
        if (hashTable->hashArray[i] == NULL) // bucket not allocated, has no Specs
            continue;
        initMLinChain(hashTable->hashArray[i]->specList);
    }
    lastMLVector = NULL;
}

void initMLinChain(SpecNode *head)
{
    SpecNode *specPtr = head;
    while (specPtr != NULL)
    {
        initMLforSpec(specPtr);
        specPtr = specPtr->nextSpec;
    }
}

void initMLforSpec(SpecNode *specNode)
{
    SpecInfo *specInfo = specNode->cliquePtr->specInfo;
    if (specInfo == NULL)
        return;
    //printf("nai: %s\n", specInfo->specId);
    if (lastMLVector != NULL)
    {
        //printf("last vector last word: %s\n", ((MLInfo *)lastMLVector->items[vectorItemsCount(lastMLVector) - 1])->word);
        //puts(" ");
        //printf("specId: %s\n", specInfo->specId);
        copyToEmptyVector(specInfo->vectorMLinfo, lastMLVector);
    }
    populateBOW(specInfo);
}

void populateBOW(SpecInfo *specInfo)
{
    processWordAddToBow(specInfo->vectorMLinfo, "page");
    processWordAddToBow(specInfo->vectorMLinfo, "title");
    extractWordAndProcess(specInfo->vectorMLinfo, specInfo->pageTitle);
    traverseInfoList(specInfo->vectorMLinfo, specInfo->infoList);

    lastMLVector = specInfo->vectorMLinfo;
    puts("-----------------------------------");
    printBOW(specInfo);
    puts("-----------------------------------");
}

void extractWordAndProcess(Vector *vectorMLinfo, char *sentence)
{
    char delim[12] = {
        ' ',
        ',',
        '.',
        ':',
        '\n',
        '&',
        '\\',
        '|',
        '/',
        '-',
        '(',
        ')',
    };
    char *ptr = strtok(sentence, delim);
    while (ptr != NULL)
    {
        processWordAddToBow(vectorMLinfo, ptr);
        ptr = strtok(NULL, delim);
    }
}

void traverseInfoList(Vector *vectorMLinfo, InfoList *infoList)
{
    InfoNode *tmpInfoNode = infoList->head;
    while (tmpInfoNode != NULL)
    {
        extractWordAndProcess(vectorMLinfo, tmpInfoNode->description);
        extractWordAndProcess(vectorMLinfo, tmpInfoNode->content);

        tmpInfoNode = tmpInfoNode->next;
    }
}

void processWordAddToBow(Vector *vectorMLinfo, char *word)
{
    char *wordCopy = createString(word);
    for (int i = 0; i < strlen(wordCopy); i++)
    {
        wordCopy[i] = tolower(wordCopy[i]);
    }
    if (!isInStopwords(wordCopy))
        addWordToBOW(vectorMLinfo, wordCopy);
    free(wordCopy);
}

int isInStopwords(char *word)
{
    int stopWordsCount = vectorItemsCount(stopWords);
    for (int i = 0; i < stopWordsCount; i++)
    {
        if (same_string(word, stopWords->items[i]))
            return 1;
    }
    return 0;
}

void addWordToBOW(Vector *vectorMLinfo, char *word)
{
    //printf("Adding: %s\n", word);
    if (incrementIfAlreadyInBOW(vectorMLinfo, word))
    {
        return;
    }

    MLInfo *newMLinfo = initMLinfo(word);
    newMLinfo->bow = 1;
    vectorPushBack(vectorMLinfo, newMLinfo);
}

int incrementIfAlreadyInBOW(Vector *vectorMLinfo, char *word)
{
    int vectorItemsCnt = vectorItemsCount(vectorMLinfo);
    for (int i = 0; i < vectorItemsCnt; i++)
    {
        if (same_string(word, ((MLInfo *)vectorMLinfo->items[i])->word))
        {
            ((MLInfo *)vectorMLinfo->items[i])->bow++;
            //printf("found again for: %s , %s\n", ((MLInfo *)vectorMLinfo->items[i])->word, word);
            //printf("count now: %d\n", ((MLInfo *)vectorMLinfo->items[i])->bow);
            return 1;
        }
    }
    //printf("not found: %s\n", word);
    return 0;
}

void copyToEmptyVector(Vector *dstVector, Vector *srcVector)
{
    if (dstVector == NULL || srcVector == NULL || vectorItemsCount(dstVector) != 0)
    {
        return;
    }
    int srcItemsCount = vectorItemsCount(srcVector);
    for (int i = 0; i < srcItemsCount; i++)
    {
        vectorPushBack(dstVector, initMLinfo(((MLInfo *)srcVector->items[i])->word));
    }
}

void printBOW(SpecInfo *specInfo)
{
    Vector *vectorML = specInfo->vectorMLinfo;
    int count = vectorItemsCount(vectorML);
    printf("specId: %s\n", specInfo->specId);
    for (int i = 0; i < count; i++)
    {
        printf("%s : %d | ", ((MLInfo *)vectorML->items[i])->word, ((MLInfo *)vectorML->items[i])->bow);
    }
}

void freeMLinfo(Vector *vectorMLinfo)
{
    int count = vectorItemsCount(vectorMLinfo);
    for (int i = 0; i < count; i++)
    {
        free(((MLInfo *)vectorMLinfo->items[i])->word);
    }
}