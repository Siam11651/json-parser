cmake_minimum_required(VERSION 3.5.0)
project(json-parser VERSION 0.1.0 LANGUAGES C CXX)
enable_testing()
add_library(json-parser
    "src/array.cpp"
    "src/number.cpp"
    "src/object.cpp"
    "src/parser.cpp"
    "src/string.cpp"
    "src/value.cpp")
target_include_directories(json-parser PUBLIC "include")
set_property(TARGET json-parser PROPERTY CXX_STANDARD 20)

add_executable(test-0 "tests/test-0.cpp")
target_include_directories(test-0 PUBLIC "include")
target_link_libraries(test-0 json-parser)
set_property(TARGET test-0 PROPERTY CXX_STANDARD 20)
add_test(NAME test-0 COMMAND test-0)