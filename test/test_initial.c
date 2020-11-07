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

// void test_init(void)
// {
//    SpecInfo *test_spec = initSpecInfo("test_1", "test_2");
//    TEST_ASSERT_NOT_NULL(test_spec);
//    TEST_ASSERT_EQUAL_STRING("test_1", test_spec->specId);
//    TEST_ASSERT_EQUAL_STRING("test_2", test_spec->pageTitle);
//    //unitSpecInfo(test_spec);
//    freeSpecInfo(test_spec);
// }

// void test_addToHTable()
// {
//    HashTable *hashTable = initHashTable(1000);
//    char *specid = strdup("kalispera");
//    char *title = strdup("test");
//    char *specid2 = strdup("kalispera-test");
//    char *title2 = strdup("test");
//    SpecInfo *specInfo = initSpecInfo(specid, title);
//    addToHashTable(hashTable, specInfo);
//    SpecInfo *specInfo2 = initSpecInfo(specid2, title2);
//    addToHashTable(hashTable, specInfo2);
//    freeHashTable(hashTable);
//    free(specid);
//    free(title);
//    free(specid2);
//    free(title2);
// }

int main(void)
{
   UnityBegin("test/test_initial.c");

   RUN_TEST(test_hello);
   //freeHashTable(initHashTable(29000));
   //freeCliqueNode(initCliqueNode());
   //readDictionary("Datasets/sigmod_medium_labelled_dataset.csv");
   //test_addToHTable();

   UnityEnd();

   //UnityBegin("test/test_spec1.0.c");

   //RUN_TEST(test_init);

   //UnityEnd();
   return 0;
}
