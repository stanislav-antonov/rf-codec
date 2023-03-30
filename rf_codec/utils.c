//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#include "utils.h"

uint16_t utils_pack(char* data, uint16_t length, uint16_t* packed) {
    uint16_t packed_length = 0;
    for (uint8_t i = 0, j = 0; i < length; i += 2, j++) {
        packed[j] = (uint8_t)data[i] | (data[i + 1] << 8);
        ++packed_length;
    }
    
    return packed_length;
}

uint16_t utils_unpack(uint16_t* data, uint16_t length, char* unpacked) {
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

void print_array(uint16_t *array, int n) {
    printf("[");
    for (uint16_t i = 0; i < n; i++) {
        if (i == (n-1)) {
            printf("%d]\n", array[i]);
        } else {
            printf("%d, ", array[i]);
        }
    }
}
