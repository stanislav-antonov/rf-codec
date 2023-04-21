#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hamming_codec.h"
#include "ccitt_whitening.h"
#include "rs_codec.h"

int main(int argc, const char * argv[]) {
    /*
    int n = 4;
    int size = 3;
    uint8_t data[] = { 0b10001000, 0b01000100, 0b11001100 };
    uint8_t encoded[size * 4];
    memset(encoded, 0, sizeof(encoded));
    
    uint16_t encoded_length = hamming_encode(data, size, n, encoded);
    
    uint8_t data_decoded[3];
    memset(data_decoded, 0, sizeof(data_decoded));
    hamming_decode(encoded, encoded_length, n, data_decoded);
    */
    
    uint8_t msg_in[] = {0x40, 0xd2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06, 0x27, 0x26, 0x96, 0xc6, 0xc6, 0x96, 0x70, 0xec};
    
    uint16_t msg_in_len = 16;
    
    uint8_t nsym = 10;
    uint16_t msg_out_len = nsym + msg_in_len;
    uint8_t msg_out[msg_out_len];
    
    rs_init();
    rs_encode(nsym, msg_in, msg_in_len, msg_out, &msg_out_len);
    
    return 0;
}
