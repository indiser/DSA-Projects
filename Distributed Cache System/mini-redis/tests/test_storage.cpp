#include <gtest/gtest.h>
#include "storage/InMemoryStorage.h"
#include <algorithm>

class StorageTest : public ::testing::Test {
protected:
    InMemoryStorage* storage;
    
    void SetUp() override {
        storage = new InMemoryStorage();
    }
    
    void TearDown() override {
        delete storage;
    }
};

TEST_F(StorageTest, BasicSetAndGet) {
    storage->set("key1", "value1");
    
    auto result = storage->get("key1");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "value1");
}

TEST_F(StorageTest, GetNonExistent) {
    auto result = storage->get("nonexistent");
    EXPECT_FALSE(result.has_value());
}

TEST_F(StorageTest, UpdateValue) {
    storage->set("key1", "value1");
    storage->set("key1", "value2");
    
    auto result = storage->get("key1");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "value2");
}

TEST_F(StorageTest, Remove) {
    storage->set("key1", "value1");
    
    EXPECT_TRUE(storage->remove("key1"));
    EXPECT_FALSE(storage->get("key1").has_value());
}

TEST_F(StorageTest, RemoveNonExistent) {
    EXPECT_FALSE(storage->remove("nonexistent"));
}

TEST_F(StorageTest, Exists) {
    storage->set("key1", "value1");
    
    EXPECT_TRUE(storage->exists("key1"));
    EXPECT_FALSE(storage->exists("key2"));
}

TEST_F(StorageTest, Size) {
    EXPECT_EQ(storage->size(), 0);
    
    storage->set("key1", "value1");
    EXPECT_EQ(storage->size(), 1);
    
    storage->set("key2", "value2");
    EXPECT_EQ(storage->size(), 2);
    
    storage->remove("key1");
    EXPECT_EQ(storage->size(), 1);
}

TEST_F(StorageTest, Clear) {
    storage->set("key1", "value1");
    storage->set("key2", "value2");
    storage->set("key3", "value3");
    
    storage->clear();
    
    EXPECT_EQ(storage->size(), 0);
    EXPECT_FALSE(storage->exists("key1"));
}

TEST_F(StorageTest, MultipleKeys) {
    storage->set("key1", "value1");
    storage->set("key2", "value2");
    storage->set("key3", "value3");
    
    EXPECT_EQ(storage->size(), 3);
    EXPECT_EQ(storage->get("key1").value(), "value1");
    EXPECT_EQ(storage->get("key2").value(), "value2");
    EXPECT_EQ(storage->get("key3").value(), "value3");
}

TEST_F(StorageTest, EmptyStringValue) {
    storage->set("key1", "");
    
    auto result = storage->get("key1");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "");
}

TEST_F(StorageTest, LargeValue) {
    std::string large_value(100000, 'X');
    storage->set("key1", large_value);
    
    auto result = storage->get("key1");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().size(), 100000);
}

TEST_F(StorageTest, SpecialCharacters) {
    storage->set("key!@#$%", "value^&*()");
    
    auto result = storage->get("key!@#$%");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "value^&*()");
}

TEST_F(StorageTest, UnicodeSupport) {
    storage->set("键", "值");
    storage->set("clé", "valeur");
    storage->set("مفتاح", "قيمة");
    
    EXPECT_TRUE(storage->exists("键"));
    EXPECT_TRUE(storage->exists("clé"));
    EXPECT_TRUE(storage->exists("مفتاح"));
}

TEST_F(StorageTest, OverwriteIncrementsSize) {
    storage->set("key1", "value1");
    EXPECT_EQ(storage->size(), 1);
    
    storage->set("key1", "value2");
    EXPECT_EQ(storage->size(), 1);  // Size should not change
}

TEST_F(StorageTest, GetAllKeys) {
    storage->set("key1", "value1");
    storage->set("key2", "value2");
    storage->set("key3", "value3");
    
    auto keys = storage->getAllKeys();
    EXPECT_EQ(keys.size(), 3);
    
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), std::string("key1")) != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), std::string("key2")) != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), std::string("key3")) != keys.end());
}

TEST_F(StorageTest, ClearEmptyStorage) {
    storage->clear();
    EXPECT_EQ(storage->size(), 0);
}

TEST_F(StorageTest, StressTest) {
    for (int i = 0; i < 1000; i++) {
        storage->set("key" + std::to_string(i), "value" + std::to_string(i));
    }
    
    EXPECT_EQ(storage->size(), 1000);
    
    for (int i = 0; i < 1000; i++) {
        auto result = storage->get("key" + std::to_string(i));
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), "value" + std::to_string(i));
    }
}

TEST_F(StorageTest, RemoveAllKeys) {
    storage->set("key1", "value1");
    storage->set("key2", "value2");
    storage->set("key3", "value3");
    
    storage->remove("key1");
    storage->remove("key2");
    storage->remove("key3");
    
    EXPECT_EQ(storage->size(), 0);
}
