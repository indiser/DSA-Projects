#include "network/Protocol.h"
#include <sstream>

Protocol::Message Protocol::parse(const std::string& input) {
    Message msg;
    
    if (input.empty()) {
        return msg;
    }
    
    char type = input[0];
    
    switch (type) {
        case '+':
            parseSimpleString(input, msg);
            break;
        case '-':
            parseError(input, msg);
            break;
        case ':':
            parseInteger(input, msg);
            break;
        case '$':
            parseBulkString(input, 0, msg);
            break;
        case '*':
            parseArray(input, msg);
            break;
        default:
            break;
    }
    
    return msg;
}

size_t Protocol::parseSimpleString(const std::string& input, Message& msg) {
    msg.type = Type::SimpleString;
    size_t pos = input.find("\r\n");
    if (pos != std::string::npos) {
        msg.value = input.substr(1, pos - 1);
    }
    return pos + 2;
}

size_t Protocol::parseError(const std::string& input, Message& msg) {
    msg.type = Type::Error;
    size_t pos = input.find("\r\n");
    if (pos != std::string::npos) {
        msg.value = input.substr(1, pos - 1);
    }
    return pos + 2;
}

size_t Protocol::parseInteger(const std::string& input, Message& msg) {
    msg.type = Type::Integer;
    size_t pos = input.find("\r\n");
    if (pos != std::string::npos) {
        msg.value = input.substr(1, pos - 1);
    }
    return pos + 2;
}

size_t Protocol::parseBulkString(const std::string& input, size_t pos, Message& msg) {
    msg.type = Type::BulkString;
    
    // Find end of length line
    size_t lenEnd = input.find("\r\n", pos);
    if (lenEnd == std::string::npos) {
        return std::string::npos;
    }
    
    // Parse length
    int length = std::stoi(input.substr(pos + 1, lenEnd - pos - 1));
    
    // Handle null bulk string
    if (length == -1) {
        msg.value = "";
        return lenEnd + 2;
    }
    
    // Handle empty bulk string
    if (length == 0) {
        msg.value = "";
        return lenEnd + 4; // Skip "\r\n\r\n"
    }
    
    // Extract string content
    size_t start = lenEnd + 2;
    msg.value = input.substr(start, length);
    
    return start + length + 2; // Skip content and "\r\n"
}

size_t Protocol::parseArray(const std::string& input, Message& msg) {
    msg.type = Type::Array;
    
    // Find end of count line
    size_t countEnd = input.find("\r\n");
    if (countEnd == std::string::npos) {
        return std::string::npos;
    }
    
    // Parse array count
    int count = std::stoi(input.substr(1, countEnd - 1));
    
    if (count == 0) {
        return countEnd + 2;
    }
    
    size_t pos = countEnd + 2;
    
    // Parse each element (assuming bulk strings)
    for (int i = 0; i < count; i++) {
        Message element;
        pos = parseBulkString(input, pos, element);
        if (pos == std::string::npos) {
            break;
        }
        msg.array.push_back(element.value);
    }
    
    return pos;
}

std::string Protocol::serialize(const Message& msg) {
    switch (msg.type) {
        case Type::SimpleString:
            return serializeSimpleString(msg.value);
        case Type::Error:
            return serializeError(msg.value);
        case Type::Integer:
            return serializeInteger(msg.value);
        case Type::BulkString:
            return serializeBulkString(msg.value);
        case Type::Array:
            return serializeArray(msg.array);
        default:
            return "";
    }
}

std::string Protocol::serializeSimpleString(const std::string& value) {
    return "+" + value + "\r\n";
}

std::string Protocol::serializeError(const std::string& value) {
    return "-" + value + "\r\n";
}

std::string Protocol::serializeInteger(const std::string& value) {
    return ":" + value + "\r\n";
}

std::string Protocol::serializeBulkString(const std::string& value) {
    if (value.empty()) {
        return "$-1\r\n";
    }
    return "$" + std::to_string(value.length()) + "\r\n" + value + "\r\n";
}

std::string Protocol::serializeArray(const std::vector<std::string>& array) {
    std::string result = "*" + std::to_string(array.size()) + "\r\n";
    
    for (const auto& element : array) {
        result += serializeBulkString(element);
    }
    
    return result;
}
