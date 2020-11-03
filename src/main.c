#include "../hdr/includes.h"


int main(int argc, char **argv)
{
  //printf("%s", hello());
  //read_from_file(DATASETX);
  //printf("%d\n", count_datafiles("./Datasets/2013_camera_specs/"));
  //read_from_dir("./Datasets/2013_camera_specs/");

  //HashBucket **hashTable = initHashTable(count_datafiles(DATASET_PATH));

  HashBucket **hashTable = initHashTable(10);

  read_from_dir(DATASET_PATH, hashTable);

  /*SpecInfo *spec = initSpecInfo("hello", "world");
  printf("%s\n", spec->specId);
  printf("%s\n", spec->pageTitle);*/
  return 0;
}
