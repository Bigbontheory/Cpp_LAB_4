cmake_minimum_required(VERSION 3.14)

project("Cpp_Lab_4" CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем библиотеку для заголовочных файлов из папки src
add_library(ordinal INTERFACE)
target_include_directories(ordinal INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Загрузка и подключение Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Собираем исполняемый файл тестов со всеми исходниками
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
 "tests/prepend_generator_tests.cpp" "tests/insert_at_seq_generator_tests.cpp" "tests/sequence_stream_tests.cpp" "tests/lazy_output_tests.cpp" "tests/lazy_input_tests.cpp" "tests/replace_at_generator_tests.cpp")

# Связываем тесты с нашей библиотекой и gtest
target_link_libraries(tests PRIVATE ordinal gtest_main)

# Включаем автоматический поиск тестов для тест-раннера
include(GoogleTest)
gtest_discover_tests(tests)