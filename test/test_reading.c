#include <stddef.h>
#include "Unity/src/unity.h"

#include "../hdr/includes.h"

void test_count_datafiles(void) {
  TEST_ASSERT_EQUAL(2, count_datafiles("./test_files/"));
  TEST_ASSERT_EQUAL(8, count_datafiles("./src/"));
  TEST_ASSERT_EQUAL(9, count_datafiles("./hdr/"));
}

void test_reading()
{
  UnityBegin("../test/test_reading.c");

  RUN_TEST(test_count_datafiles);

  UnityEnd();
}
