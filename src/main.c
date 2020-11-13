#include "../hdr/includes.h"

int main(int argc, char **argv)
{

  //printf("%s", hello());
  //read_from_file(DATASETX);
  //printf("%d\n", count_datafiles("./Datasets/2013_camera_specs/"));
  //read_from_dir("./Datasets/2013_camera_specs/");

  //HashBucket **hashTable = initHashTable(count_datafiles(DATASET_PATH));

  HashBucket **hashTable = initHashTable(count_datafiles(DATASET_PATH));

  read_from_dir(DATASET_PATH, hashTable);

  print_cliques(hashTable);

  /*SpecInfo *spec = initSpecInfo("hello", "world");
  printf("%s\n", spec->specId);
  printf("%s\n", spec->pageTitle);*/
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
