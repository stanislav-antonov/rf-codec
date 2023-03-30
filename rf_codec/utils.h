//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

uint16_t utils_pack(char* data, uint16_t length, uint16_t* packed);

uint16_t utils_unpack(uint16_t* data, uint16_t length, char* unpacked);

uint8_t utils_get_bit(uint16_t data, uint8_t index);

void utils_set_bit(uint16_t* data, uint8_t index, uint8_t bit);

uint8_t utils_bit_is_set(uint16_t data, uint8_t index);

void print_array(uint16_t *array, int n);

#endif
