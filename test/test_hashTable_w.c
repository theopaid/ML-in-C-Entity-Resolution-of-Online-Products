#include <stddef.h>
#include "Unity/src/unity.h"

#include "../hdr/includes.h"

void test_initHtable_w(void)
{
    HashTable_w *htable = initHashTable_w(15000);
    TEST_ASSERT_NOT_NULL(htable);
    freeHashTable_w(htable);
}
void test_initPairInfo_w(void)
{
    PairInfo_w *newPairInfo = initPairInfo_w("leftSpec", "rightSpec", 1);
    TEST_ASSERT_NULL(newPairInfo->nextPair);
    TEST_ASSERT_NOT_NULL(newPairInfo);
    freePairInfo_w(newPairInfo);
}

void test_addAndSearchChain_w()
{
    HashTable_w *hashTable = initHashTable_w(1000);
    PairInfo_w *newPairInfo = initPairInfo_w("leftSpec", "rightSpec", 1);
    addToHashTable_w(hashTable, newPairInfo);
    PairInfo_w *pairInfo = searchHashTable_w(hashTable, "leftSpec", "rightSpec");
    TEST_ASSERT_NOT_NULL(pairInfo);
    PairInfo_w *pairInfo2 = searchHashTable_w(hashTable, "leftSpecWrong", "rightSpec");
    TEST_ASSERT_NULL(pairInfo2);
    PairInfo_w *pairInfo3 = searchHashTable_w(hashTable, "rightSpec", "leftSpec");
    TEST_ASSERT_EQUAL_STRING("leftSpec", pairInfo3->leftSpecId);
    freeHashTable_w(hashTable);
}

void test_hashing_w()
{
    UnityBegin("../test/test_hashTable.c");

    RUN_TEST(test_initHtable_w);
    RUN_TEST(test_initHtable_w);
    RUN_TEST(test_initPairInfo_w);
    RUN_TEST(test_addAndSearchChain_w);

    UnityEnd();
}