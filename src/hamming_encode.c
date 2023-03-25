//
//  hamming_encode.c
//  Playground
//
//  Created by Stanislav Antonov on 21/03/2023.
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#include "../include/hamming_encode.h"

uint8_t calculate_parity_bit(uint16_t cw, uint8_t cw_length, uint16_t parity_bits_index_mask, uint8_t parity_bit_position) {
    uint8_t parity_bit = 0;
    
    for (uint8_t cw_bit_index = 0; cw_bit_index < cw_length; cw_bit_index++) {
        uint8_t is_parity_bit_index = ((parity_bits_index_mask >> cw_bit_index) & 1);
        if (!is_parity_bit_index) {
            // this is a data bit
            uint8_t cw_bit_position = cw_bit_index + 1;
            if ((cw_bit_position & parity_bit_position))
            {
                uint8_t cw_data_bit = (cw & (1 << cw_bit_index)) >> cw_bit_index;
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
        parity_bit_index = (1 << parameters.parity_bits_number) - 1; // parity_bit_position = 2 ^ r
        parameters.codeword_length = data_word_length + parameters.parity_bits_number;
        if (parity_bit_index >= parameters.codeword_length) {
            break;
        }
        
        // raise a bit to mark parity bit positions at each codeword
        parameters.parity_bits_index_mask |= 1 << parity_bit_index;
        parameters.parity_bits_number++;
    }
    
    return parameters;
}


void hamming_encode(char* data, char* encoded, uint16_t data_length, enum HammingDataWord dw_bits_count) {
    uint16_t code_words[data_length];
    uint16_t data_words[(data_length / 2) + 1];
    uint16_t data_words_count = utils_pack_byte_array(data, data_words, data_length);
    
    for (int i = 0; i < data_words_count; i++) {
        printf("dw: %hu\n", data_words[i]);
    }
    
    struct HammingCodeWordParameters cw_parameters = hamming_calculate_cw_parameters(dw_bits_count);
    
    for (uint8_t dw_offset = 0, cw_offset = 0; dw_offset < data_words_count * 16; dw_offset += dw_bits_count, cw_offset += cw_parameters.codeword_length) {
        uint16_t cw = 0;
        
        uint8_t dw_bit_index = 0;
        uint16_t dw = bit_array_get_bits(data_words, dw_offset, dw_bits_count);
        
        // Start composing hamming codewords:
        // scatter the data and parity bit placeholders on proper positions across Hamming code words
        for (uint8_t cw_bit_index = cw_parameters.codeword_length; cw_bit_index-- > 0;) {
            // check whether a bit is set (1) for the given cw_bit_index position
            // at the mask, thus to determine all the data bit positions
            uint8_t is_parity_bit_index = ((cw_parameters.parity_bits_index_mask >> cw_bit_index) & 1);
            if (!is_parity_bit_index) {
                uint8_t dw_bit = (dw & (1 << dw_bit_index)) >> dw_bit_index;
                cw = (cw & ~(1 << cw_bit_index)) | (dw_bit << cw_bit_index);
                dw_bit_index++;
            }
        }
        
        // Calculate parity bits and place them to proper positions in the code words
        for (uint8_t i = cw_parameters.parity_bits_number; i-- > 0;) {
            uint8_t parity_bit_position = (1 << i);
            uint8_t parity_bit = calculate_parity_bit(cw, cw_parameters.codeword_length,
                                                      cw_parameters.parity_bits_index_mask, parity_bit_position);
            
            uint8_t parity_bit_index = parity_bit_position - 1;
            cw = (cw & ~(1 << parity_bit_index)) | (parity_bit << parity_bit_index);
        }
        
        for (uint8_t cw_bit_index = 0; cw_bit_index < cw_parameters.codeword_length; cw_bit_index++) {
            uint8_t cw_bit = (cw & (1 << cw_bit_index)) >> cw_bit_index;
            if (cw_bit) {
                bit_array_set_bit(code_words, cw_offset + cw_bit_index);
            }
        }
    }
    
    for (int i = 0; i < data_length; i++) {
        printf("cw: %hu\n", code_words[i]);
    }
}
