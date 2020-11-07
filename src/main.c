#include "../hdr/includes.h"

double timeSpent;

int main(int argc, char **argv)
{
  clock_t begin = clock();

  if (!validArgs(argc, argv))
  {
    printf("The arguments given are invalid. Try again.\n");
    return -1;
  }

  char *datasetX, *datasetW;
  getArgs(&datasetX, &datasetW, argv);
  puts(datasetW);
  puts(datasetX);

  HashTable *hashTable = initHashTable(count_datafiles(datasetX));

  read_from_dir(datasetX, hashTable);
  readDictionary(datasetW);

  freeHashTable(hashTable);

  clock_t end = clock();
  timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
  printInsights();
  return 0;
}

void printExecTime()
{
  printf("Execution time: %f\n", timeSpent);
}
