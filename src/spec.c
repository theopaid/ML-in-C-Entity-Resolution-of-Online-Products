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

void add_newInfo_toSpec(SpecInfo *spec, char *desc, char *info) {

}

void add_newInfo_toSpec_option(SpecInfo *spec, char *desc, char* option) {

}

void print_spec(SpecInfo *spec) {
  if ( spec->specId == NULL || spec->pageTitle == NULL  ) {
    printf("UNINITIALIZED SPEC\n");
    return;
  }
  printf("\tSPEC:\t%s\n\t\tTITLE:\t%s\n", spec->specId, spec->pageTitle);
}
