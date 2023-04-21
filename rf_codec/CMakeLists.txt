add_library(rf_codec STATIC
    utils.c
    bit_array.c
    hamming_codec.c
    ccitt_whitening.c
    gf.c
    rs_codec.c
)

target_include_directories(rf_codec PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
