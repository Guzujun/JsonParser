# JSON Parser

A JSON parser implemented in C++ that supports the complete JSON specification, including:

- All basic data types (string, number, boolean, null)
- Nested arrays and objects
- Complete string escape handling
- Unicode character support
- Error handling and exception reporting

## Build Requirements

- CMake 3.10 or higher
- C++17 compatible compiler
- Git (for cloning the repository)

## Build Steps

1. Clone the repository:
```bash
git clone https://github.com/yourusername/JsonParser.git
cd JsonParser
```

2. Create and enter build directory:
```bash
mkdir build
cd build
```

3. Configure and build the project:
```bash
cmake ..
make
```

## Running Tests

After building, you can run the tests:
```bash
cd build
ctest
```

## Running Examples

The project includes example code that demonstrates basic usage:
```bash
cd build
./examples/json_example
```

## Usage Example

```cpp
#include "json_parser.h"
#include <iostream>

int main() {
    try {
        // Parse JSON string
        json::JsonParser parser(R"({"name": "John", "age": 30})");
        auto value = parser.parse();
        
        // Access parsed values
        const auto& object = value.asObject();
        std::cout << "Name: " << object.at("name").asString() << std::endl;
        std::cout << "Age: " << object.at("age").asNumber() << std::endl;
        
        // Serialize back to JSON string
        std::cout << "Serialized: " << value.toString() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

## Project Structure

```
JsonParser/
├── CMakeLists.txt           # Main CMake configuration
├── README.md               # This file
├── include/                # Header files
│   ├── json_lexer.h
│   ├── json_parser.h
│   └── json_value.h
├── src/                    # Source files
│   ├── json_lexer.cpp
│   ├── json_parser.cpp
│   └── json_value.cpp
├── examples/               # Example code
│   └── main.cpp
└── tests/                  # Test files
    └── test_json.cpp
```

## Features

- Full JSON specification support
- Efficient parsing and serialization
- Detailed error reporting
- Type-safe API
- Complete unit test coverage

## License

MIT License 