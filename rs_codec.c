//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#include "rs_codec.h"

void rs_init() {
    gf_init();
}

void rs_generator_poly(uint8_t nsym, uint8_t * g, uint16_t * len_g) {
    g[0] = 1;
    *len_g = 1;
    
    uint8_t q[2];
    uint16_t len_q = 2;
    
    for (uint16_t i = 0; i < nsym; i++) {
        q[0] = 1;
        q[1] = gf_pow(2, i);
        gf_poly_mult(g, q, *len_g, len_q, g, len_g);
    }
}

void rs_encode(uint8_t nsym, uint8_t * msg_in, uint16_t msg_in_len, uint8_t * msg_out, uint16_t * msg_out_len) {
    
    uint16_t len_g;
    uint8_t g[2 * nsym];
    rs_generator_poly(nsym, g, &len_g);
    
    uint16_t msg_in_extended_len = msg_in_len + len_g - 1;
    uint8_t msg_in_extended[msg_in_extended_len];
    memset(msg_in_extended, 0, sizeof(msg_in_extended));
    
    uint16_t remainder_len;
    uint8_t remainder[len_g];
    memset(remainder, 0, sizeof(remainder));
    
    gf_poly_div(msg_in_extended, msg_in_extended_len, g, len_g, NULL, NULL, remainder, &remainder_len);

    memcpy(msg_out, msg_in, msg_in_len * sizeof(uint8_t));
    memcpy(msg_out + msg_in_len, remainder, remainder_len * sizeof(uint8_t));
}
