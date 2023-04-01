//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#ifndef hamming_encode_h
#define hamming_encode_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bit_array.h"
#include "utils.h"

enum HammingDataWord {
    HAMMING3_1 = 1,   // hamming(3,1) or tripple repetition code
    HAMMING7_4 = 4,   // hamming(7,4)
    HAMMING15_11 = 11 // hamming(15,11)
};

struct HammingCodeWordParameters {
    uint8_t total_bits_count;
    uint8_t parity_bits_count;
    uint16_t parity_bits_index_mask;
};

uint16_t hamming_encode(uint8_t* data, uint16_t data_length, enum HammingDataWord dw_bits_count, uint8_t* encoded);

#endif
