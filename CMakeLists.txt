cmake_minimum_required(VERSION 3.5)
project(kvdb-c-plus-plus CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories(src/include)
include_directories(tests)

add_executable(kvdb-c-plus-plus 
    src/main.cpp
)

add_executable(test_index 
    tests/index/test_index.cpp
    tests/unit_test_framework.cpp
    src/index/index.cpp
    src/data/log_record.cpp
)

enable_testing()
add_test(NAME test_index COMMAND test_index)
