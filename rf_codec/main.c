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
    
    uint16_t count = hamming_encode(data, size, n, encoded);
    
    return 0;
}
