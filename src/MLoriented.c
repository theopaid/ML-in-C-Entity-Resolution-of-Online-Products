#include "../hdr/includes.h"
int specsInitiated = 0;
double differentSpecs = 0.0;

Vector *lastMLVector;
Vector *stopWords;
Vector *idfVector;

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
    specsInitiated = 0;
}

void initMLinChain(SpecNode *head)
{
    SpecNode *specPtr = head;
    while (specPtr != NULL)
    {
        //printf("SPEC: %s\n", specPtr->cliquePtr->specInfo->specId);
        specsInitiated++;
        //printf("spec num: %d\n", specsInitiated);
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
    //printBOW(specInfo);
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

idfInfo *initIDFinfo(char *string)
{
    idfInfo *newIDFinfo = safe_malloc(sizeof(idfInfo));
    newIDFinfo->word = createString(string);
    newIDFinfo->idfValue = 0.0;
    newIDFinfo->timesSeen = 0.0;
    newIDFinfo->alreadySeenInSpecFlag = 0;

    return newIDFinfo;
}

void createIDFvector(HashTable *hashTable, Vector *stopwords)
{
    if (hashTable == NULL)
        return;
    stopWords = stopWords;
    idfVector = vectorInit();
    differentSpecs = 0;
    for (int i = 0; i < hashTable->size; i++)
    {
        if (hashTable->hashArray[i] == NULL) // bucket not allocated, has no Specs
            continue;
        initIDFinChain(hashTable->hashArray[i]->specList);
    }
    computeIDFvalues(idfVector);
    printIDFvector(idfVector);
    freeIDFinfo(idfVector);
    freeVector(idfVector);
}

void initIDFinChain(SpecNode *head)
{
    SpecNode *specPtr = head;
    while (specPtr != NULL)
    {
        //printf("SPEC: %s\n", specPtr->cliquePtr->specInfo->specId);
        specsInitiated++;
        //printf("spec num: %d\n", specsInitiated);
        addSpecToIDF(specPtr);
        specPtr = specPtr->nextSpec;
    }
}

void addSpecToIDF(SpecNode *specNode)
{
    differentSpecs++;
    resetIDFSeenFlags(idfVector);
    SpecInfo *specInfo = specNode->cliquePtr->specInfo;
    if (specInfo == NULL)
        return;
    processWordAddToIDF("page");
    processWordAddToIDF("title");
    addSentenceToIDF(specInfo->pageTitle);
    traverseInfoListToIDF(specInfo->infoList);
}

void addWordToIDF(char *wordToBeInserted)
{
    int itemsCount = vectorItemsCount(idfVector);
    for (int i = 0; i < itemsCount; i++)
    {
        char *currentWord = ((idfInfo *)vectorGet(idfVector, i))->word;
        if (same_string(currentWord, wordToBeInserted))
        {
            if (((idfInfo *)vectorGet(idfVector, i))->alreadySeenInSpecFlag == 0)
            {
                ((idfInfo *)vectorGet(idfVector, i))->timesSeen++;
                ((idfInfo *)vectorGet(idfVector, i))->alreadySeenInSpecFlag = 1;
            }
            return;
        }
    }
    // word not found, time to insert it
    idfInfo *newIDFinfo = initIDFinfo(wordToBeInserted);
    newIDFinfo->timesSeen = 1.0;
    vectorPushBack(idfVector, newIDFinfo);
}

void computeIDFvalues(Vector *idfVector)
{
    int itemsCount = vectorItemsCount(idfVector);
    for (int i = 0; i < itemsCount; i++)
    {
        ((idfInfo *)vectorGet(idfVector, i))->idfValue = log(differentSpecs / ((idfInfo *)vectorGet(idfVector, i))->timesSeen);
    }
}

void printIDFvector(Vector *idfVector)
{
    puts("====================");
    int itemsCount = vectorItemsCount(idfVector);
    for (int i = 0; i < itemsCount; i++)
    {
        printf("%s : %f || ", ((idfInfo *)vectorGet(idfVector, i))->word, ((idfInfo *)vectorGet(idfVector, i))->idfValue);
    }
    puts("====================");
}

void addSentenceToIDF(char *sentence)
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
        processWordAddToIDF(ptr);
        ptr = strtok(NULL, delim);
    }
}

void processWordAddToIDF(char *word)
{
    char *wordCopy = createString(word);
    for (int i = 0; i < strlen(wordCopy); i++)
    {
        wordCopy[i] = tolower(wordCopy[i]);
    }
    if (!isInStopwords(wordCopy))
        addWordToIDF(wordCopy);
    free(wordCopy);
}

void traverseInfoListToIDF(InfoList *infoList)
{
    InfoNode *tmpInfoNode = infoList->head;
    while (tmpInfoNode != NULL)
    {
        addSentenceToIDF(tmpInfoNode->description);
        addSentenceToIDF(tmpInfoNode->content);

        tmpInfoNode = tmpInfoNode->next;
    }
}

void freeIDFinfo(Vector *idfVector)
{
    int count = vectorItemsCount(idfVector);
    for (int i = 0; i < count; i++)
    {
        free(((idfInfo *)vectorGet(idfVector, i))->word);
    }
}

void resetIDFSeenFlags(Vector *idfVector)
{
    int itemsCount = vectorItemsCount(idfVector);
    for (int i = 0; i < itemsCount; i++)
    {
        ((idfInfo *)vectorGet(idfVector, i))->alreadySeenInSpecFlag = 0;
    }
}
