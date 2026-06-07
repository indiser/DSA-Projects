#include <gtest/gtest.h>
#include "cache/LFUPolicy.h"

class LFUPolicyTest : public ::testing::Test {
protected:
    LFUPolicy* lfu;
    
    void SetUp() override {
        lfu = new LFUPolicy(3);
    }
    
    void TearDown() override {
        delete lfu;
    }
};

TEST_F(LFUPolicyTest, BasicInsertAndAccess) {
    lfu->onInsert("key1");
    lfu->onInsert("key2");
    lfu->onInsert("key3");
    
    EXPECT_NO_THROW(lfu->onAccess("key1"));
    EXPECT_NO_THROW(lfu->onAccess("key2"));
}

TEST_F(LFUPolicyTest, EvictLeastFrequentlyUsed) {
    lfu->onInsert("key1");
    lfu->onInsert("key2");
    lfu->onInsert("key3");
    
    // All have frequency 1, evict first inserted (LRU tie-breaking)
    std::string evicted = lfu->evict();
    EXPECT_EQ(evicted, "key1");
}

TEST_F(LFUPolicyTest, FrequencyTracking) {
    lfu->onInsert("key1");
    lfu->onInsert("key2");
    lfu->onInsert("key3");
    
    // Increase frequency of key1
    lfu->onAccess("key1");
    lfu->onAccess("key1");
    
    // key2 or key3 should be evicted (both freq=1)
    std::string evicted = lfu->evict();
    EXPECT_TRUE(evicted == "key2" || evicted == "key3");
}

TEST_F(LFUPolicyTest, HighFrequencyNotEvicted) {
    lfu->onInsert("key1");
    lfu->onInsert("key2");
    lfu->onInsert("key3");
    
    // key1 accessed multiple times
    for (int i = 0; i < 10; i++) {
        lfu->onAccess("key1");
    }
    
    // key2 or key3 should be evicted, not key1
    std::string evicted = lfu->evict();
    EXPECT_NE(evicted, "key1");
}

TEST_F(LFUPolicyTest, RemoveKey) {
    lfu->onInsert("key1");
    lfu->onInsert("key2");
    lfu->onInsert("key3");
    
    lfu->remove("key2");
    
    std::string evicted = lfu->evict();
    EXPECT_TRUE(evicted == "key1" || evicted == "key3");
    EXPECT_NE(evicted, "key2");
}

TEST_F(LFUPolicyTest, TieBreakingLRU) {
    lfu->onInsert("key1");
    lfu->onInsert("key2");
    lfu->onInsert("key3");
    
    // All have same frequency, should evict oldest (key1)
    std::string evicted = lfu->evict();
    EXPECT_EQ(evicted, "key1");
}

TEST_F(LFUPolicyTest, EmptyEviction) {
    std::string evicted = lfu->evict();
    EXPECT_TRUE(evicted.empty());
}

TEST_F(LFUPolicyTest, SingleElement) {
    lfu->onInsert("key1");
    
    std::string evicted = lfu->evict();
    EXPECT_EQ(evicted, "key1");
}

TEST_F(LFUPolicyTest, FrequencyIncrement) {
    lfu->onInsert("key1");
    lfu->onInsert("key2");
    
    lfu->onAccess("key2");
    lfu->onAccess("key2");
    lfu->onAccess("key2");
    
    // key1 has freq=1, key2 has freq=4
    std::string evicted = lfu->evict();
    EXPECT_EQ(evicted, "key1");
}

TEST_F(LFUPolicyTest, MixedFrequencies) {
    lfu->onInsert("A");
    lfu->onInsert("B");
    lfu->onInsert("C");
    
    lfu->onAccess("A");  // freq=2
    lfu->onAccess("B");  // freq=2
    lfu->onAccess("B");  // freq=3
    
    // C has freq=1, should be evicted
    std::string evicted = lfu->evict();
    EXPECT_EQ(evicted, "C");
}

TEST_F(LFUPolicyTest, RemoveNonExistent) {
    lfu->onInsert("key1");
    
    EXPECT_NO_THROW(lfu->remove("key2"));
}

TEST_F(LFUPolicyTest, AccessAfterRemove) {
    lfu->onInsert("key1");
    lfu->remove("key1");
    
    EXPECT_NO_THROW(lfu->onAccess("key1"));
}

TEST_F(LFUPolicyTest, CapacityOne) {
    LFUPolicy small_lfu(1);
    
    small_lfu.onInsert("key1");
    std::string evicted = small_lfu.evict();
    EXPECT_EQ(evicted, "key1");
}
