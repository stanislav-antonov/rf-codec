//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#include "bit_array.h"

void bit_array_set_bit(uint16_t array[], uint8_t bit_index) {
    array[bit_index / 16] |= 1 << (bit_index % 16);
}

void bit_array_clear_bit(uint16_t array[], uint8_t bit_index) {
    array[bit_index / 16] &= ~(1 << (bit_index % 16));
}

uint8_t bit_array_get_bit_16(uint16_t array[], uint8_t bit_index) {
    return ((array[bit_index / 16] & (1 << (bit_index % 16)))) != 0;
}

uint8_t bit_array_get_bit_8(uint8_t array[], uint8_t bit_index) {
    return ((array[bit_index / 8] & (1 << (bit_index % 8)))) != 0;
}

uint16_t bit_array_get_bits(uint16_t array[], uint8_t bit_index_offset, uint8_t bits_number) {
    uint16_t result = 0;
    for (uint8_t bit_index = bit_index_offset + bits_number - 1; ; bit_index--) {
        uint8_t bit_value = bit_array_get_bit_16(array, bit_index);
        result = (result << 1) | bit_value;
        if (bit_index == bit_index_offset) {
            break;
        }
    }
    
    return result;
}

void bit_array_append_bits(uint16_t array[], uint16_t length, uint16_t bits) {
    uint16_t index = length / 16;
    uint16_t offset = length % 16;
    
    array[index] |= bits << offset;
    if (offset) {
        array[index + 1] |= bits >> (16 - offset);
    }
}
