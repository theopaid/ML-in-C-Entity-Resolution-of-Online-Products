#include <stddef.h>
#include "Unity/src/unity.h"

#include "../src/initial.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_hello(void)
{
   TEST_ASSERT_EQUAL_STRING("Hello, world!\n", hello());
}

void test_init(void) {
  SpecInfo *test_spec = initSpecInfo("test_1", "test_2");
  TEST_ASSERT_NOT_NULL(test_spec);
  TEST_ASSERT_EQUAL_STRING("test_1", test_spec->specId);
  TEST_ASSERT_EQUAL_STRING("test_2", test_spec->pageTitle);
  unitSpecInfo(test_spec);
}

int main(void)
{
   UnityBegin("test/test_initial.c");

   RUN_TEST(test_hello);

   UnityEnd();

   UnityBegin("test/test_spec1.0.c");

   RUN_TEST(test_init);

   UnityEnd();
   return 0;
}
