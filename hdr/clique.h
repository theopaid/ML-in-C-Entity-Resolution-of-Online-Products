#ifndef _CLIQUE_H
#define _CLIQUE_H

#include "../hdr/hash.h"

typedef struct HashTable HashTable;
typedef struct SpecNode SpecNode;

typedef struct CliqueNode CliqueNode;
typedef struct MissMatchNode MissMatchNode;

struct CliqueNode // Θα υλοποιηθεί κυκλική λίστα. Αρχικά, το next θα δείχνει στον εαυτό του λόγω κυκλικότητας.
{                 // Όταν δημιουργούνται ζευγάρια, θα ενωποιούνται αυτά, κάνοντας update τα *next, *prev.
    SpecInfo *specInfo;
    CliqueNode *next;
    CliqueNode *prev;
    MissMatchNode *missMatchList;
    int isPrinted;
    int hasPrintedMissMatches;
};

struct MissMatchNode
{
    CliqueNode *cliqueNode;
    MissMatchNode *next;
};

CliqueNode *initCliqueNode();

void updateCliques(char *leftSpecId, char *rightSpecId, HashTable *hashTable);

CliqueNode *getCliqueNode(char *specId, HashTable *hashTable);

void makeCliqueAdjustmnets(CliqueNode *leftClique, CliqueNode *rightClique);

void printSpecMatches(SpecNode *specNode, FILE *fptr);

void printSpecMissMatches(SpecNode *specNode, FILE *fptr);

void removeFromClique(CliqueNode *cliqueNode);

int alreadyInSameClique(CliqueNode *leftCliqueNode, CliqueNode *rightCliqueNode);

void printMatchesCount();

void printMissMatchesCount();

void resetAllPrintedStatus(HashTable *hashTable);

void resetAllPrintedStatusInChain(SpecNode *head);

void freeCliqueNode(CliqueNode *cliqueNode);

void updateMissMatchCliques(char *leftSpecId, char *rightSpecId, HashTable *hashTable);

void updateMissMatchList(CliqueNode *srcClique, CliqueNode *missMatchClique);

MissMatchNode *createMissMatchNode(CliqueNode *missMatchClique);

void freeMissMatchNode(MissMatchNode *missMatchNode);

#endif