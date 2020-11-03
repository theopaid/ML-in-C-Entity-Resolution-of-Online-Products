#include "../hdr/includes.h"

SpecInfo* initSpecInfo(char *id, char *title) {
  SpecInfo *newSpec = (SpecInfo*)safe_malloc(sizeof(SpecInfo));
  newSpec->specId = (char*)safe_malloc(strlen(id)+1);
  strcpy(newSpec->specId, id);
  newSpec->pageTitle = (char*)safe_malloc(strlen(title)+1);
  strcpy(newSpec->pageTitle, title);
  newSpec->infoList = NULL;
  return newSpec;
}

void unitSpecInfo(SpecInfo *spec) {
  free(spec->specId);
  free(spec->pageTitle);
  free(spec->infoList);
  free(spec);
}
