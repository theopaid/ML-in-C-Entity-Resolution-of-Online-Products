#include <stddef.h>
#include "Unity/src/unity.h"

#include "../hdr/includes.h"

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
   UnityBegin("test/test_initial.c");

   RUN_TEST(test_hello);
   //initHashTable(29000);
   //initCliqueNode();

   UnityEnd();

   return 0;
}
