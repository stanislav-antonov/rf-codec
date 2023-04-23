//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

uint16_t utils_pack(uint8_t* data, uint16_t length, uint16_t* packed);

uint16_t utils_unpack(uint16_t* data, uint16_t length, uint8_t* unpacked);

uint8_t utils_get_bit(uint16_t data, uint8_t index);

void utils_set_bit(uint16_t* data, uint8_t index, uint8_t bit);

uint8_t utils_bit_is_set(uint16_t data, uint8_t index);

void utils_print_array_16(uint16_t *array, uint16_t n);

void utils_print_array_8(uint8_t *array, uint16_t n);

uint16_t utils_div_ceil(uint16_t x, uint16_t y);

uint16_t utils_max(uint16_t m, uint16_t n);

uint8_t utils_max_array_8(uint8_t * array, uint8_t len);

#endif
