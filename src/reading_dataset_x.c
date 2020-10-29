#include "../hdr/includes.h"

void read_from_file(char *file_name) {
  FILE *dataset_x;
  char line[1000];
  dataset_x = fopen(file_name, "r");
  if ( dataset_x == NULL ) {
    perror("Unable to open dataset_x file!");
    exit(1);
  } else {
    while ( fgets(line, 1000, dataset_x) != NULL ) {
      puts(line);
    }
    fclose(dataset_x);
  }
}
