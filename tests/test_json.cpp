#include "json_parser.h"
#include <iostream>
#include <cassert>
#include <cmath>

// 浮点数比较的辅助函数
bool isClose(double a, double b, double epsilon = 1e-6) {
    return std::abs(a - b) < epsilon;
}

void testBasicTypes() {
    // 测试字符串
    {
        json::JsonParser parser("\"Hello, World!\"");
        auto value = parser.parse();
        assert(value.isString());
        assert(value.asString() == "Hello, World!");
    }
    
    // 测试数字
    {
        json::JsonParser parser("123.456");
        auto value = parser.parse();
        assert(value.isNumber());
        assert(isClose(value.asNumber(), 123.456));
    }
    
    // 测试布尔值
    {
        json::JsonParser parser("true");
        auto value = parser.parse();
        assert(value.isBoolean());
        assert(value.asBoolean() == true);
    }
    
    // 测试null
    {
        json::JsonParser parser("null");
        auto value = parser.parse();
        assert(value.isNull());
    }
}

void testArrays() {
    // 测试空数组
    {
        json::JsonParser parser("[]");
        auto value = parser.parse();
        assert(value.isArray());
        assert(value.asArray().empty());
    }
    
    // 测试简单数组
    {
        json::JsonParser parser("[1, 2, 3]");
        auto value = parser.parse();
        assert(value.isArray());
        const auto& array = value.asArray();
        assert(array.size() == 3);
        assert(isClose(array[0].asNumber(), 1.0));
        assert(isClose(array[1].asNumber(), 2.0));
        assert(isClose(array[2].asNumber(), 3.0));
    }
    
    // 测试混合类型数组
    {
        json::JsonParser parser("[1, \"two\", true, null]");
        auto value = parser.parse();
        assert(value.isArray());
        const auto& array = value.asArray();
        assert(array.size() == 4);
        assert(isClose(array[0].asNumber(), 1.0));
        assert(array[1].asString() == "two");
        assert(array[2].asBoolean() == true);
        assert(array[3].isNull());
    }
}

void testObjects() {
    // 测试空对象
    {
        json::JsonParser parser("{}");
        auto value = parser.parse();
        assert(value.isObject());
        assert(value.asObject().empty());
    }
    
    // 测试简单对象
    {
        json::JsonParser parser("{\"name\": \"John\", \"age\": 30}");
        auto value = parser.parse();
        assert(value.isObject());
        const auto& object = value.asObject();
        assert(object.size() == 2);
        assert(object.at("name").asString() == "John");
        assert(isClose(object.at("age").asNumber(), 30.0));
    }
    
    // 测试嵌套对象
    {
        json::JsonParser parser("{\"person\": {\"name\": \"John\", \"age\": 30}}");
        auto value = parser.parse();
        assert(value.isObject());
        const auto& object = value.asObject();
        assert(object.size() == 1);
        const auto& person = object.at("person");
        assert(person.isObject());
        assert(person.asObject().at("name").asString() == "John");
        assert(isClose(person.asObject().at("age").asNumber(), 30.0));
    }
}

void testStringEscaping() {
    // 测试转义字符
    {
        json::JsonParser parser("\"\\\"\\\\\\/\\b\\f\\n\\r\\t\"");
        auto value = parser.parse();
        assert(value.isString());
        assert(value.asString() == "\"\\/\b\f\n\r\t");
    }
    
    // 测试Unicode转义
    {
        json::JsonParser parser("\"\\u0041\"");
        auto value = parser.parse();
        assert(value.isString());
        assert(value.asString() == "A");
    }
}

void testComplexExample() {
    const char* json = R"({
        "name": "John Doe",
        "age": 30,
        "isActive": true,
        "scores": [95, 87, 92],
        "address": {
            "street": "123 Main St",
            "city": "New York",
            "zip": "10001"
        },
        "tags": ["employee", "developer"],
        "metadata": null
    })";
    
    json::JsonParser parser(json);
    auto value = parser.parse();
    
    // 验证解析结果
    assert(value.isObject());
    const auto& object = value.asObject();
    
    assert(object.at("name").asString() == "John Doe");
    assert(isClose(object.at("age").asNumber(), 30.0));
    assert(object.at("isActive").asBoolean() == true);
    
    const auto& scores = object.at("scores").asArray();
    assert(scores.size() == 3);
    assert(isClose(scores[0].asNumber(), 95.0));
    assert(isClose(scores[1].asNumber(), 87.0));
    assert(isClose(scores[2].asNumber(), 92.0));
    
    const auto& address = object.at("address").asObject();
    assert(address.at("street").asString() == "123 Main St");
    assert(address.at("city").asString() == "New York");
    assert(address.at("zip").asString() == "10001");
    
    const auto& tags = object.at("tags").asArray();
    assert(tags.size() == 2);
    assert(tags[0].asString() == "employee");
    assert(tags[1].asString() == "developer");
    
    assert(object.at("metadata").isNull());
}

int main() {
    try {
        testBasicTypes();
        testArrays();
        testObjects();
        testStringEscaping();
        testComplexExample();
        
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
} 