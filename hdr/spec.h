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

SpecInfo *initSpecInfo(char *, char *, char *);
void unitSpecInfo(SpecInfo *);
void add_newInfo_toSpec(SpecInfo *, char *, char *);
void print_spec(SpecInfo *);
void print_info_list(InfoList *list);
void add_newInfo_toList(InfoList*, char*, char*);

void add_newInfo_toNode(InfoNode *, char*, char*);
struct InfoList
{
  InfoNode *head;
};

struct InfoNode // Περιέχει την περιγραφή (description) του spec.
{               // Σε μορφή λίστας θα αποθηκεύονται.
  char *description;
  char *content; // description:content αντιστοίχιση των γραμμών του .json
  InfoNode *next;
};

void freeInfoNode(InfoNode *infoNode);

void freeInfoList(InfoList *infoList);

void freeSpecInfo(SpecInfo *specInfo);


#endif
