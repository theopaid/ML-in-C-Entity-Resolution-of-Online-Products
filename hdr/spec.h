#ifndef _SPEC_H
#define _SPEC_H

typedef struct SpecInfo SpecInfo;
typedef struct InfoList InfoList;
typedef struct InfoNode InfoNode;
typedef struct MoreOptionsList MoreList;
typedef struct MoreOptionsNode MoreNode;

struct SpecInfo
{
  char *specId;
  char *pageTitle;
  InfoList *infoList;
};

SpecInfo* initSpecInfo(char *, char*);
void unitSpecInfo(SpecInfo *);
void add_newInfo_toSpec(SpecInfo *, char *, char *);
void add_newInfo_toSpec_option(SpecInfo *, char *, char *);
void print_spec(SpecInfo *);

struct InfoList
{
  InfoNode *head;
};

struct InfoNode // Περιέχει την περιγραφή (description) του spec.
{               // Σε μορφή λίστας θα αποθηκεύονται.
  char *description;
  MoreList *contentList;
  InfoNode *next;
};

struct MoreList { // List since some items have more than one options
  int options;
  MoreNode *head;
};

struct MoreNode {
  char *content; // description:content αντιστοίχιση των γραμμών του .json
  MoreNode *next;
};


#endif
