#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hamming_codec.h"
#include "ccitt_whitening.h"

int main(int argc, const char * argv[]) {
    int n = 4;
    int size = 3;
    uint8_t data[] = { 0b10001000, 0b01000100, 0b11001100 };
    uint8_t encoded[size * 4];
    memset(encoded, 0, sizeof(encoded));
    
    uint16_t encoded_length = hamming_encode(data, size, n, encoded);
    
    uint8_t data_decoded[3];
    memset(data_decoded, 0, sizeof(data_decoded));
    hamming_decode(encoded, encoded_length, n, data_decoded);
    
    return 0;
}
