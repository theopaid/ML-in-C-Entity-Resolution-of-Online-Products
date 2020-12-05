#include "../hdr/includes.h"

long matchesFound = 0;
long missMatchesFound = 0;

CliqueNode *initCliqueNode()
{
    CliqueNode *newCliqueNode = (CliqueNode *)safe_malloc(sizeof(CliqueNode));
    newCliqueNode->specInfo = NULL;
    newCliqueNode->next = newCliqueNode; // we have a cyclic list
    newCliqueNode->prev = newCliqueNode;
    newCliqueNode->missMatchList = NULL;
    newCliqueNode->isPrinted = 0;
    newCliqueNode->hasPrintedMissMatches = 0;

    return newCliqueNode;
}

void updateCliques(char *leftSpecId, char *rightSpecId, HashTable *hashTable)
{
    CliqueNode *leftClique = getCliqueNode(leftSpecId, hashTable);
    CliqueNode *rightClique = getCliqueNode(rightSpecId, hashTable);
    if (leftClique == NULL || rightClique == NULL)
        return;
    makeCliqueAdjustmnets(leftClique, rightClique);
}

CliqueNode *getCliqueNode(char *specId, HashTable *hashTable)
{
    SpecNode *mySpecNode = searchHashTable(hashTable, specId);
    if (mySpecNode == NULL)
        return NULL;
    return mySpecNode->cliquePtr;
}

void makeCliqueAdjustmnets(CliqueNode *leftClique, CliqueNode *rightClique)
{
    if (leftClique == NULL || rightClique == NULL)
        return;
    if (alreadyInSameClique(leftClique, rightClique) == 1)
    {
        return;
    }
    rightClique->prev->next = leftClique->next;
    leftClique->next->prev = rightClique->prev;
    leftClique->next = rightClique;
    rightClique->prev = leftClique;
}

void updateMissMatchCliques(char *leftSpecId, char *rightSpecId, HashTable *hashTable)
{
    CliqueNode *leftClique = getCliqueNode(leftSpecId, hashTable);
    CliqueNode *rightClique = getCliqueNode(rightSpecId, hashTable);
    if (leftClique == NULL || rightClique == NULL)
        return;
    updateMissMatchList(leftClique, rightClique);
    updateMissMatchList(rightClique, leftClique);
}

void updateMissMatchList(CliqueNode *srcClique, CliqueNode *missMatchClique)
{
    MissMatchNode *tmpNode = srcClique->missMatchList;
    if (tmpNode == NULL)
    {
        MissMatchNode *newMissMatchNode = createMissMatchNode(missMatchClique);
        srcClique->missMatchList = newMissMatchNode;
        return;
    }
    while (tmpNode->next != NULL)
    {
        if (strcmp(tmpNode->cliqueNode->specInfo->specId, missMatchClique->specInfo->specId) == 0)
        // Check if miss match already exists for clique node
        {
            return;
        }
        tmpNode = tmpNode->next;
    }
    if (strcmp(tmpNode->cliqueNode->specInfo->specId, missMatchClique->specInfo->specId) == 0)
    {
        return;
    }
    MissMatchNode *newMissMatchNode = createMissMatchNode(missMatchClique);
    tmpNode->next = newMissMatchNode;
}

MissMatchNode *createMissMatchNode(CliqueNode *missMatchClique)
{
    MissMatchNode *newMissMatchNode = safe_malloc(sizeof(MissMatchNode));
    newMissMatchNode->cliqueNode = missMatchClique;
    newMissMatchNode->next = NULL;
    return newMissMatchNode;
}

void removeFromClique(CliqueNode *cliqueNode)
{
    cliqueNode->prev->next = cliqueNode->next;
    cliqueNode->next->prev = cliqueNode->prev;
    cliqueNode->next = cliqueNode;
    cliqueNode->prev = cliqueNode;
}

void printSpecMatches(SpecNode *specNode, FILE *fptr)
{
    CliqueNode *cliqueNode = specNode->cliquePtr;
    CliqueNode *cliquePtr = cliqueNode->next;
    while (strcmp(cliquePtr->specInfo->specId, cliqueNode->specInfo->specId) != 0)
    {
        if (cliquePtr->isPrinted == 1)
        {
            cliquePtr = cliquePtr->next;
            continue;
        }
        matchesFound++;
        printf("%s , %s\n", cliqueNode->specInfo->specId, cliquePtr->specInfo->specId);
        if (fptr != NULL)
            fprintf(fptr, "%s , %s\n", cliqueNode->specInfo->specId, cliquePtr->specInfo->specId);
        cliquePtr = cliquePtr->next;
    }
}

void printSpecMissMatches(SpecNode *specNode, FILE *fptr)
{
    CliqueNode *cliquePtr = specNode->cliquePtr;
    MissMatchNode *tmpNode = cliquePtr->missMatchList;
    while (tmpNode != NULL)
    {
        if (tmpNode->cliqueNode->hasPrintedMissMatches == 1)
        {
            tmpNode = tmpNode->next;
            continue;
        }
        missMatchesFound++;
        printf("%s,%s\n", cliquePtr->specInfo->specId, tmpNode->cliqueNode->specInfo->specId);
        if (fptr != NULL)
            fprintf(fptr, "%s , %s\n", cliquePtr->specInfo->specId, tmpNode->cliqueNode->specInfo->specId);
        tmpNode = tmpNode->next;
    }
}

int alreadyInSameClique(CliqueNode *leftCliqueNode, CliqueNode *rightCliqueNode)
{
    if (leftCliqueNode == NULL || rightCliqueNode == NULL)
        return 0;
    CliqueNode *cliquePtr = leftCliqueNode->next;
    while (strcmp(cliquePtr->specInfo->specId, leftCliqueNode->specInfo->specId) != 0)
    {
        if (strcmp(cliquePtr->specInfo->specId, rightCliqueNode->specInfo->specId) == 0)
        {
            return 1;
        }
        cliquePtr = cliquePtr->next;
    }

    return 0;
}

void resetAllPrintedStatus(HashTable *hashTable)
{
    if (hashTable == NULL)
        return;
    for (int i = 0; i < hashTable->size; i++)
    {
        if (hashTable->hashArray[i] == NULL) // bucket not allocated, has no Specs
            continue;
        resetAllPrintedStatusInChain(hashTable->hashArray[i]->specList);
    }
}

void resetAllPrintedStatusInChain(SpecNode *head)
{
    SpecNode *specPtr = head;
    while (specPtr != NULL)
    {
        specPtr->cliquePtr->isPrinted = 0;
        specPtr->cliquePtr->hasPrintedMissMatches = 0;
        specPtr = specPtr->nextSpec;
    }
}

void printMatchesCount()
{
    printf("Matches found: %ld\n", matchesFound);
}

void printMissMatchesCount()
{
    printf("Miss-Matches found: %ld\n", missMatchesFound);
}

void freeCliqueNode(CliqueNode *cliqueNode)
{
    if (cliqueNode == NULL)
        return;
    freeSpecInfo(cliqueNode->specInfo);
    freeMissMatchNode(cliqueNode->missMatchList);
    free(cliqueNode);
}

void freeMissMatchNode(MissMatchNode *missMatchNode)
{
    if (missMatchNode == NULL)
        return;
    freeMissMatchNode(missMatchNode->next);
    free(missMatchNode);
}
