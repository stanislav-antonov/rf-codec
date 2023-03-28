#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hamming_codec.h"
#include "ccitt_whitening.h"

int main(int argc, const char * argv[]) {
    /*
    int n = 4;
    int size = 5;
    char data[] = { 0b10111011, 0b10111011, 0b10111011, 0b10111011, 0b10111011 };
    char encoded[size * 3];
    
    hamming_encode(data, size, n, encoded);
    
    printf("data: %s\n", data);
    printf("code words: %s\n", encoded);
    */
    
    uint8_t data[] = {  0x11, 0x22, 0x33, 0x44 };
    
    ccitt_whitening(data, 4);
    
    return 0;
}
