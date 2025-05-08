# JSON Parser

一个用C++实现的JSON解析器，支持完整的JSON规范，包括：

- 所有基本数据类型（字符串、数字、布尔值、null）
- 数组和对象的嵌套
- 完整的字符串转义处理
- Unicode字符支持
- 错误处理和异常报告

## 构建要求

- CMake 3.10或更高版本
- C++17兼容的编译器

## 构建步骤

```bash
mkdir build
cd build
cmake ..
make
```

## 运行测试

```bash
cd build
ctest
```

## 使用示例

```cpp
#include "json_parser.h"
#include <iostream>

int main() {
    try {
        // 解析JSON字符串
        json::JsonParser parser(R"({"name": "John", "age": 30})");
        auto value = parser.parse();
        
        // 访问解析后的值
        const auto& object = value.asObject();
        std::cout << "Name: " << object.at("name").asString() << std::endl;
        std::cout << "Age: " << object.at("age").asNumber() << std::endl;
        
        // 序列化回JSON字符串
        std::cout << "Serialized: " << value.toString() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

## 特性

- 完整的JSON规范支持
- 高效的解析和序列化
- 详细的错误报告
- 类型安全的API
- 完整的单元测试覆盖

## 许可证

MIT License 