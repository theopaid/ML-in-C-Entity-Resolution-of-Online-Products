#ifndef _SPEC_H
#define _SPEC_H

typedef struct InfoNodes {
  char *description;
  char *content;
  struct InfoNodes *next;
} InfoNode;

typedef struct InfoList {
  InfoNode *head;
};

typedef struct SpecInfo {
  char *specId;
  char *pageTitle;
  InfoList *infoList;
};

#endif
