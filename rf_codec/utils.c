//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#include "utils.h"

#define print_array(type_t, array, n) ({ \
    printf("["); \
    for (uint16_t i = 0; i < n; i++) { \
        if (i == (n - 1)) { \
            printf("%d]\n", (type_t)array[i]); \
        } else { \
            printf("%d, ", (type_t)array[i]); \
        } \
    } \
}) \

uint16_t utils_pack(uint8_t* data, uint16_t length, uint16_t* packed) {
    uint16_t packed_length = 0;
    for (uint8_t i = 0, j = 0; i < length; i += 2, j++) {
        packed[j] = (uint8_t)data[i] | (data[i + 1] << 8);
        ++packed_length;
    }
    
    return packed_length;
}

uint16_t utils_unpack(uint16_t* data, uint16_t length, uint8_t* unpacked) {
    for (uint8_t i = 0, j = 0; i < length; i++, j += 2) {
        unpacked[j] = data[i] & 0xff;
        unpacked[j + 1] = ((data[i] >> 8) & 0xff);
    }
    
    return length * 2;
}

uint8_t utils_get_bit(uint16_t data, uint8_t index) {
    return ((data & (1 << index)) >> index);
}

void utils_set_bit(uint16_t* data, uint8_t index, uint8_t bit) {
    *data = ((*data & ~(1 << index)) | (bit <<  index));
}

uint8_t utils_bit_is_set(uint16_t data, uint8_t index) {
    return ((data >> index) & 1);
}

uint16_t utils_div_ceil(uint16_t x, uint16_t y) {
    return 1 + ((x - 1) / y);
}

void utils_print_array_16(uint16_t *array, uint16_t n) {
    print_array(uint16_t, array, n);
}

void utils_print_array_8(uint8_t *array, uint16_t n) {
    print_array(uint8_t, array, n);
}

uint16_t utils_max(uint16_t m, uint16_t n) {
    return (m > n) ? m : n;
}
