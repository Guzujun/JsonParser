#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace json {

// Token类型
enum class TokenType {
    // 结构标记
    LEFT_BRACE,    // {
    RIGHT_BRACE,   // }
    LEFT_BRACKET,  // [
    RIGHT_BRACKET, // ]
    COMMA,         // ,
    COLON,         // :
    
    // 字面量
    STRING,        // "string"
    NUMBER,        // 123.456
    TRUE,          // true
    FALSE,         // false
    NULL_,         // null
    
    // 特殊标记
    END_OF_FILE,   // 文件结束
    ERROR          // 错误
};

// Token结构
struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
};

// 词法分析器异常
class LexerError : public std::runtime_error {
public:
    explicit LexerError(const std::string& message) 
        : std::runtime_error(message) {}
};

// 词法分析器
class JsonLexer {
public:
    explicit JsonLexer(const std::string& input);
    
    // 获取下一个token
    Token nextToken();
    
    // 获取当前位置
    size_t getLine() const { return line_; }
    size_t getColumn() const { return column_; }

private:
    std::string input_;
    size_t current_;
    size_t line_;
    size_t column_;
    
    // 辅助函数
    char advance();
    char peek() const;
    bool isAtEnd() const;
    void skipWhitespace();
    Token makeToken(TokenType type, const std::string& value = "");
    Token makeError(const std::string& message);
    
    // 处理各种token
    Token scanString();
    Token scanNumber();
    Token scanIdentifier();
};

} // namespace json 