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

// synd_len = nsym + 1
void rs_calc_syndromes(uint8_t * msg, uint16_t msg_len, uint8_t nsym, uint8_t * synd, uint8_t * synd_len) {
    synd[0] = 0;
    for (uint8_t i = 0; i < nsym; i++) {
        synd[i + 1] = gf_poly_eval(msg, msg_len, gf_pow(2, i));
    }
    
    *synd_len = nsym + 1;
}

bool rs_message_is_corrupted(uint8_t * msg, uint16_t msg_len, uint8_t nsym) {
    uint8_t synd_len = nsym + 1;
    uint8_t synd[synd_len];
    memset(synd, 0, sizeof(synd));
    
    rs_calc_syndromes(msg, msg_len, nsym, synd, &synd_len);
    uint8_t res = utils_max_array_8(synd, synd_len);
    
    return res > 0;
}

void rs_find_error_locator(uint8_t nsym, uint8_t * synd, uint8_t synd_len, uint8_t * err_loc, uint16_t * err_loc_len) {
    
    err_loc[0] = 1; // errors locator polynomial
    *err_loc_len = 1;
    
    uint8_t old_loc[synd_len];
    memset(old_loc, 0, sizeof(old_loc));
    
    old_loc[0] = 1;
    uint8_t old_loc_len = 1;
    
    uint8_t synd_shift = synd_len - nsym;
    
    for (uint8_t i = 0; i < nsym; i++) {
        
        old_loc_len++;
        
        uint8_t k = i + synd_shift;
        uint8_t delta = synd[k];
        for (uint8_t j = 1; j < *err_loc_len; j++) {
            uint8_t aa = err_loc[*err_loc_len - (j + 1)];
            uint8_t bb = synd[k - j];
            uint8_t cc = gf_mult(aa, bb);
            delta ^= cc;
        }
        
        if (delta == 0) {
            continue;
        }
        
        if (old_loc_len > *err_loc_len) {
            uint8_t new_loc[old_loc_len];
            memset(new_loc, 0, sizeof(new_loc));
            gf_poly_scale(old_loc, old_loc_len, delta, new_loc);
            
            memset(old_loc, 0, sizeof(old_loc));
            gf_poly_scale(err_loc, *err_loc_len, gf_inverse(delta), old_loc);
            
            uint8_t old_loc_len_tmp = old_loc_len;
            old_loc_len = *err_loc_len;
            
            memset(err_loc, 0, sizeof(*err_loc));
            memcpy(err_loc, new_loc, old_loc_len_tmp * sizeof(uint8_t));
            
            *err_loc_len = old_loc_len_tmp;
        }
        
        uint8_t old_loc_scaled[old_loc_len];
        memset(old_loc_scaled, 0, sizeof(old_loc_scaled));
        gf_poly_scale(old_loc, old_loc_len, delta, old_loc_scaled);
        
        uint8_t err_loc_result[*err_loc_len];
        memset(err_loc_result, 0, sizeof(err_loc_result));
        gf_poly_add(err_loc, *err_loc_len, old_loc_scaled, old_loc_len, err_loc_result, err_loc_len);
        
        memset(err_loc, 0, sizeof(*err_loc));
        memcpy(err_loc, err_loc_result, *err_loc_len * sizeof(uint8_t));
    }

}
