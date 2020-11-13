#include <stddef.h>
#include "Unity/src/unity.h"

#include "../hdr/initial.h"

void test_hashing();
void test_clique();

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

int main(void)
{

   /*SpecInfo *test_spec = initSpecInfo("site", "test_1.json", "test_2");
   TEST_ASSERT_NOT_NULL(test_spec);
   TEST_ASSERT_EQUAL_STRING("site//test_1", test_spec->specId);
   TEST_ASSERT_EQUAL_STRING("test_2", test_spec->pageTitle);
   //unitSpecInfo(test_spec);
   freeSpecInfo(test_spec);
}*/

   test_hashing();
   test_clique();
   return 0;
}


void printExecTime()
{
}
