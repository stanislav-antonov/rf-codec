//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#include "hamming_codec.h"

uint8_t calculate_parity_bit(uint16_t cw, struct HammingCodeWordParameters cw_parameters, uint8_t parity_bit_position) {
    
    uint8_t parity_bit = 0;
    
    for (uint8_t cw_bit_index = 0; cw_bit_index < cw_parameters.total_bits_count; cw_bit_index++) {
        uint8_t is_parity_bit_index = ((cw_parameters.parity_bits_index_mask >> cw_bit_index) & 1);
        if (!is_parity_bit_index) {
            // this is a data bit
            uint8_t cw_bit_position = cw_bit_index + 1;
            if ((cw_bit_position & parity_bit_position))
            {
                uint8_t cw_data_bit = utils_get_bit(cw, (cw_parameters.total_bits_count - 1) - cw_bit_index);
                parity_bit ^= cw_data_bit;
            }
        }
    }
    
    return parity_bit;
}

struct HammingCodeWordParameters hamming_calculate_cw_parameters(enum HammingDataWord data_word_length) {
    struct HammingCodeWordParameters parameters = { 0, 0, 0 };
    uint8_t parity_bit_index = 0;
    
    // calculate codeword length (cw_length) and parity bits number (parity_bits_number)
    // based on given data bits number (data_word_length)
    while (1) {
        parity_bit_index = (1 << parameters.parity_bits_count) - 1; // parity_bit_position = 2 ^ r
        parameters.total_bits_count = data_word_length + parameters.parity_bits_count;
        if (parity_bit_index >= parameters.total_bits_count) {
            break;
        }
        
        // raise a bit to mark parity bit positions at each codeword
        parameters.parity_bits_index_mask |= 1 << parity_bit_index;
        parameters.parity_bits_count++;
    }
    
    return parameters;
}

uint16_t hamming_encode(uint8_t* data, uint16_t data_length, enum HammingDataWord dw_bits_count, uint8_t* encoded) {
    
    uint16_t data_bits_count = 8 * data_length;
    uint16_t data_words_count = utils_div_ceil(data_bits_count, dw_bits_count);
    
    struct HammingCodeWordParameters cw_parameters = hamming_calculate_cw_parameters(dw_bits_count);
    
    uint16_t code_words_bit_count = data_words_count * cw_parameters.total_bits_count;
    
    for (uint16_t dw_offset = 0, cw_offset = 0;
         dw_offset < data_bits_count;
         dw_offset += dw_bits_count, cw_offset += cw_parameters.total_bits_count
         ) {
        
        uint8_t dw_bit_index = 0;
        uint16_t cw = 0;
        uint16_t dw = bit_array_get_bits_8(data, dw_offset, dw_bits_count);
        
        // Start composing hamming codewords:
        // scatter the data and parity bit placeholders on proper positions across Hamming code words
        for (uint8_t cw_bit_index = 0; cw_bit_index < cw_parameters.total_bits_count; cw_bit_index++) {
            
            // check whether a bit is set (1) for the given cw_bit_index position
            // at the mask, thus to determine all the data bit positions
            uint8_t is_parity_bit_index = utils_bit_is_set(cw_parameters.parity_bits_index_mask, cw_bit_index);
            
            if (!is_parity_bit_index) {
                uint8_t dw_bit = utils_get_bit(dw, ((dw_bits_count - 1) - dw_bit_index));
                utils_set_bit(&cw, ((cw_parameters.total_bits_count - 1) - cw_bit_index), dw_bit);
                dw_bit_index++;
            }
        }
        
        // Calculate parity bits and place them to proper positions in the code words
        for (uint8_t i = 0; i < cw_parameters.parity_bits_count; i++) {
            uint8_t parity_bit_position = (1 << i);
            uint8_t parity_bit = calculate_parity_bit(cw, cw_parameters, parity_bit_position);
            uint8_t parity_bit_index = cw_parameters.total_bits_count - parity_bit_position;
            
            utils_set_bit(&cw, parity_bit_index, parity_bit);
        }
        
        bit_array_append_bits_8(encoded, cw_offset, cw);
    }
    
    // bit_array_print_as_bits_8(data, data_bits_count);
    // bit_array_print_as_bits_8(encoded, code_words_bit_count);
    
    return utils_div_ceil(code_words_bit_count, 8);
}

uint16_t hamming_decode(uint8_t* data, uint16_t data_length, enum HammingDataWord dw_bits_count, uint8_t* decoded) {
    
    struct HammingCodeWordParameters cw_parameters = hamming_calculate_cw_parameters(dw_bits_count);
    
    uint16_t dw_offset = 0;
    uint16_t cw_bits_count = ((8 * data_length) % cw_parameters.total_bits_count) * cw_parameters.total_bits_count;
    
    for (uint16_t cw_offset = 0; cw_offset < cw_bits_count; cw_offset += cw_parameters.total_bits_count
         ) {
        
        uint16_t error_bit_index = 0;
        uint16_t cw = bit_array_get_bits_8(data, cw_offset, cw_parameters.total_bits_count);
        
        // Calculate parity bits and place them to proper positions in the code words
        for (uint8_t i = 0; i < cw_parameters.parity_bits_count; i++) {
            uint8_t parity_bit_position = (1 << i);
            uint8_t parity_bit = calculate_parity_bit(cw, cw_parameters, parity_bit_position);
            uint8_t parity_bit_index = cw_parameters.total_bits_count - parity_bit_position;
            
            if (parity_bit != utils_get_bit(cw, parity_bit_index)) {
                error_bit_index += parity_bit_index;
            }
        }
        
        if (error_bit_index != 0) {
            // correct the errors
        }
        
        uint16_t dw = 0;
        uint8_t dw_bit_index = 0;
        
        for (uint8_t cw_bit_index = 0; cw_bit_index < cw_parameters.total_bits_count; cw_bit_index++) {
            
            // check whether a bit is set (1) for the given cw_bit_index position
            // at the mask, thus to determine all the data bit positions
            uint8_t is_parity_bit_index = utils_bit_is_set(cw_parameters.parity_bits_index_mask, cw_bit_index);
            
            if (!is_parity_bit_index) {
                uint8_t dw_bit = utils_get_bit(cw, ((cw_parameters.total_bits_count - 1) - cw_bit_index));
                utils_set_bit(&dw, ((dw_bits_count - 1) - dw_bit_index), dw_bit);
                dw_bit_index++;
            }
        }
        
        bit_array_append_bits_8(decoded, dw_offset, dw);
        dw_offset += dw_bits_count;
    }
    
    return utils_div_ceil(dw_offset, 8);
}
