#include "json_lexer.h"
#include <cctype>
#include <sstream>
#include <iomanip>

namespace json {

JsonLexer::JsonLexer(const std::string& input)
    : input_(input), current_(0), line_(1), column_(0) {}

Token JsonLexer::nextToken() {
    skipWhitespace();
    
    if (isAtEnd()) {
        return makeToken(TokenType::END_OF_FILE);
    }
    
    char c = advance();
    
    switch (c) {
        case '{': return makeToken(TokenType::LEFT_BRACE);
        case '}': return makeToken(TokenType::RIGHT_BRACE);
        case '[': return makeToken(TokenType::LEFT_BRACKET);
        case ']': return makeToken(TokenType::RIGHT_BRACKET);
        case ',': return makeToken(TokenType::COMMA);
        case ':': return makeToken(TokenType::COLON);
        case '"': return scanString();
        case 't': return scanIdentifier();
        case 'f': return scanIdentifier();
        case 'n': return scanIdentifier();
        default:
            if (std::isdigit(c) || c == '-') {
                current_--; // 回退一个字符，让scanNumber处理第一个字符
                return scanNumber();
            }
            return makeError("Unexpected character");
    }
}

char JsonLexer::advance() {
    char c = input_[current_++];
    if (c == '\n') {
        line_++;
        column_ = 0;
    } else {
        column_++;
    }
    return c;
}

char JsonLexer::peek() const {
    if (isAtEnd()) return '\0';
    return input_[current_];
}

bool JsonLexer::isAtEnd() const {
    return current_ >= input_.length();
}

void JsonLexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (std::isspace(c)) {
            advance();
        } else {
            break;
        }
    }
}

Token JsonLexer::makeToken(TokenType type, const std::string& value) {
    return Token{type, value, line_, column_};
}

Token JsonLexer::makeError(const std::string& message) {
    return Token{TokenType::ERROR, message, line_, column_};
}

Token JsonLexer::scanString() {
    std::string value;
    bool escaped = false;
    
    while (!isAtEnd()) {
        char c = peek();
        if (c == '"' && !escaped) {
            advance(); // 消费结束的引号
            return makeToken(TokenType::STRING, value);
        }
        
        if (escaped) {
            switch (c) {
                case '"': value += '"'; break;
                case '\\': value += '\\'; break;
                case '/': value += '/'; break;
                case 'b': value += '\b'; break;
                case 'f': value += '\f'; break;
                case 'n': value += '\n'; break;
                case 'r': value += '\r'; break;
                case 't': value += '\t'; break;
                case 'u': {
                    // 处理Unicode转义序列
                    std::string hex;
                    for (int i = 0; i < 4; i++) {
                        advance();
                        if (isAtEnd()) {
                            return makeError("Incomplete Unicode escape sequence");
                        }
                        hex += peek();
                    }
                    
                    // 将十六进制字符串转换为Unicode字符
                    try {
                        unsigned int codePoint = std::stoi(hex, nullptr, 16);
                        if (codePoint <= 0x7F) {
                            // ASCII字符
                            value += static_cast<char>(codePoint);
                        } else if (codePoint <= 0x7FF) {
                            // 2字节UTF-8
                            value += static_cast<char>(0xC0 | (codePoint >> 6));
                            value += static_cast<char>(0x80 | (codePoint & 0x3F));
                        } else if (codePoint <= 0xFFFF) {
                            // 3字节UTF-8
                            value += static_cast<char>(0xE0 | (codePoint >> 12));
                            value += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
                            value += static_cast<char>(0x80 | (codePoint & 0x3F));
                        } else {
                            // 4字节UTF-8
                            value += static_cast<char>(0xF0 | (codePoint >> 18));
                            value += static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F));
                            value += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
                            value += static_cast<char>(0x80 | (codePoint & 0x3F));
                        }
                    } catch (const std::exception&) {
                        return makeError("Invalid Unicode escape sequence");
                    }
                    break;
                }
                default:
                    return makeError("Invalid escape sequence");
            }
            escaped = false;
        } else if (c == '\\') {
            escaped = true;
        } else {
            value += c;
        }
        advance();
    }
    
    return makeError("Unterminated string");
}

Token JsonLexer::scanNumber() {
    std::string value;
    bool hasDecimal = false;
    
    // 处理负号
    if (peek() == '-') {
        value += advance();
    }
    
    // 处理整数部分
    if (!std::isdigit(peek())) {
        return makeError("Expected digit");
    }
    
    while (!isAtEnd() && std::isdigit(peek())) {
        value += advance();
    }
    
    // 处理小数部分
    if (!isAtEnd() && peek() == '.') {
        hasDecimal = true;
        value += advance();
        
        if (!std::isdigit(peek())) {
            return makeError("Expected digit after decimal point");
        }
        
        while (!isAtEnd() && std::isdigit(peek())) {
            value += advance();
        }
    }
    
    // 处理指数部分
    if (!isAtEnd() && (peek() == 'e' || peek() == 'E')) {
        value += advance();
        
        if (!isAtEnd() && (peek() == '+' || peek() == '-')) {
            value += advance();
        }
        
        if (!std::isdigit(peek())) {
            return makeError("Expected digit in exponent");
        }
        
        while (!isAtEnd() && std::isdigit(peek())) {
            value += advance();
        }
    }
    
    return makeToken(TokenType::NUMBER, value);
}

Token JsonLexer::scanIdentifier() {
    std::string value;
    value += input_[current_ - 1]; // 添加第一个字符
    
    while (!isAtEnd() && std::isalpha(peek())) {
        value += advance();
    }
    
    if (value == "true") return makeToken(TokenType::TRUE);
    if (value == "false") return makeToken(TokenType::FALSE);
    if (value == "null") return makeToken(TokenType::NULL_);
    
    return makeError("Invalid identifier");
}

} // namespace json 