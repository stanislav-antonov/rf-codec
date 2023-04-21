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
    
    uint16_t len_q = 2;
    uint8_t q[len_q];
    
    uint16_t len_gg = nsym * 2;
    uint8_t gg[len_gg];
    
    for (uint16_t i = 0; i < nsym; i++) {
        q[0] = 1;
        q[1] = gf_pow(2, i);
        memset(gg, 0, sizeof(gg));
        
        gf_poly_mult(g, *len_g, q, len_q, gg, &len_gg);
        
        memcpy(g, gg, len_gg * sizeof(uint8_t));
        *len_g = len_gg;
    }
}

void rs_encode(uint8_t nsym, uint8_t * msg_in, uint16_t msg_in_len, uint8_t * msg_out, uint16_t * msg_out_len) {
    
    uint16_t len_g = 2 * nsym;
    uint8_t g[len_g];
    memset(g, 0, sizeof(g));
    rs_generator_poly(nsym, g, &len_g);
    
    uint16_t msg_in_extended_len = msg_in_len + len_g - 1;
    uint8_t msg_in_extended[msg_in_extended_len];
    memset(msg_in_extended, 0, sizeof(msg_in_extended));
    memcpy(msg_in_extended, msg_in, sizeof(uint8_t) * msg_in_len);
    
    uint16_t r_len;
    uint8_t r[len_g];
    memset(r, 0, sizeof(r));
    
    gf_poly_div(msg_in_extended, msg_in_extended_len, g, len_g, NULL, NULL, r, &r_len);

    memcpy(msg_out, msg_in, msg_in_len * sizeof(uint8_t));
    memcpy(msg_out + msg_in_len, r, r_len * sizeof(uint8_t));
    *msg_out_len = msg_in_len + r_len;
}
