#ifndef _SPEC_H
#define _SPEC_H

typedef struct SpecInfo SpecInfo;
typedef struct InfoList InfoList;
typedef struct InfoNode InfoNode;

struct SpecInfo
{
  char *specId;
  char *pageTitle;
  InfoList *infoList;
};

SpecInfo *initSpecInfo(char *, char *);
void unitSpecInfo(SpecInfo *);

struct InfoList
{
  InfoNode *head;
};

struct InfoNode // Περιέχει την περιγραφή (descirption) του spec.
{               // Σε μορφή λίστας θα αποθηκεύονται.
  char *descirption;
  char *content; // description:content αντιστοίχιση των γραμμών του .json
  InfoNode *next;
};

void freeInfoNode(InfoNode *infoNode);

void freeInfoList(InfoList *infoList);

void freeSpecInfo(SpecInfo *specInfo);

#endif
