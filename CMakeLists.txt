cmake_minimum_required(VERSION 3.14)

if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,EditAndContinue,ProgramDatabase>")
endif()

project("Cpp_Lab_4" CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ordinal INTERFACE)
target_include_directories(ordinal INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests "tests/ordinal_tests.cpp" "tests/function_generator_tests.cpp")

target_link_libraries(tests PRIVATE ordinal gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)