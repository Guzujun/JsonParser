#include "json_lexer.h"
#include <cctype>

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
                    // TODO: 实现Unicode字符转换
                    value += "\\u" + hex;
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