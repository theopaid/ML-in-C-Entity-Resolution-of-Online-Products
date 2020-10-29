#ifndef _RDX_H
#define _RDX_H

typedef struct InfoNode {
  char *info_description;
  char *info_content;
  struct InfoNode *next;
} InfoNode;

typedef struct InfoList {
  InfoNode *head;
} InfoList;

typedef struct Specs {
  char *page_title;
  InfoList *info_list;
} Spec;

void read_from_file(char*);

#endif
