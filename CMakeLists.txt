cmake_minimum_required(VERSION 3.5.0)
project(json-parser VERSION 0.1.0 LANGUAGES C CXX)

add_library(json-parser
    "src/number.cpp"
    "src/parser.cpp"
    "src/string.cpp"
    "src/value.cpp")
target_include_directories(json-parser PUBLIC "include")
set_property(TARGET json-parser PROPERTY CXX_STANDARD 20)

if(BUILD_TESTS)
    add_executable(json-parser-test-0
        "tests/test-0.cpp")
    target_include_directories(json-parser-test-0 PUBLIC "include")
    target_link_libraries(json-parser-test-0 json-parser)
    set_property(TARGET json-parser-test-0 PROPERTY CXX_STANDARD 20)
endif()