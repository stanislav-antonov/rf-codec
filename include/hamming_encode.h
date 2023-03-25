//
//  hamming_encode.h
//  Playground
//
//  Created by Stanislav Antonov on 21/03/2023.
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
    uint8_t codeword_length;
    uint8_t parity_bits_number;
    uint16_t parity_bits_index_mask;
};

void hamming_encode(char* data, char* encoded, uint16_t data_length, enum HammingDataWord data_word_length);

#endif
