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

typedef struct Specs {
  char *spec_id;
  char *page_title;
  InfoList *info_list;
} Spec;

#endif
