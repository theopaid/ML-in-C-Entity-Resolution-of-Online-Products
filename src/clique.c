#include "../hdr/includes.h"

long matchesFound = 0;

CliqueNode *initCliqueNode()
{
    CliqueNode *newCliqueNode = (CliqueNode *)safe_malloc(sizeof(CliqueNode));
    newCliqueNode->specInfo = NULL;
    newCliqueNode->next = newCliqueNode; // we have a cyclic list
    newCliqueNode->prev = newCliqueNode;

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
        matchesFound++;
        printf("%s,%s\n", cliqueNode->specInfo->specId, cliquePtr->specInfo->specId);
        if (fptr != NULL)
            fprintf(fptr, "%s , %s\n", cliqueNode->specInfo->specId, cliquePtr->specInfo->specId);
        cliquePtr = cliquePtr->next;
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

void printMatchesCount()
{
    printf("Matches found: %ld\n", matchesFound);
}

void freeCliqueNode(CliqueNode *cliqueNode)
{
    if (cliqueNode == NULL)
        return;
    freeSpecInfo(cliqueNode->specInfo);
    free(cliqueNode);
}