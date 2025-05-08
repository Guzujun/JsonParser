#include "json_value.h"
#include <sstream>
#include <iomanip>

namespace json {

std::string JsonValue::toString() const {
    if (isNull()) {
        return "null";
    }
    
    if (isBoolean()) {
        return asBoolean() ? "true" : "false";
    }
    
    if (isNumber()) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(6) << asNumber();
        std::string str = ss.str();
        
        // 移除末尾的0
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        if (str.back() == '.') {
            str.pop_back();
        }
        
        return str;
    }
    
    if (isString()) {
        std::stringstream ss;
        ss << '"';
        
        for (char c : asString()) {
            switch (c) {
                case '"': ss << "\\\""; break;
                case '\\': ss << "\\\\"; break;
                case '/': ss << "\\/"; break;
                case '\b': ss << "\\b"; break;
                case '\f': ss << "\\f"; break;
                case '\n': ss << "\\n"; break;
                case '\r': ss << "\\r"; break;
                case '\t': ss << "\\t"; break;
                default:
                    if (c < 32 || c > 126) {
                        // 处理非ASCII字符
                        ss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
                    } else {
                        ss << c;
                    }
            }
        }
        
        ss << '"';
        return ss.str();
    }
    
    if (isArray()) {
        std::stringstream ss;
        ss << '[';
        
        const auto& array = asArray();
        for (size_t i = 0; i < array.size(); ++i) {
            if (i > 0) {
                ss << ',';
            }
            ss << array[i].toString();
        }
        
        ss << ']';
        return ss.str();
    }
    
    if (isObject()) {
        std::stringstream ss;
        ss << '{';
        
        const auto& object = asObject();
        size_t i = 0;
        for (const auto& [key, value] : object) {
            if (i > 0) {
                ss << ',';
            }
            ss << '"' << key << "\":" << value.toString();
            ++i;
        }
        
        ss << '}';
        return ss.str();
    }
    
    return "null"; // 不应该到达这里
}

} // namespace json 