#pragma once

#include "json_value.h"
#include "json_lexer.h"
#include <memory>

namespace json {

// Parser exception
class ParserError : public std::runtime_error {
public:
    explicit ParserError(const std::string& message) 
        : std::runtime_error(message) {}
};

// JSON parser
class JsonParser {
public:
    explicit JsonParser(const std::string& input);
    
    // Parse JSON string
    JsonValue parse();

private:
    std::unique_ptr<JsonLexer> lexer_;
    Token current_;
    Token previous_;
    
    // Helper functions
    void advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);
    
    // Value parsers
    JsonValue parseValue();
    JsonValue parseObject();
    JsonValue parseArray();
    JsonValue parseString();
    JsonValue parseNumber();
    JsonValue parseBoolean();
    JsonValue parseNull();
};

} // namespace json 