#include "json_parser.h"
#include <sstream>

namespace json {

JsonParser::JsonParser(const std::string& input)
    : lexer_(std::make_unique<JsonLexer>(input)) {
    advance();
}

JsonValue JsonParser::parse() {
    JsonValue value = parseValue();
    
    if (current_.type != TokenType::END_OF_FILE) {
        throw ParserError("Expected end of file");
    }
    
    return value;
}

void JsonParser::advance() {
    previous_ = current_;
    current_ = lexer_->nextToken();
}

bool JsonParser::check(TokenType type) const {
    return current_.type == type;
}

bool JsonParser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token JsonParser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        Token token = current_;
        advance();
        return token;
    }
    
    std::stringstream ss;
    ss << message << " at line " << current_.line << ", column " << current_.column;
    throw ParserError(ss.str());
}

JsonValue JsonParser::parseValue() {
    switch (current_.type) {
        case TokenType::LEFT_BRACE:
            return parseObject();
        case TokenType::LEFT_BRACKET:
            return parseArray();
        case TokenType::STRING:
            return parseString();
        case TokenType::NUMBER:
            return parseNumber();
        case TokenType::TRUE:
            advance();
            return JsonValue(true);
        case TokenType::FALSE:
            advance();
            return JsonValue(false);
        case TokenType::NULL_:
            advance();
            return JsonValue(nullptr);
        default:
            throw ParserError("Unexpected token");
    }
}

JsonValue JsonParser::parseObject() {
    JsonValue::Object object;
    
    advance(); // Consume left brace
    
    if (!check(TokenType::RIGHT_BRACE)) {
        do {
            // Parse key
            Token key = consume(TokenType::STRING, "Expected string key");
            
            // Parse colon
            consume(TokenType::COLON, "Expected ':' after key");
            
            // Parse value
            object[key.value] = parseValue();
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after object");
    return JsonValue(std::move(object));
}

JsonValue JsonParser::parseArray() {
    JsonValue::Array array;
    
    advance(); // Consume left bracket
    
    if (!check(TokenType::RIGHT_BRACKET)) {
        do {
            array.push_back(parseValue());
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RIGHT_BRACKET, "Expected ']' after array");
    return JsonValue(std::move(array));
}

JsonValue JsonParser::parseString() {
    JsonValue value(current_.value);
    advance();
    return value;
}

JsonValue JsonParser::parseNumber() {
    double number = std::stod(current_.value);
    advance();
    return JsonValue(number);
}

} // namespace json 