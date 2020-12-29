#include "../hdr/includes.h"
int specsInitiated = 0;
double differentSpecs = 0.0;

Vector *lastMLVector;
Vector *stopWords;
Vector *idfVector;
Vector *tf_idfVector;

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
    char delim[15] = {
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
        '!',
        '?',
        '=',
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
    newIDFinfo->tfSum = 0.0;
    newIDFinfo->tf_idfValue = 0.0;
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
    //printf("idfVector size: %d\n", idfVector->itemsInserted);
    //printf("first: %s , %f , %f\n", ((idfInfo *)vectorGet(idfVector, 2000))->word, ((idfInfo *)vectorGet(idfVector, 2000))->timesSeen, ((idfInfo *)vectorGet(idfVector, 2000))->idfValue);
    computeTF_IDFvalues(hashTable, idfVector);

    freeIDFinfo(idfVector);
    freeVector(idfVector);
}

void initIDFinChain(SpecNode *head)
{
    SpecNode *specPtr = head;
    while (specPtr != NULL)
    {
        printf("SPEC: %s\n", specPtr->cliquePtr->specInfo->specId);
        specsInitiated++;
        printf("spec num: %d\n", specsInitiated);
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
    Vector *tfVector = specInfo->tfVector;
    processWordAddToIDF("page", tfVector);
    processWordAddToIDF("title", tfVector);
    addSentenceToIDF(specInfo->pageTitle, tfVector);
    traverseInfoListToIDF(specInfo->infoList, tfVector);
    computeTFvalue(tfVector);
}

void addWordToIDF(char *wordToBeInserted, Vector *tfVector)
{
    addWordToTF(wordToBeInserted, tfVector);

    int itemsCount = vectorItemsCount(idfVector);
    char *currentWord;
    for (int i = 0; i < itemsCount; i++)
    {
        currentWord = ((idfInfo *)idfVector->items[i])->word;
        if (same_string(currentWord, wordToBeInserted))
        {
            if (((idfInfo *)idfVector->items[i])->alreadySeenInSpecFlag == 0)
            {
                ((idfInfo *)idfVector->items[i])->timesSeen++;
                ((idfInfo *)idfVector->items[i])->alreadySeenInSpecFlag = 1;
            }
            return;
        }
    }
    // word not found, time to insert it
    idfInfo *newIDFinfo = initIDFinfo(wordToBeInserted);
    newIDFinfo->timesSeen = 1.0;
    vectorPushBack(idfVector, newIDFinfo);
}

void addWordToTF(char *wordToBeInserted, Vector *tfVector)
{
    int itemsCount = vectorItemsCount(tfVector);
    char *currentWord;
    for (int i = 0; i < itemsCount; i++)
    {
        currentWord = ((tfInfo *)tfVector->items[i])->word;
        if (same_string(currentWord, wordToBeInserted))
        {
            ((tfInfo *)tfVector->items[i])->tfValue++;
            return;
        }
    }

    // if word was not found in TF-vector, add it
    tfInfo *newTFinfo = initTFinfo(wordToBeInserted);
    newTFinfo->tfValue = 1.0;
    vectorPushBack(tfVector, newTFinfo);
}

void computeIDFvalues(Vector *idfVector)
{
    int itemsCount = vectorItemsCount(idfVector);
    for (int i = 0; i < itemsCount; i++)
    {
        ((idfInfo *)idfVector->items[i])->idfValue = log(differentSpecs / ((idfInfo *)idfVector->items[i])->timesSeen);
    }
}

void printIDFvector(Vector *idfVector)
{
    // puts("====================");
    // int itemsCount = vectorItemsCount(idfVector);
    // for (int i = 0; i < itemsCount; i++)
    // {
    //     printf("%s : %f || ", ((idfInfo *)idfVector->items[i])->word, ((idfInfo *)idfVector->items[i])->idfValue);
    // }
    // puts("====================");
}

void addSentenceToIDF(char *sentence, Vector *tfVector)
{
    char delim[15] = {
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
        '!',
        '?',
        '=',
    };
    char *ptr = strtok(sentence, delim);
    while (ptr != NULL)
    {
        processWordAddToIDF(ptr, tfVector);
        ptr = strtok(NULL, delim);
    }
}

void processWordAddToIDF(char *word, Vector *tfVector)
{
    char *wordCopy = createString(word);
    for (int i = 0; i < strlen(wordCopy); i++)
    {
        wordCopy[i] = tolower(wordCopy[i]);
    }
    if (!isInStopwords(wordCopy))
        addWordToIDF(wordCopy, tfVector);
    free(wordCopy);
}

void traverseInfoListToIDF(InfoList *infoList, Vector *tfVector)
{
    InfoNode *tmpInfoNode = infoList->head;
    while (tmpInfoNode != NULL)
    {
        addSentenceToIDF(tmpInfoNode->description, tfVector);
        addSentenceToIDF(tmpInfoNode->content, tfVector);

        tmpInfoNode = tmpInfoNode->next;
    }
}

void freeIDFinfo(Vector *idfVector)
{
    int count = vectorItemsCount(idfVector);
    for (int i = 0; i < count; i++)
    {
        free(((idfInfo *)idfVector->items[i])->word);
    }
}

void resetIDFSeenFlags(Vector *idfVector)
{
    int itemsCount = vectorItemsCount(idfVector);
    for (int i = 0; i < itemsCount; i++)
    {
        ((idfInfo *)idfVector->items[i])->alreadySeenInSpecFlag = 0;
    }
}

tfInfo *initTFinfo(char *string)
{
    tfInfo *newTFinfo = safe_malloc(sizeof(tfInfo));
    newTFinfo->word = createString(string);
    newTFinfo->tfValue = 0.0;

    return newTFinfo;
}

void freeTFinfo(Vector *tfVector)
{
    int count = vectorItemsCount(idfVector);
    for (int i = 0; i < count; i++)
    {
        //puts("freed in tf");
        free(((tfInfo *)tfVector->items[i])->word);
    }
}

void computeTFvalue(Vector *tfVector)
{
    int itemsCount = vectorItemsCount(tfVector);
    double currentValue = 0.0;
    for (int i = 0; i < itemsCount; i++)
    {
        currentValue = ((tfInfo *)tfVector->items[i])->tfValue;
        ((tfInfo *)tfVector->items[i])->tfValue = currentValue / itemsCount;
    }
}

void computeTF_IDFvalues(HashTable *hashTable, Vector *idfVector)
{
    SpecNode *specPtr = NULL;
    for (int i = 0; i < hashTable->size; i++)
    {
        if (hashTable->hashArray[i] == NULL) // bucket not allocated, has no Specs
            continue;
        specPtr = hashTable->hashArray[i]->specList;
        while (specPtr != NULL)
        {
            addTFvectorToIDF(specPtr->cliquePtr->specInfo->tfVector, idfVector);
            specPtr = specPtr->nextSpec;
        }
    }
    int itemsCount = vectorItemsCount(idfVector);
    for (int j = 0; j < itemsCount; j++)
    {
        ((idfInfo *)idfVector->items[j])->tf_idfValue = (((idfInfo *)idfVector->items[j])->tfSum / specsInitiated) * ((idfInfo *)idfVector->items[j])->idfValue;
    }
    trimNitemsFromTF_IDF(idfVector);

    specPtr = NULL;
    for (int i = 0; i < hashTable->size; i++)
    {
        if (hashTable->hashArray[i] == NULL) // bucket not allocated, has no Specs
            continue;
        specPtr = hashTable->hashArray[i]->specList;
        while (specPtr != NULL)
        {
            createTF_IDFforSpec(specPtr->cliquePtr->specInfo, idfVector);
            specPtr = specPtr->nextSpec;
        }
    }
    // SpecInfo *testSpecInfo = searchHashTable(hashTable, "www.ebay.com//47709")->cliquePtr->specInfo;
    // for (int i = 0; i < 3000; i++)
    // {
    //     printf(" %s : %f || ", ((tf_idfInfo *)testSpecInfo->tf_idfVectorFinal->items[i])->word, ((tf_idfInfo *)testSpecInfo->tf_idfVectorFinal->items[i])->tf_idfValue);
    // }
}

void addTFvectorToIDF(Vector *tfVector, Vector *idfVector)
{
    int tfItemsCount = vectorItemsCount(tfVector);
    int idfItemsCount = vectorItemsCount(idfVector);
    char *currentTFword = NULL, *currentIDFword = NULL;
    for (int i = 0; i < tfItemsCount; i++)
    {
        currentTFword = ((tfInfo *)tfVector->items[i])->word;
        for (int j = 0; j < idfItemsCount; j++)
        {
            currentIDFword = ((idfInfo *)idfVector->items[j])->word;
            if (same_string(currentTFword, currentIDFword))
            {
                ((idfInfo *)idfVector->items[j])->tfSum += ((tfInfo *)tfVector->items[i])->tfValue;
                break;
            }
        }
    }
}

tf_idfInfo *initTF_IDFinfo(char *string)
{
    tf_idfInfo *newTF_IDFinfo = safe_malloc(sizeof(tf_idfInfo));
    newTF_IDFinfo->word = createString(string);
    newTF_IDFinfo->tf_idfValue = 0.0;

    return newTF_IDFinfo;
}

void trimNitemsFromTF_IDF(Vector *idfVector)
{
    selectionSort(idfVector);
}

void selectionSort(Vector *idfVector)
{
    int i, j, min_idx;
    int itemsCount = vectorItemsCount(idfVector);

    // One by one move boundary of unsorted subarray
    for (i = 0; i < itemsCount - 1; i++)
    {
        // Find the maximum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < itemsCount; j++)
            if (((idfInfo *)idfVector->items[j])->tf_idfValue > ((idfInfo *)idfVector->items[min_idx])->tf_idfValue)
                min_idx = j;

        // Swap the found maximum element with the first element
        idfInfo *idfInfoToSwap = (idfInfo *)idfVector->items[i];
        idfVector->items[i] = (idfInfo *)idfVector->items[min_idx];
        idfVector->items[min_idx] = idfInfoToSwap;
    }
}

void createTF_IDFforSpec(SpecInfo *specInfo, Vector *idfVector)
{
    // copy n best tf_idf value words to the spec's tf_idf new vector
    int n = TF_IDF_SIZE;
    char *currentWord = NULL;
    for (int i = 0; i < n; i++)
    {
        currentWord = ((idfInfo *)idfVector->items[i])->word;
        tf_idfInfo *newTF_IDFinfo = initTF_IDFinfo(currentWord);
        vectorPushBack(specInfo->tf_idfVectorFinal, newTF_IDFinfo);
    }

    // check if spec contains these words, or else input 0 tf_idf value
    int tfVectorItemsCount = vectorItemsCount(specInfo->tfVector);
    char *currentTF_IDFword = NULL;
    char *currentTFword = NULL;
    int flag = 0;
    for (int i = 0; i < n; i++)
    {
        flag = 0;
        currentTF_IDFword = ((tf_idfInfo *)specInfo->tf_idfVectorFinal->items[i])->word;
        for (int j = 0; j < tfVectorItemsCount; j++)
        {
            currentTFword = ((tfInfo *)specInfo->tfVector->items[j])->word;
            if (same_string(currentTF_IDFword, currentTFword))
            {
                double tf_value = ((tfInfo *)specInfo->tfVector->items[j])->tfValue;
                double idf_value = ((idfInfo *)idfVector->items[i])->idfValue;
                ((tf_idfInfo *)specInfo->tf_idfVectorFinal->items[i])->tf_idfValue = tf_value * idf_value;
                flag = 1;
                break;
            }
        }
        if (flag == 1)
            continue;
        // word is not contained in spec
        ((tf_idfInfo *)specInfo->tf_idfVectorFinal->items[i])->tf_idfValue = 0.0;
    }
}
