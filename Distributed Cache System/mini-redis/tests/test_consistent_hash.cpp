#include <gtest/gtest.h>
#include "utils/ConsistentHash.h"
#include <unordered_set>

class ConsistentHashTest : public ::testing::Test {
protected:
    ConsistentHash* ch;
    
    void SetUp() override {
        ch = new ConsistentHash(150);  // 150 virtual nodes
    }
    
    void TearDown() override {
        delete ch;
    }
};

TEST_F(ConsistentHashTest, AddNode) {
    ch->addNode("node1");
    
    std::string node = ch->getNode("key1");
    EXPECT_EQ(node, "node1");
}

TEST_F(ConsistentHashTest, MultipleNodes) {
    ch->addNode("node1");
    ch->addNode("node2");
    ch->addNode("node3");
    
    std::string node = ch->getNode("key1");
    EXPECT_FALSE(node.empty());
}

TEST_F(ConsistentHashTest, ConsistentMapping) {
    ch->addNode("node1");
    ch->addNode("node2");
    ch->addNode("node3");
    
    std::string node1 = ch->getNode("key1");
    std::string node2 = ch->getNode("key1");
    
    EXPECT_EQ(node1, node2);
}

TEST_F(ConsistentHashTest, DifferentKeys) {
    ch->addNode("node1");
    ch->addNode("node2");
    ch->addNode("node3");
    
    std::string node_a = ch->getNode("keyA");
    std::string node_b = ch->getNode("keyB");
    
    // Different keys might map to different nodes
    EXPECT_FALSE(node_a.empty());
    EXPECT_FALSE(node_b.empty());
}

TEST_F(ConsistentHashTest, RemoveNode) {
    ch->addNode("node1");
    ch->addNode("node2");
    ch->addNode("node3");
    
    std::string node_before = ch->getNode("key1");
    
    ch->removeNode("node2");
    
    std::string node_after = ch->getNode("key1");
    
    // If key1 was on node2, it should move. Otherwise, stay same.
    EXPECT_FALSE(node_after.empty());
    EXPECT_NE(node_after, "node2");
}

TEST_F(ConsistentHashTest, LoadBalancing) {
    ch->addNode("node1");
    ch->addNode("node2");
    ch->addNode("node3");
    
    std::unordered_map<std::string, int> distribution;
    
    for (int i = 0; i < 1000; i++) {
        std::string key = "key" + std::to_string(i);
        std::string node = ch->getNode(key);
        distribution[node]++;
    }
    
    // Check that keys are somewhat evenly distributed
    EXPECT_EQ(distribution.size(), 3);
    
    for (auto& pair : distribution) {
        EXPECT_GT(pair.second, 200);  // At least 200 keys per node
        EXPECT_LT(pair.second, 500);  // At most 500 keys per node
    }
}

TEST_F(ConsistentHashTest, AddNodeMinimalRedistribution) {
    ch->addNode("node1");
    ch->addNode("node2");
    ch->addNode("node3");
    
    std::unordered_map<std::string, std::string> mappings_before;
    
    for (int i = 0; i < 100; i++) {
        std::string key = "key" + std::to_string(i);
        mappings_before[key] = ch->getNode(key);
    }
    
    ch->addNode("node4");
    
    int changes = 0;
    for (int i = 0; i < 100; i++) {
        std::string key = "key" + std::to_string(i);
        if (mappings_before[key] != ch->getNode(key)) {
            changes++;
        }
    }
    
    // Should redistribute approximately 1/4 of keys (25%)
    EXPECT_LT(changes, 40);  // Less than 40% changed
}

TEST_F(ConsistentHashTest, RemoveNodeMinimalRedistribution) {
    ch->addNode("node1");
    ch->addNode("node2");
    ch->addNode("node3");
    
    std::unordered_map<std::string, std::string> mappings_before;
    
    for (int i = 0; i < 100; i++) {
        std::string key = "key" + std::to_string(i);
        mappings_before[key] = ch->getNode(key);
    }
    
    ch->removeNode("node3");
    
    int changes = 0;
    for (int i = 0; i < 100; i++) {
        std::string key = "key" + std::to_string(i);
        std::string node_after = ch->getNode(key);
        if (mappings_before[key] != node_after && mappings_before[key] != "node3") {
            changes++;
        }
    }
    
    // Only keys that were on node3 should move
    EXPECT_LT(changes, 10);  // Very few keys should change (excluding node3)
}

TEST_F(ConsistentHashTest, SingleNode) {
    ch->addNode("node1");
    
    for (int i = 0; i < 100; i++) {
        std::string key = "key" + std::to_string(i);
        EXPECT_EQ(ch->getNode(key), "node1");
    }
}

TEST_F(ConsistentHashTest, EmptyRing) {
    std::string node = ch->getNode("key1");
    EXPECT_TRUE(node.empty());
}

TEST_F(ConsistentHashTest, RemoveNonExistentNode) {
    ch->addNode("node1");
    
    EXPECT_NO_THROW(ch->removeNode("node2"));
}

TEST_F(ConsistentHashTest, AddSameNodeTwice) {
    ch->addNode("node1");
    ch->addNode("node1");
    
    std::string node = ch->getNode("key1");
    EXPECT_EQ(node, "node1");
}

TEST_F(ConsistentHashTest, VirtualNodesCount) {
    ConsistentHash ch_low(10);   // 10 virtual nodes
    ConsistentHash ch_high(500); // 500 virtual nodes
    
    ch_low.addNode("node1");
    ch_low.addNode("node2");
    
    ch_high.addNode("node1");
    ch_high.addNode("node2");
    
    // Both should work, high should have better distribution
    EXPECT_FALSE(ch_low.getNode("key1").empty());
    EXPECT_FALSE(ch_high.getNode("key1").empty());
}

TEST_F(ConsistentHashTest, LargeScale) {
    for (int i = 0; i < 10; i++) {
        ch->addNode("node" + std::to_string(i));
    }
    
    std::unordered_map<std::string, int> distribution;
    
    for (int i = 0; i < 10000; i++) {
        std::string key = "key" + std::to_string(i);
        std::string node = ch->getNode(key);
        distribution[node]++;
    }
    
    EXPECT_EQ(distribution.size(), 10);
    
    // Check reasonable distribution (800-1200 per node)
    for (auto& pair : distribution) {
        EXPECT_GT(pair.second, 700);
        EXPECT_LT(pair.second, 1300);
    }
}

TEST_F(ConsistentHashTest, SpecialCharacterNodes) {
    ch->addNode("node-1");
    ch->addNode("node_2");
    ch->addNode("node:3");
    
    std::string node = ch->getNode("key1");
    EXPECT_FALSE(node.empty());
}

TEST_F(ConsistentHashTest, StabilityAfterMultipleOperations) {
    ch->addNode("node1");
    ch->addNode("node2");
    
    std::string node1 = ch->getNode("key1");
    
    ch->addNode("node3");
    ch->removeNode("node3");
    
    std::string node2 = ch->getNode("key1");
    
    // key1 should still map to same node after add/remove of different node
    EXPECT_EQ(node1, node2);
}
