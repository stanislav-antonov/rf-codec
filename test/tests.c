#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "unity.h"
#include "ccitt_whitening.h"

void setUp(void) {}

void tearDown(void) {}

void ccitt_whitening_test() {
    uint8_t data[] = { 0x11, 0x22, 0x33, 0x44 };
    ccitt_whitening(data, 4);
    uint8_t expected[] = { 0xEE, 0xA5, 0x8B, 0x1D };
    UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(data, expected, 4, __LINE__, NULL);
}

void hamming_encoding_test() {
    uint8_t data[] = { 0x11, 0x22, 0x33, 0x44 };
    ccitt_whitening(data, 4);
    uint8_t expected[] = { 0xEE, 0xA5, 0x8B, 0x1D };
    UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(data, expected, 4, __LINE__, NULL);
}


int main(int argc, const char * argv[]) {
    UNITY_BEGIN();
    
    ccitt_whitening_test();
    
    return UNITY_END();
}

