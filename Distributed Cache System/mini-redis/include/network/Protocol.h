#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <vector>

class Protocol {
public:
    enum class Type {
        SimpleString,
        Error,
        Integer,
        BulkString,
        Array
    };
    
    struct Message {
        Type type;
        std::string value;
        std::vector<std::string> array;
    };
    
    Protocol() = default;
    ~Protocol() = default;
    
    Message parse(const std::string& input);
    std::string serialize(const Message& msg);
    
private:
    size_t parseSimpleString(const std::string& input, Message& msg);
    size_t parseError(const std::string& input, Message& msg);
    size_t parseInteger(const std::string& input, Message& msg);
    size_t parseBulkString(const std::string& input, size_t pos, Message& msg);
    size_t parseArray(const std::string& input, Message& msg);
    
    std::string serializeSimpleString(const std::string& value);
    std::string serializeError(const std::string& value);
    std::string serializeInteger(const std::string& value);
    std::string serializeBulkString(const std::string& value);
    std::string serializeArray(const std::vector<std::string>& array);
};

#endif // PROTOCOL_H
