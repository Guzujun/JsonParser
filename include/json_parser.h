#pragma once

#include "json_value.h"
#include "json_lexer.h"
#include <memory>

namespace json {

// 解析器异常
class ParserError : public std::runtime_error {
public:
    explicit ParserError(const std::string& message) 
        : std::runtime_error(message) {}
};

// JSON解析器
class JsonParser {
public:
    explicit JsonParser(const std::string& input);
    
    // 解析JSON字符串
    JsonValue parse();

private:
    std::unique_ptr<JsonLexer> lexer_;
    Token current_;
    Token previous_;
    
    // 辅助函数
    void advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);
    
    // 解析各种JSON值
    JsonValue parseValue();
    JsonValue parseObject();
    JsonValue parseArray();
    JsonValue parseString();
    JsonValue parseNumber();
    JsonValue parseBoolean();
    JsonValue parseNull();
};

} // namespace json 