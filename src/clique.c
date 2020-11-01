#include "../hdr/includes.h"

CliqueNode *initCliqueNode()
{
    CliqueNode *newCliqueNode = (CliqueNode *)safe_malloc(sizeof(CliqueNode));
    newCliqueNode->specInfo = NULL;
    newCliqueNode->next = newCliqueNode; // we have a cyclic list

    return newCliqueNode;
}