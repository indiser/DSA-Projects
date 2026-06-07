#include <gtest/gtest.h>
#include "network/Protocol.h"

class ProtocolTest : public ::testing::Test {
protected:
    Protocol* protocol;
    
    void SetUp() override {
        protocol = new Protocol();
    }
    
    void TearDown() override {
        delete protocol;
    }
};

TEST_F(ProtocolTest, ParseSimpleString) {
    std::string input = "+OK\r\n";
    auto message = protocol->parse(input);
    
    EXPECT_EQ(message.type, Protocol::Type::SimpleString);
    EXPECT_EQ(message.value, "OK");
}

TEST_F(ProtocolTest, ParseError) {
    std::string input = "-ERR unknown command\r\n";
    auto message = protocol->parse(input);
    
    EXPECT_EQ(message.type, Protocol::Type::Error);
    EXPECT_EQ(message.value, "ERR unknown command");
}

TEST_F(ProtocolTest, ParseInteger) {
    std::string input = ":1000\r\n";
    auto message = protocol->parse(input);
    
    EXPECT_EQ(message.type, Protocol::Type::Integer);
    EXPECT_EQ(message.value, "1000");
}

TEST_F(ProtocolTest, ParseBulkString) {
    std::string input = "$5\r\nhello\r\n";
    auto message = protocol->parse(input);
    
    EXPECT_EQ(message.type, Protocol::Type::BulkString);
    EXPECT_EQ(message.value, "hello");
}

TEST_F(ProtocolTest, ParseNullBulkString) {
    std::string input = "$-1\r\n";
    auto message = protocol->parse(input);
    
    EXPECT_EQ(message.type, Protocol::Type::BulkString);
    EXPECT_TRUE(message.value.empty());
}

TEST_F(ProtocolTest, ParseEmptyBulkString) {
    std::string input = "$0\r\n\r\n";
    auto message = protocol->parse(input);
    
    EXPECT_EQ(message.type, Protocol::Type::BulkString);
    EXPECT_EQ(message.value, "");
}

TEST_F(ProtocolTest, ParseArray) {
    std::string input = "*2\r\n$3\r\nGET\r\n$3\r\nkey\r\n";
    auto message = protocol->parse(input);
    
    EXPECT_EQ(message.type, Protocol::Type::Array);
    EXPECT_EQ(message.array.size(), 2);
    EXPECT_EQ(message.array[0], "GET");
    EXPECT_EQ(message.array[1], "key");
}

TEST_F(ProtocolTest, ParseEmptyArray) {
    std::string input = "*0\r\n";
    auto message = protocol->parse(input);
    
    EXPECT_EQ(message.type, Protocol::Type::Array);
    EXPECT_EQ(message.array.size(), 0);
}

TEST_F(ProtocolTest, ParseSetCommand) {
    std::string input = "*3\r\n$3\r\nSET\r\n$4\r\nkey1\r\n$6\r\nvalue1\r\n";
    auto message = protocol->parse(input);
    
    EXPECT_EQ(message.type, Protocol::Type::Array);
    EXPECT_EQ(message.array.size(), 3);
    EXPECT_EQ(message.array[0], "SET");
    EXPECT_EQ(message.array[1], "key1");
    EXPECT_EQ(message.array[2], "value1");
}

TEST_F(ProtocolTest, SerializeSimpleString) {
    Protocol::Message msg;
    msg.type = Protocol::Type::SimpleString;
    msg.value = "OK";
    
    std::string output = protocol->serialize(msg);
    EXPECT_EQ(output, "+OK\r\n");
}

TEST_F(ProtocolTest, SerializeError) {
    Protocol::Message msg;
    msg.type = Protocol::Type::Error;
    msg.value = "ERR invalid command";
    
    std::string output = protocol->serialize(msg);
    EXPECT_EQ(output, "-ERR invalid command\r\n");
}

TEST_F(ProtocolTest, SerializeInteger) {
    Protocol::Message msg;
    msg.type = Protocol::Type::Integer;
    msg.value = "42";
    
    std::string output = protocol->serialize(msg);
    EXPECT_EQ(output, ":42\r\n");
}

TEST_F(ProtocolTest, SerializeBulkString) {
    Protocol::Message msg;
    msg.type = Protocol::Type::BulkString;
    msg.value = "hello";
    
    std::string output = protocol->serialize(msg);
    EXPECT_EQ(output, "$5\r\nhello\r\n");
}

TEST_F(ProtocolTest, SerializeNullBulkString) {
    Protocol::Message msg;
    msg.type = Protocol::Type::BulkString;
    msg.value = "";
    
    std::string output = protocol->serialize(msg);
    EXPECT_EQ(output, "$-1\r\n");
}

TEST_F(ProtocolTest, SerializeArray) {
    Protocol::Message msg;
    msg.type = Protocol::Type::Array;
    msg.array = {"GET", "key"};
    
    std::string output = protocol->serialize(msg);
    EXPECT_EQ(output, "*2\r\n$3\r\nGET\r\n$3\r\nkey\r\n");
}

TEST_F(ProtocolTest, ParseNegativeInteger) {
    std::string input = ":-100\r\n";
    auto message = protocol->parse(input);
    
    EXPECT_EQ(message.type, Protocol::Type::Integer);
    EXPECT_EQ(message.value, "-100");
}

TEST_F(ProtocolTest, ParseLargeBulkString) {
    std::string large_value(1000, 'A');
    std::string input = "$1000\r\n" + large_value + "\r\n";
    auto message = protocol->parse(input);
    
    EXPECT_EQ(message.type, Protocol::Type::BulkString);
    EXPECT_EQ(message.value.size(), 1000);
}

TEST_F(ProtocolTest, ParseLargeArray) {
    std::string input = "*100\r\n";
    for (int i = 0; i < 100; i++) {
        input += "$3\r\nkey\r\n";
    }
    
    auto message = protocol->parse(input);
    EXPECT_EQ(message.type, Protocol::Type::Array);
    EXPECT_EQ(message.array.size(), 100);
}

TEST_F(ProtocolTest, ParseSpecialCharacters) {
    std::string input = "$12\r\nhello\nworld!\r\n";
    auto message = protocol->parse(input);
    
    EXPECT_EQ(message.type, Protocol::Type::BulkString);
    EXPECT_EQ(message.value, "hello\nworld!");
}

TEST_F(ProtocolTest, RoundTripSimpleString) {
    Protocol::Message msg;
    msg.type = Protocol::Type::SimpleString;
    msg.value = "PONG";
    
    std::string serialized = protocol->serialize(msg);
    auto parsed = protocol->parse(serialized);
    
    EXPECT_EQ(parsed.type, msg.type);
    EXPECT_EQ(parsed.value, msg.value);
}

TEST_F(ProtocolTest, RoundTripArray) {
    Protocol::Message msg;
    msg.type = Protocol::Type::Array;
    msg.array = {"SET", "mykey", "myvalue"};
    
    std::string serialized = protocol->serialize(msg);
    auto parsed = protocol->parse(serialized);
    
    EXPECT_EQ(parsed.type, msg.type);
    EXPECT_EQ(parsed.array.size(), msg.array.size());
    EXPECT_EQ(parsed.array, msg.array);
}
