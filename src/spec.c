#include "../hdr/includes.h"

SpecInfo *initSpecInfo(char *id, char *title)
{
    SpecInfo *newSpec = (SpecInfo *)safe_malloc(sizeof(SpecInfo));
    newSpec->specId = (char *)safe_malloc(strlen(id) + 1);
    strcpy(newSpec->specId, id);
    newSpec->pageTitle = (char *)safe_malloc(strlen(title) + 1);
    strcpy(newSpec->pageTitle, title);
    newSpec->infoList = NULL;
    return newSpec;
}

// void unitSpecInfo(SpecInfo *spec)
// {
//     free(spec->specId);
//     free(spec->pageTitle);
//     free(spec->infoList);
//     free(spec);
// }

void freeInfoNode(InfoNode *infoNode)
{
    if (infoNode == NULL)
        return;
    free(infoNode->descirption);
    free(infoNode->content);
    freeInfoNode(infoNode->next);
    free(infoNode);
}

void freeInfoList(InfoList *infoList)
{
    if (infoList == NULL)
        return;
    freeInfoNode(infoList->head);
    free(infoList);
}

void freeSpecInfo(SpecInfo *specInfo)
{
    if (specInfo == NULL)
        return;
    free(specInfo->specId);
    free(specInfo->pageTitle);
    freeInfoList(specInfo->infoList);
    free(specInfo);
}