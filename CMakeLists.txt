cmake_minimum_required(VERSION 3.10)
project(JsonParser VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加include目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加源文件
set(SOURCES
    src/json_value.cpp
    src/json_parser.cpp
    src/json_lexer.cpp
)

# 创建库
add_library(jsonparser ${SOURCES})

# 添加测试和示例
enable_testing()
add_subdirectory(tests)
add_subdirectory(examples) 