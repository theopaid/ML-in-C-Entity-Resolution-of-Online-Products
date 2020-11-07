#include "../hdr/includes.h"

double timeSpent;

int main(int argc, char **argv)
{
  clock_t begin = clock();
  //printf("%s", hello());
  //read_from_file(DATASETX);
  //printf("%d\n", count_datafiles("./Datasets/2013_camera_specs/"));
  //read_from_dir("./Datasets/2013_camera_specs/");

  //HashBucket **hashTable = initHashTable(count_datafiles(DATASET_PATH));

  HashTable *hashTable = initHashTable(count_datafiles(DATASET_PATH));

  read_from_dir(DATASET_PATH, hashTable);

  freeHashTable(hashTable);

  /*SpecInfo *spec = initSpecInfo("hello", "world");
  printf("%s\n", spec->specId);
  printf("%s\n", spec->pageTitle);*/
  clock_t end = clock();
  timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
  printInsights();
  return 0;

  // printf("%s", hello());
  // read_from_file(DATASETX);
  // if (!validArgs(argc, argv))
  // {
  //   printf("The arguments given are invalid. Try again.\n");
  //   return -1;
  // }

  // char *datasetX, *datasetW;
  // getArgs(&datasetX, &datasetW, argv);

  // read_from_file(datasetX);
}

void printExecTime()
{
  printf("Execution time: %f\n", timeSpent);
}
