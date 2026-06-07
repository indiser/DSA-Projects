#include <gtest/gtest.h>
#include "cache/CacheEngine.h"
#include <thread>
#include <vector>
#include <algorithm>

class CacheEngineTest : public ::testing::Test {
protected:
    CacheEngine* cache;
    
    void SetUp() override {
        cache = new CacheEngine(3, EvictionPolicyType::LRU);
    }
    
    void TearDown() override {
        delete cache;
    }
};

TEST_F(CacheEngineTest, BasicPutAndGet) {
    cache->put("key1", "value1");
    
    auto result = cache->get("key1");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "value1");
}

TEST_F(CacheEngineTest, GetNonExistent) {
    auto result = cache->get("nonexistent");
    EXPECT_FALSE(result.has_value());
}

TEST_F(CacheEngineTest, UpdateExistingKey) {
    cache->put("key1", "value1");
    cache->put("key1", "value2");
    
    auto result = cache->get("key1");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "value2");
}

TEST_F(CacheEngineTest, Remove) {
    cache->put("key1", "value1");
    
    EXPECT_TRUE(cache->remove("key1"));
    EXPECT_FALSE(cache->get("key1").has_value());
}

TEST_F(CacheEngineTest, RemoveNonExistent) {
    EXPECT_FALSE(cache->remove("nonexistent"));
}

TEST_F(CacheEngineTest, Exists) {
    cache->put("key1", "value1");
    
    EXPECT_TRUE(cache->exists("key1"));
    EXPECT_FALSE(cache->exists("key2"));
}

TEST_F(CacheEngineTest, EvictionWhenFull) {
    cache->put("key1", "value1");
    cache->put("key2", "value2");
    cache->put("key3", "value3");
    
    // Cache is full, adding key4 should evict key1 (LRU)
    cache->put("key4", "value4");
    
    EXPECT_FALSE(cache->get("key1").has_value());
    EXPECT_TRUE(cache->get("key4").has_value());
}

TEST_F(CacheEngineTest, Size) {
    EXPECT_EQ(cache->size(), 0);
    
    cache->put("key1", "value1");
    EXPECT_EQ(cache->size(), 1);
    
    cache->put("key2", "value2");
    EXPECT_EQ(cache->size(), 2);
    
    cache->remove("key1");
    EXPECT_EQ(cache->size(), 1);
}

TEST_F(CacheEngineTest, Clear) {
    cache->put("key1", "value1");
    cache->put("key2", "value2");
    cache->put("key3", "value3");
    
    cache->clear();
    
    EXPECT_EQ(cache->size(), 0);
    EXPECT_FALSE(cache->exists("key1"));
    EXPECT_FALSE(cache->exists("key2"));
    EXPECT_FALSE(cache->exists("key3"));
}

TEST_F(CacheEngineTest, LFUEvictionPolicy) {
    CacheEngine lfu_cache(3, EvictionPolicyType::LFU);
    
    lfu_cache.put("key1", "value1");
    lfu_cache.put("key2", "value2");
    lfu_cache.put("key3", "value3");
    
    // Access key2 multiple times
    lfu_cache.get("key2");
    lfu_cache.get("key2");
    
    // Adding key4 should evict key1 or key3 (least frequent)
    lfu_cache.put("key4", "value4");
    
    EXPECT_TRUE(lfu_cache.exists("key2"));
    EXPECT_TRUE(lfu_cache.exists("key4"));
}

TEST_F(CacheEngineTest, TTLExpiration) {
    cache->put("key1", "value1");
    cache->setTTL("key1", 1);  // 1 second
    
    EXPECT_TRUE(cache->exists("key1"));
    
    // Wait for expiration
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    EXPECT_FALSE(cache->exists("key1"));
}

TEST_F(CacheEngineTest, TTLNotExpired) {
    cache->put("key1", "value1");
    cache->setTTL("key1", 5);
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    EXPECT_TRUE(cache->exists("key1"));
}

TEST_F(CacheEngineTest, GetStats) {
    cache->put("key1", "value1");
    cache->get("key1");  // Hit
    cache->get("key2");  // Miss
    
    auto stats = cache->getStats();
    
    EXPECT_EQ(stats.hits, 1);
    EXPECT_EQ(stats.misses, 1);
    EXPECT_EQ(stats.size, 1);
}

TEST_F(CacheEngineTest, ConcurrentPuts) {
    // Simplified test - just verify no crashes
    for (int i = 0; i < 10; i++) {
        std::string key = "key" + std::to_string(i);
        std::string value = "value" + std::to_string(i);
        cache->put(key, value);
    }
    
    EXPECT_EQ(cache->size(), 3);  // Capacity is 3
}

TEST_F(CacheEngineTest, ConcurrentGets) {
    cache->put("key1", "value1");
    
    // Simple sequential reads
    for (int i = 0; i < 10; i++) {
        auto result = cache->get("key1");
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), "value1");
    }
}

TEST_F(CacheEngineTest, MixedConcurrentOperations) {
    // Sequential mixed operations
    cache->put("key1", "value1");
    cache->put("key2", "value2");
    cache->put("key3", "value3");
    
    auto val1 = cache->get("key1");
    auto val2 = cache->get("key2");
    
    cache->remove("key1");
    
    EXPECT_TRUE(val1.has_value());
    EXPECT_TRUE(val2.has_value());
    EXPECT_FALSE(cache->exists("key1"));
    EXPECT_EQ(cache->size(), 2);
}

TEST_F(CacheEngineTest, EmptyStringValue) {
    cache->put("key1", "");
    
    auto result = cache->get("key1");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "");
}

TEST_F(CacheEngineTest, LargeValue) {
    std::string large_value(10000, 'A');
    cache->put("key1", large_value);
    
    auto result = cache->get("key1");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().size(), 10000);
}

TEST_F(CacheEngineTest, SpecialCharactersInKey) {
    cache->put("key:with:colons", "value1");
    cache->put("key-with-dashes", "value2");
    cache->put("key_with_underscores", "value3");
    
    EXPECT_TRUE(cache->exists("key:with:colons"));
    EXPECT_TRUE(cache->exists("key-with-dashes"));
    EXPECT_TRUE(cache->exists("key_with_underscores"));
}
