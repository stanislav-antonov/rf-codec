//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#ifndef bit_array_h
#define bit_array_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void bit_array_set_bit(uint16_t array[], uint8_t bit_index);

void bit_array_clear_bit(uint16_t array[], uint8_t bit_index);

uint8_t bit_array_get_bit_16(uint16_t array[], uint8_t bit_index);

uint8_t bit_array_get_bit_8(uint8_t array[], uint8_t bit_index);

uint16_t bit_array_get_bits(uint16_t array[], uint8_t bit_index_offset, uint8_t bits_number);

void bit_array_append_bits(uint16_t array[], uint16_t length, uint16_t bits);

#endif
