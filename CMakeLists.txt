cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(hamming_codes)

add_library(hamming_codes SHARED src/bit_array.c src/hamming_encode.c src/utils.c)

include(CTest)

add_test(tests ${CMAKE_CTEST_COMMAND}
    --build-and-test "${CMAKE_SOURCE_DIR}/tests"
                     "${CMAKE_BINARY_DIR}/tests"
    --build-generator ${CMAKE_GENERATOR}
    --build-makeprogram ${CMAKE_MAKE_PROGRAM}
    --build-project hamming_codes
    --test-command tests)


