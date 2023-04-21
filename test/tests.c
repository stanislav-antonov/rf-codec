#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "unity.h"
#include "ccitt_whitening.h"
#include "hamming_codec.h"
#include "bit_array.h"
#include "gf.h"
#include "rs_codec.h"

void setUp(void) {}

void tearDown(void) {}

void ccitt_whitening_test() {
    uint8_t data[] = { 0x11, 0x22, 0x33, 0x44 };
    ccitt_whitening(data, 4);
    uint8_t expected[] = { 0xEE, 0xA5, 0x8B, 0x1D };
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, data, 4, NULL);
}

void hamming_encoding_test() {
    uint8_t data_length = 3;
    uint8_t dw_bits_count = 4;
    uint8_t encoded_data[data_length * 4];
    memset(encoded_data, 0, sizeof(encoded_data));
    uint8_t source_data[] = { 0x88, 0x44, 0xCC };
    
    uint16_t encoded_data_length = hamming_encode(source_data, data_length, dw_bits_count, encoded_data);
    TEST_ASSERT_EQUAL_UINT16(6, encoded_data_length);
    
    uint8_t expected[] = { 0x70, 0x38, 0x93, 0xC9, 0xE3, 0x01 };
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, encoded_data, encoded_data_length, NULL);
}

void hamming_decoding_test() {
    uint8_t data_length = 6;
    uint8_t dw_bits_count = 4;
    uint8_t decoded_data[data_length];
    memset(decoded_data, 0, sizeof(decoded_data));
    uint8_t source_data[] = { 0x70, 0x38, 0x93, 0xC9, 0xE3, 0x01 };
    
    uint16_t decoded_data_length = hamming_decode(source_data, data_length, dw_bits_count, decoded_data);
    TEST_ASSERT_EQUAL_UINT16(3, decoded_data_length);
    
    uint8_t expected[] = { 0x88, 0x44, 0xCC };
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, decoded_data, decoded_data_length, NULL);
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
    
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(source_data, decoded_data, decoded_data_length, NULL);
}

void galois_field_ops_test() {
    uint8_t mult_result = gf_mult(137, 42);
    TEST_ASSERT_EQUAL_UINT8(195, mult_result);
}

void reed_solomon_encoding_test() {
    uint16_t msg_in_len = 16;
    uint8_t msg_in[] = {0x40, 0xd2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06, 0x27, 0x26, 0x96, 0xc6, 0xc6, 0x96, 0x70, 0xec};
    
    uint8_t nsym = 10;
    uint8_t msg_out_len_expected = nsym + msg_in_len;
    uint8_t msg_out[msg_out_len_expected];
    memset(msg_out, 0, sizeof(msg_out));
    
    uint8_t msg_out_expected[] = {0x40, 0xd2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06, 0x27, 0x26, 0x96, 0xc6, 0xc6, 0x96, 0x70, 0xec, 0xbc, 0x2a, 0x90, 0x13, 0x6b, 0xaf, 0xef, 0xfd, 0x4b, 0xe0 };
    
    uint16_t msg_out_len;
    rs_encode(nsym, msg_in, msg_in_len, msg_out, &msg_out_len);
    
    TEST_ASSERT_EQUAL_UINT8(msg_out_len_expected, msg_out_len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(msg_out_expected, msg_out, msg_out_len, NULL);
}

int main(int argc, const char * argv[]) {
    UNITY_BEGIN();
    
    ccitt_whitening_test();
    hamming_encoding_test();
    hamming_decoding_test();
    hamming_encoding_decoding_no_errors_test();
    
    rs_init();
    
    galois_field_ops_test();
    reed_solomon_encoding_test();
    
    return UNITY_END();
}

