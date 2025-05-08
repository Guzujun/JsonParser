add_executable(test_json test_json.cpp)
target_link_libraries(test_json jsonparser)

add_test(NAME test_json COMMAND test_json) 