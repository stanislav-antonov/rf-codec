add_library(codec STATIC
    utils.c
    bit_array.c
    codec.c
)

target_include_directories(codec PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
