#include <gtest/gtest.h>
#include "cache/LRUPolicy.h"

class LRUPolicyTest : public ::testing::Test {
protected:
    LRUPolicy* lru;
    
    void SetUp() override {
        lru = new LRUPolicy(3);
    }
    
    void TearDown() override {
        delete lru;
    }
};

TEST_F(LRUPolicyTest, BasicInsertAndAccess) {
    lru->onInsert("key1");
    lru->onInsert("key2");
    lru->onInsert("key3");
    
    EXPECT_NO_THROW(lru->onAccess("key1"));
    EXPECT_NO_THROW(lru->onAccess("key2"));
}

TEST_F(LRUPolicyTest, EvictLeastRecentlyUsed) {
    lru->onInsert("key1");
    lru->onInsert("key2");
    lru->onInsert("key3");
    
    // key1 is least recently used
    std::string evicted = lru->evict();
    EXPECT_EQ(evicted, "key1");
}

TEST_F(LRUPolicyTest, AccessUpdatesRecency) {
    lru->onInsert("key1");
    lru->onInsert("key2");
    lru->onInsert("key3");
    
    // Access key1, making it most recent
    lru->onAccess("key1");
    
    // Now key2 should be evicted
    std::string evicted = lru->evict();
    EXPECT_EQ(evicted, "key2");
}

TEST_F(LRUPolicyTest, RemoveKey) {
    lru->onInsert("key1");
    lru->onInsert("key2");
    lru->onInsert("key3");
    
    lru->remove("key2");
    
    // key1 should be evicted (key2 was removed)
    std::string evicted = lru->evict();
    EXPECT_EQ(evicted, "key1");
}

TEST_F(LRUPolicyTest, EvictWhenFull) {
    lru->onInsert("key1");
    lru->onInsert("key2");
    lru->onInsert("key3");
    
    // Cache is full, evict before inserting
    std::string evicted = lru->evict();
    EXPECT_EQ(evicted, "key1");
    
    lru->onInsert("key4");
    
    // Now key2 is least recent
    evicted = lru->evict();
    EXPECT_EQ(evicted, "key2");
}

TEST_F(LRUPolicyTest, MultipleAccesses) {
    lru->onInsert("key1");
    lru->onInsert("key2");
    lru->onInsert("key3");
    
    lru->onAccess("key1");
    lru->onAccess("key1");
    lru->onAccess("key2");
    
    // key3 is least recent
    std::string evicted = lru->evict();
    EXPECT_EQ(evicted, "key3");
}

TEST_F(LRUPolicyTest, EmptyEviction) {
    // Evicting from empty cache should return empty string
    std::string evicted = lru->evict();
    EXPECT_TRUE(evicted.empty());
}

TEST_F(LRUPolicyTest, SingleElement) {
    lru->onInsert("key1");
    
    std::string evicted = lru->evict();
    EXPECT_EQ(evicted, "key1");
}

TEST_F(LRUPolicyTest, RemoveNonExistent) {
    lru->onInsert("key1");
    
    EXPECT_NO_THROW(lru->remove("key2"));
}

TEST_F(LRUPolicyTest, AccessAfterRemove) {
    lru->onInsert("key1");
    lru->onInsert("key2");
    
    lru->remove("key1");
    
    // Accessing removed key should not crash
    EXPECT_NO_THROW(lru->onAccess("key1"));
}

TEST_F(LRUPolicyTest, InsertSameKeyTwice) {
    lru->onInsert("key1");
    lru->onInsert("key1");
    
    // Should handle duplicate inserts gracefully
    std::string evicted = lru->evict();
    EXPECT_EQ(evicted, "key1");
}

TEST_F(LRUPolicyTest, ComplexAccessPattern) {
    lru->onInsert("A");
    lru->onInsert("B");
    lru->onInsert("C");
    
    lru->onAccess("A");  // A is most recent
    lru->onAccess("B");  // B is most recent
    // C is least recent
    
    std::string evicted = lru->evict();
    EXPECT_EQ(evicted, "C");
    
    lru->onInsert("D");
    lru->onAccess("A");
    
    // B is least recent now
    evicted = lru->evict();
    EXPECT_EQ(evicted, "B");
}

TEST_F(LRUPolicyTest, CapacityOne) {
    LRUPolicy small_lru(1);
    
    small_lru.onInsert("key1");
    std::string evicted = small_lru.evict();
    EXPECT_EQ(evicted, "key1");
}

TEST_F(LRUPolicyTest, LargeCapacity) {
    LRUPolicy large_lru(100);
    
    for (int i = 0; i < 100; i++) {
        large_lru.onInsert("key" + std::to_string(i));
    }
    
    // First inserted should be first evicted
    std::string evicted = large_lru.evict();
    EXPECT_EQ(evicted, "key0");
}

TEST_F(LRUPolicyTest, AlternatingAccessPattern) {
    lru->onInsert("key1");
    lru->onInsert("key2");
    lru->onInsert("key3");
    
    lru->onAccess("key1");
    lru->onAccess("key3");
    lru->onAccess("key1");
    
    // key2 never accessed, should be evicted
    std::string evicted = lru->evict();
    EXPECT_EQ(evicted, "key2");
}
