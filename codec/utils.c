//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#include "utils.h"

uint16_t utils_pack(char* bytes, uint16_t length, uint16_t* packed) {
    uint16_t packed_length = 0;
    for (uint8_t i = 0; i < length; i += 2) {
        packed[packed_length] = (uint8_t)bytes[i] | (bytes[i + 1] << 8);
        packed_length++;
    }
    
    return packed_length;
}
