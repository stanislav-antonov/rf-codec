//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#include "utils.h"

uint16_t utils_pack_byte_array(char* bytes, uint16_t* packed_bytes, uint16_t bytes_length) {
    uint16_t packed_data_length = 0;
    for (uint8_t i = 0; i < bytes_length; i += 2) {
        packed_bytes[packed_data_length] = (uint8_t)bytes[i] | (bytes[i + 1] << 8);
        packed_data_length++;
    }
    
    return packed_data_length;
}
