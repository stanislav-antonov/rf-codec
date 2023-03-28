//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#include "ccitt_whitening.h"

void ccitt_whitening(uint8_t* data, uint16_t length) {
    uint8_t whitening_key_msb = 0xFF;
    uint8_t whitening_key_lsb = 0xFF;  // initial value of the generator polynome GN9
    
    uint8_t whitening_key_lsb_result = whitening_key_lsb;
    uint8_t whitening_key_msb_previous = 0;
    
    for (uint16_t j = 0; j < length; j++) {
        data[j] ^= whitening_key_lsb_result;
        for (uint8_t i = 0; i < 8; i++) {
            whitening_key_msb_previous = whitening_key_msb;
            whitening_key_msb = ((whitening_key_lsb & 0x01) ^ ((whitening_key_lsb >> 5) & 0x01));
            whitening_key_lsb = (((whitening_key_msb_previous << 7) & 0x80) | ((whitening_key_lsb >> 1) & 0xFF));
        }
        
        whitening_key_lsb_result = (((whitening_key_lsb & 0xF0) >> 4) | ((whitening_key_lsb & 0x0F) << 4));
        whitening_key_lsb_result = (((whitening_key_lsb_result & 0xCC) >> 2) | ((whitening_key_lsb_result & 0x33) << 2));
        whitening_key_lsb_result = (((whitening_key_lsb_result & 0xAA) >> 1) | ((whitening_key_lsb_result & 0x55) << 1));
    }
}
