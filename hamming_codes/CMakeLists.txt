add_library(hamming_codes STATIC
    utils.c
    bit_array.c
    hamming_encode.c
)

target_include_directories(hamming_codes PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
