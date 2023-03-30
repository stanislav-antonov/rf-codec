#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hamming_codec.h"
#include "ccitt_whitening.h"

int main(int argc, const char * argv[]) {
    
    int n = 4;
    int size = 3;
    char data[] = { 0b10001000, 0b01000100, 0b11001100 };
    char encoded[size * 3];
    
    uint16_t count = hamming_encode(data, size, n, encoded);
    
    for(uint8_t i = 0; i < count; i++) {
        printf("byte: %c\n", encoded[i]);
    }
    
    return 0;
}
