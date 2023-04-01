//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#include "bit_array.h"

#define bit_array_get_bit(array, bit_index, n) ({ \
    uint8_t retval = ((array[bit_index / n] & (1 << (bit_index % n)))) != 0; \
    retval; \
}) \

void bit_array_set_bit_16(uint16_t array[], uint8_t bit_index) {
    array[bit_index / 16] |= 1 << (bit_index % 16);
}

void bit_array_clear_bit_16(uint16_t array[], uint8_t bit_index) {
    array[bit_index / 16] &= ~(1 << (bit_index % 16));
}

uint8_t bit_array_get_bit_16(uint16_t array[], uint8_t bit_index) {
    return bit_array_get_bit(array, bit_index, 16);
}

uint8_t bit_array_get_bit_8(uint8_t array[], uint8_t bit_index) {
    return bit_array_get_bit(array, bit_index, 8);
}

uint16_t bit_array_get_bits_16(uint16_t array[], uint8_t bit_index_offset, uint8_t bits_count) {
    uint16_t result = 0;
    for (uint8_t bit_index = bit_index_offset + bits_count - 1; ; bit_index--) {
        uint8_t bit_value = bit_array_get_bit_16(array, bit_index);
        result = (result << 1) | bit_value;
        if (bit_index == bit_index_offset) {
            break;
        }
    }
    
    return result;
}

void bit_array_append_bits_16(uint16_t array[], uint16_t length, uint16_t bits) {
    uint16_t index = length / 16;
    uint16_t offset = length % 16;
    
    array[index] |= bits << offset;
    if (offset) {
        array[index + 1] |= bits >> (16 - offset);
    }
}
