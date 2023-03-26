//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#include "bit_array.h"

void bit_array_set_bit(uint16_t array[], uint8_t bit_index) {
    array[bit_index / 16] |= 1 << (bit_index % 16);
}

uint8_t bit_array_get_bit(uint16_t array[], uint8_t bit_index) {
    return ((array[bit_index / 16] & (1 << (bit_index % 16)))) != 0;
}

uint16_t bit_array_get_bits(uint16_t array[], uint8_t bit_index_offset, uint8_t bits_number) {
    uint16_t result = 0;
    for (uint8_t bit_index = bit_index_offset + bits_number - 1; ; bit_index--) {
        uint8_t bit_value = bit_array_get_bit(array, bit_index);
        result = (result << 1) | bit_value;
        if (bit_index == bit_index_offset) {
            break;
        }
    }
    
    return result;
}

