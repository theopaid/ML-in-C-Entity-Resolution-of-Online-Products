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
   test_hashing();
   test_clique();
   return 0;
}

void printExecTime()
{
}
