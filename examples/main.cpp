#include "json_parser.h"
#include <iostream>

int main() {
    try {
        // Test basic types
        {
            std::cout << "Test basic types:" << std::endl;
            json::JsonParser parser(R"({"string": "Hello", "number": 42, "boolean": true, "null": null})");
            auto value = parser.parse();
            const auto& obj = value.asObject();
            
            std::cout << "String: " << obj.at("string").asString() << std::endl;
            std::cout << "Number: " << obj.at("number").asNumber() << std::endl;
            std::cout << "Boolean: " << (obj.at("boolean").asBoolean() ? "true" : "false") << std::endl;
            std::cout << "Null: " << (obj.at("null").isNull() ? "null" : "not null") << std::endl;
            std::cout << std::endl;
        }

        // Test array
        {
            std::cout << "Test array:" << std::endl;
            json::JsonParser parser(R"([1, "two", true, null])");
            auto value = parser.parse();
            const auto& array = value.asArray();
            
            std::cout << "Array size: " << array.size() << std::endl;
            std::cout << "First element: " << array[0].asNumber() << std::endl;
            std::cout << "Second element: " << array[1].asString() << std::endl;
            std::cout << "Third element: " << (array[2].asBoolean() ? "true" : "false") << std::endl;
            std::cout << "Fourth element: " << (array[3].isNull() ? "null" : "not null") << std::endl;
            std::cout << std::endl;
        }

        // Test nested object
        {
            std::cout << "Test nested object:" << std::endl;
            json::JsonParser parser(R"({
                "person": {
                    "name": "Zhang San",
                    "age": 25,
                    "address": {
                        "city": "Beijing",
                        "street": "Chaoyang District"
                    },
                    "hobbies": ["Reading", "Sports", "Programming"]
                }
            })");
            auto value = parser.parse();
            const auto& person = value.asObject().at("person");
            
            std::cout << "Name: " << person.asObject().at("name").asString() << std::endl;
            std::cout << "Age: " << person.asObject().at("age").asNumber() << std::endl;
            std::cout << "City: " << person.asObject().at("address").asObject().at("city").asString() << std::endl;
            std::cout << "Street: " << person.asObject().at("address").asObject().at("street").asString() << std::endl;
            
            const auto& hobbies = person.asObject().at("hobbies").asArray();
            std::cout << "Hobbies: ";
            for (size_t i = 0; i < hobbies.size(); ++i) {
                if (i > 0) std::cout << ", ";
                std::cout << hobbies[i].asString();
            }
            std::cout << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 