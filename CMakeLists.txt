cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

project(hamming_codes)

add_library(hamming_codes SHARED src/bit_array.c src/hamming_encode.c src/utils.c)

install(TARGETS hamming_codes DESTINATION lib)
install(FILES src/bit_array.h src/hamming_encode.h src/utils.h DESTINATION lib)


