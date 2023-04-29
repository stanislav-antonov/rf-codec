#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hamming_codec.h"
#include "ccitt_whitening.h"
#include "rs_codec.h"

int main(int argc, const char * argv[]) {
    uint16_t msg_original_len = 16;
    uint8_t msg_original[] = {0x40, 0xd2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06, 0x27, 0x26, 0x96, 0xc6, 0xc6, 0x96, 0x70, 0xec};
    
    uint8_t nsym = 10;
    uint16_t msg_encoded_len = nsym + msg_original_len;
    uint8_t msg_encoded[msg_encoded_len];
    memset(msg_encoded, 0, sizeof(msg_encoded));
    
    rs_init();
    rs_encode(nsym, msg_original, msg_original_len, msg_encoded, &msg_encoded_len);
    
    printf("msg_out encoded:\n");
    utils_print_array_8(msg_encoded, msg_encoded_len);
    
    msg_encoded[1] = 44;
    msg_encoded[2] = 55;
    msg_encoded[3] = 66;
    
    printf("msg_out corrupted:\n");
    utils_print_array_8(msg_encoded, msg_encoded_len);
    
    rs_correct_msg(msg_encoded, &msg_encoded_len, 10);
    
    printf("msg_out corrected:\n");
    utils_print_array_8(msg_encoded, msg_encoded_len);
    
    return 0;
}
