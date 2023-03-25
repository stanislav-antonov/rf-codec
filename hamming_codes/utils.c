//
//  utils.c
//  Playground
//
//  Created by Stanislav Antonov on 21/03/2023.
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#include "utils.h"

// https://stackoverflow.com/questions/12240299/convert-bytes-to-int-uint-in-c
unsigned short utils_pack_byte_array(char* bytes, unsigned short* packed_bytes, unsigned short bytes_length) {
    unsigned short packed_data_length = 0;
    for (uint8_t i = 0; i < bytes_length; i += 2) {
        packed_bytes[packed_data_length] = (uint8_t)bytes[i] | (bytes[i + 1] << 8);
        packed_data_length++;
    }
    
    return packed_data_length;
}
