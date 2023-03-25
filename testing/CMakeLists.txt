cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

project(hamming_codes_testing)

add_executable(tests ../src/bit_array.c ../src/hamming_encode.c ../src/utils.c tests.c munit.c)

