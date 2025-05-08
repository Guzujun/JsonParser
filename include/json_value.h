#pragma once

#include <string>
#include <vector>
#include <map>
#include <variant>
#include <memory>

namespace json {

class JsonValue {
public:
    // Supported data types
    using Object = std::map<std::string, JsonValue>;
    using Array = std::vector<JsonValue>;
    using String = std::string;
    using Number = double;
    using Boolean = bool;
    using Null = std::nullptr_t;

    // Constructors
    JsonValue() : value_(Null()) {}
    JsonValue(const String& str) : value_(str) {}
    JsonValue(String&& str) : value_(std::move(str)) {}
    JsonValue(Number num) : value_(num) {}
    JsonValue(Boolean b) : value_(b) {}
    JsonValue(Null) : value_(nullptr) {}
    JsonValue(const Object& obj) : value_(obj) {}
    JsonValue(Object&& obj) : value_(std::move(obj)) {}
    JsonValue(const Array& arr) : value_(arr) {}
    JsonValue(Array&& arr) : value_(std::move(arr)) {}

    // Type checks
    bool isObject() const { return std::holds_alternative<Object>(value_); }
    bool isArray() const { return std::holds_alternative<Array>(value_); }
    bool isString() const { return std::holds_alternative<String>(value_); }
    bool isNumber() const { return std::holds_alternative<Number>(value_); }
    bool isBoolean() const { return std::holds_alternative<Boolean>(value_); }
    bool isNull() const { return std::holds_alternative<Null>(value_); }

    // Get value
    const Object& asObject() const { return std::get<Object>(value_); }
    const Array& asArray() const { return std::get<Array>(value_); }
    const String& asString() const { return std::get<String>(value_); }
    Number asNumber() const { return std::get<Number>(value_); }
    Boolean asBoolean() const { return std::get<Boolean>(value_); }

    // Serialization
    std::string toString() const;

private:
    std::variant<Object, Array, String, Number, Boolean, Null> value_;
};

} // namespace json 