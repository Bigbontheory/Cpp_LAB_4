cmake_minimum_required(VERSION 3.14)

project("Cpp_Lab_4" CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ordinal INTERFACE)
target_include_directories(ordinal INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(Cpp_Lab_4 "src/main.cpp" "src/ordinal_print.cpp")
target_link_libraries(Cpp_Lab_4 PRIVATE ordinal)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests 
    "tests/ordinal_tests.cpp" 
    "tests/function_generator_tests.cpp" 
    "tests/recurrent_generator_tests.cpp"   
    "tests/append_test.cpp"  
    "tests/lazy_seq_tests.cpp" 
    "tests/map_generator_tests.cpp" 
    "tests/concat_tests.cpp" 
    "tests/filter_generator_tests.cpp" 
    "tests/sequence_generator.cpp" 
    "tests/insert_at_tests.cpp"
    "tests/prepend_generator_tests.cpp" 
    "tests/insert_at_seq_generator_tests.cpp" 
    "tests/sequence_stream_tests.cpp" 
    "tests/lazy_output_tests.cpp" 
    "tests/lazy_input_tests.cpp" 
 "tests/statistics_tests.cpp")

target_link_libraries(tests PRIVATE ordinal gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)