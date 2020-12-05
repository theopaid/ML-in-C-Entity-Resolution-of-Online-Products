#include <stddef.h>
#include "Unity/src/unity.h"

void test_spec();
void test_reading();
void test_hashing();
void test_clique();
void test_vector();

void setUp(void)
{
}

void tearDown(void)
{
}

int main(void)
{

  test_reading();
  test_spec();
  test_hashing();
  test_clique();
  test_vector();

  return 0;
}

void printExecTime()
{
}
