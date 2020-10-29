#include "../hdr/includes.h"
#define DATASETX "../test_files/test.txt"


int main(int argc, char **argv)
{
  printf("%s", hello());
  read_from_file(DATASETX);
}
