cmake_minimum_required(VERSION 3.10)
project(HexDump)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Библиотека (ДОБАВЛЯЕМ getopt.c)
add_library(hex_dumper STATIC src/hex_dumper.c src/getopt.c)
target_include_directories(hex_dumper PUBLIC include)

# Основная программа
add_executable(hexdump src/main.c)
target_link_libraries(hexdump hex_dumper)
target_include_directories(hexdump PRIVATE include)

# Тесты
enable_testing()
add_executable(test_hex_dumper tests/test_hex_dumper.c)
target_link_libraries(test_hex_dumper hex_dumper)
target_include_directories(test_hex_dumper PRIVATE include)
add_test(NAME HexDumperTests COMMAND test_hex_dumper)