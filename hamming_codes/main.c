#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hamming_encode.h"

int main(int argc, const char * argv[]) {
    int n = 4;
    int size = 5;
    char data[] = { 0b10111011, 0b10111011, 0b10111011, 0b10111011, 0b10111011 };
    char code_words[size * 3];
    
    hamming_encode(data, code_words, size, n);
    
    printf("data: %s\n", data);
    printf("code words: %s\n", code_words);
    
    // char *cw = "0010111";
    // char *cw = "001011101111110";
    // int p = hamming_parity(15, cw);
    // printf("encoded: %d\n", p);
    
    return 0;
}
