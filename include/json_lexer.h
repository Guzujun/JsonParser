#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace json {

// Token types
enum class TokenType {
    // Structural tokens
    LEFT_BRACE,    // {
    RIGHT_BRACE,   // }
    LEFT_BRACKET,  // [
    RIGHT_BRACKET, // ]
    COMMA,         // ,
    COLON,         // :
    
    // Literals
    STRING,        // "string"
    NUMBER,        // 123.456
    TRUE,          // true
    FALSE,         // false
    NULL_,         // null
    
    // Special tokens
    END_OF_FILE,   // End of file
    ERROR          // Error
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
};

// Lexer exception
class LexerError : public std::runtime_error {
public:
    explicit LexerError(const std::string& message) 
        : std::runtime_error(message) {}
};

// Lexer
class JsonLexer {
public:
    explicit JsonLexer(const std::string& input);
    
    // Get next token
    Token nextToken();
    
    // Get current position
    size_t getLine() const { return line_; }
    size_t getColumn() const { return column_; }

private:
    std::string input_;
    size_t current_;
    size_t line_;
    size_t column_;
    
    // Helper functions
    char advance();
    char peek() const;
    bool isAtEnd() const;
    void skipWhitespace();
    Token makeToken(TokenType type, const std::string& value = "");
    Token makeError(const std::string& message);
    
    // Token handlers
    Token scanString();
    Token scanNumber();
    Token scanIdentifier();
};

} // namespace json 