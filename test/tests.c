#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "unity.h"
#include "ccitt_whitening.h"
#include "hamming_codec.h"
#include "bit_array.h"

void setUp(void) {}

void tearDown(void) {}

void ccitt_whitening_test() {
    uint8_t data[] = { 0x11, 0x22, 0x33, 0x44 };
    ccitt_whitening(data, 4);
    uint8_t expected[] = { 0xEE, 0xA5, 0x8B, 0x1D };
    UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, data, 4, __LINE__, NULL);
}

void hamming_encoding_test() {
    uint8_t data_length = 3;
    uint8_t dw_bits_count = 4;
    uint8_t encoded_data[data_length * 4];
    memset(encoded_data, 0, sizeof(encoded_data));
    uint8_t source_data[] = { 0x88, 0x44, 0xCC };
    
    uint16_t encoded_data_length = hamming_encode(source_data, data_length, dw_bits_count, encoded_data);
    UNITY_TEST_ASSERT_EQUAL_INT16(6, encoded_data_length, __LINE__, NULL);
    
    uint8_t expected[] = { 0x70, 0x38, 0x93, 0xC9, 0xE3, 0x01 };
    UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, encoded_data, encoded_data_length, __LINE__, NULL);
}

void hamming_decoding_test() {
    uint8_t data_length = 6;
    uint8_t dw_bits_count = 4;
    uint8_t decoded_data[data_length];
    memset(decoded_data, 0, sizeof(decoded_data));
    uint8_t source_data[] = { 0x70, 0x38, 0x93, 0xC9, 0xE3, 0x01 };
    
    uint16_t decoded_data_length = hamming_decode(source_data, data_length, dw_bits_count, decoded_data);
    UNITY_TEST_ASSERT_EQUAL_INT16(3, decoded_data_length, __LINE__, NULL);
    
    uint8_t expected[] = { 0x88, 0x44, 0xCC };
    UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, decoded_data, decoded_data_length, __LINE__, NULL);
}

void hamming_encoding_decoding_no_errors_test() {
    uint8_t data_length = 7;
    uint8_t dw_bits_count = 4;
    uint8_t encoded_data[data_length * 4];
    memset(encoded_data, 0, sizeof(encoded_data));
    uint8_t source_data[] = { 0x21, 0x84, 0xDF, 0x32, 0x54, 0x12, 0x77 };
    
    hamming_encode(source_data, data_length, dw_bits_count, encoded_data);
    
    data_length = 6;
    uint8_t decoded_data[data_length];
    memset(decoded_data, 0, sizeof(decoded_data));
    
    uint16_t decoded_data_length = hamming_decode(encoded_data, data_length, dw_bits_count, decoded_data);
    
    UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(source_data, decoded_data, decoded_data_length, __LINE__, NULL);
}

int main(int argc, const char * argv[]) {
    UNITY_BEGIN();
    
    ccitt_whitening_test();
    hamming_encoding_test();
    hamming_decoding_test();
    hamming_encoding_decoding_no_errors_test();
    
    return UNITY_END();
}

