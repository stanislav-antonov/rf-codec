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

bool rs_find_error_locator(uint8_t nsym, uint8_t * synd, uint8_t synd_len, uint8_t * err_loc, uint16_t * err_loc_len) {
    
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
    
    uint16_t errors_count = *err_loc_len - 1;
    
    return !(errors_count * 2 > nsym);
}

bool rs_find_errors(uint8_t * error_loc, uint16_t err_loc_len, uint16_t msg_len, uint8_t * err_pos, uint16_t * err_pos_len) {
    uint8_t error_loc_copy[err_loc_len];
    memset(error_loc_copy, 0, sizeof(error_loc_copy));
    memcpy(error_loc_copy, error_loc, err_loc_len);
    utils_array_reverse(error_loc_copy, err_loc_len);
    
    *err_pos_len = 0;
    for (uint16_t i = 0; i < msg_len; i++) {
        uint8_t res = gf_poly_eval(error_loc_copy, err_loc_len, gf_pow(2, i));
        if (res == 0) {
            err_pos[(*err_pos_len)++] = msg_len - 1 - i;
        }
    }
    
    uint16_t errors_count = err_loc_len - 1;
    
    return (*err_pos_len == errors_count);
}

void rs_forney_syndromes(uint8_t * synd, uint8_t synd_len,
                         uint8_t * err_pos, uint16_t err_pos_len, uint16_t msg_len,
                         uint8_t * f_synd, uint16_t * f_synd_len) {
    
    uint8_t erase_pos[err_pos_len];
    memset(erase_pos, 0, sizeof(erase_pos));
    for (uint16_t i = 0; i < err_pos_len; i++) {
        uint8_t p = err_pos[i];
        erase_pos[i] = msg_len - 1 - p;
    }
    
    memcpy(f_synd, synd + 1, synd_len);
    *f_synd_len = synd_len;
    
    for (uint16_t i = 0; i < err_pos_len; i++) {
        uint8_t x = gf_pow(2, erase_pos[i]);
        for (uint16_t j = 0; j < synd_len - 1; j++) {
            f_synd[j] = gf_mult(f_synd[j], x) ^ f_synd[j + 1];
        }
    }
}

void rs_find_errata_locator(uint8_t * err_pos, uint8_t err_pos_len, uint8_t * err_loc, uint16_t * err_loc_len) {
    err_loc[0] = 1;
    *err_loc_len = 1;
    
    uint8_t p[] = { 1 };
    uint8_t q[] = { 0, 0 };

    uint16_t t_len = 2;
    uint8_t t[t_len];
    
    uint16_t err_loc_result_len = err_pos_len * 2;
    uint8_t err_loc_result[err_loc_result_len];
    
    for (uint8_t i = 0; i < err_pos_len; i++) {
        q[0] = gf_pow(2, err_pos[i]);
        q[1] = 0;
        
        memset(t, 0, sizeof(t));
        gf_poly_add(p, 1, q, 2, t, &t_len);
        
        memset(err_loc_result, 0, sizeof(err_loc_result));
        gf_poly_mult(err_loc, *err_loc_len, t, t_len, err_loc_result, &err_loc_result_len);
        
        memcpy(err_loc, err_loc_result, err_loc_result_len);
        *err_loc_len = err_loc_result_len;
    }
}

void rs_find_error_evaluator(uint8_t * synd, uint8_t synd_len,
                             uint8_t * err_loc, uint16_t err_loc_len, uint8_t nsym,
                             uint8_t * err_eval, uint16_t * err_eval_len) {
    
    uint16_t p_len = synd_len + err_loc_len + 1;
    uint8_t p[p_len];
    memset(p, 0, sizeof(p));
    gf_poly_mult(synd, synd_len, err_loc, err_loc_len, p, &p_len);
    
    uint16_t q_len = nsym + 2;
    uint8_t q[q_len];
    memset(q, 0, sizeof(q));
    q[0] = 1;
    
    gf_poly_div(p, p_len, q, q_len, NULL, NULL, err_eval, err_eval_len);
}

bool rs_correct_errata(uint8_t * msg, uint16_t msg_len,
                       uint8_t * synd, uint synd_len,
                       uint8_t * err_pos, uint16_t err_pos_len) {
    
    uint8_t coef_pos[err_pos_len];
    memset(coef_pos, 0, sizeof(coef_pos));
    for (uint16_t i = 0; i < err_pos_len; i++) {
        coef_pos[i] = msg_len - 1 - err_pos[i];
    }
    
    uint16_t err_loc_len = err_pos_len * 2;
    uint8_t err_loc[err_loc_len];
    memset(err_loc, 0, sizeof(err_loc));
    rs_find_errata_locator(coef_pos, err_pos_len, err_loc, &err_loc_len);
    
    uint8_t synd_copy[synd_len];
    memset(synd_copy, 0, sizeof(synd_copy));
    memcpy(synd_copy, synd, synd_len);
    utils_array_reverse(synd_copy, synd_len);
    
    uint16_t nsym = err_loc_len - 1;
    uint16_t err_eval_len = nsym + 1;
    uint8_t err_eval[err_eval_len];
    memset(err_eval, 0, sizeof(err_eval));
    rs_find_error_evaluator(synd_copy, synd_len, err_loc, err_loc_len, nsym,
                            err_eval, &err_eval_len);
    
    uint16_t x_len = err_pos_len;
    uint8_t x[x_len];
    memset(x, 0, sizeof(x));
    for (uint16_t i = 0; i < x_len; i++) {
        x[i] = gf_pow(2, coef_pos[i]);
    }
    
    uint8_t e[msg_len];
    memset(e, 0, sizeof(e));
    
    uint8_t err_loc_prime_tmp[x_len];
    
    for (uint16_t i = 0; i < x_len; i++) {
        uint8_t xi = x[i];
        uint8_t xi_inv = gf_inverse(xi);
        
        uint16_t err_loc_prime_tmp_len = 0;
        memset(err_loc_prime_tmp, 0, sizeof(err_loc_prime_tmp));
        for (uint16_t j = 0; j < x_len; j++) {
            if (j != i) {
                uint8_t val = gf_sub(1, gf_mult(xi_inv, x[j]));
                err_loc_prime_tmp[err_loc_prime_tmp_len++] = val;
            }
        }
        
        uint8_t err_loc_prime = 1;
        for (uint16_t k = 0; k < err_loc_prime_tmp_len; k++) {
            err_loc_prime = gf_mult(err_loc_prime, err_loc_prime_tmp[k]);
        }
        
        uint8_t y = gf_poly_eval(err_eval, err_eval_len, xi_inv);
        y = gf_mult(gf_pow(xi, 1), y);
        
        if (err_loc_prime == 0) {
            return false;
        }
        
        uint8_t magnitude = gf_div(y, err_loc_prime);
        e[err_pos[i]] = magnitude;
    }
    
    uint16_t msg_corrected_len = msg_len;
    uint8_t msg_corrected[msg_corrected_len];
    memset(msg_corrected, 0, sizeof(msg_corrected));
    gf_poly_add(msg, msg_len, e, msg_len, msg_corrected, &msg_corrected_len);
    
    memset(msg, 0, sizeof(*msg));
    memcpy(msg, msg_corrected, msg_corrected_len);
    
    return true;
}

bool rs_correct_msg(uint8_t * msg, uint16_t * msg_len, uint8_t nsym) {
    if (*msg_len > 255) {
        return false;
    }
    
    uint8_t msg_corrected[*msg_len];
    memset(msg_corrected, 0, sizeof(msg_corrected));
    memcpy(msg_corrected, msg, *msg_len);
    
    uint8_t synd_len = nsym + 1;
    uint8_t synd[synd_len];
    memset(synd, 0, sizeof(synd));
    rs_calc_syndromes(msg_corrected, *msg_len, nsym, synd, &synd_len);
    
    uint8_t res = utils_max_array_8(synd, synd_len);
    if (res == 0) {
        memset(msg, 0, *msg_len);
        memcpy(msg, msg_corrected, *msg_len - nsym);
        return true;
    }
    
    uint16_t err_loc_len = *msg_len;
    uint8_t err_loc[err_loc_len];
    memset(err_loc, 0, sizeof(err_loc));
    rs_find_error_locator(nsym, synd, synd_len, err_loc, &err_loc_len);
    
    uint16_t err_pos_len = *msg_len;
    uint8_t err_pos[err_pos_len];
    memset(err_pos, 0, sizeof(err_pos));
    rs_find_errors(err_loc, err_loc_len, *msg_len, err_pos, &err_pos_len);
    
    rs_correct_errata(msg_corrected, *msg_len, synd, synd_len, err_pos, err_pos_len);
    
    memset(synd, 0, sizeof(synd));
    rs_calc_syndromes(msg_corrected, *msg_len, nsym, synd, &synd_len);
    res = utils_max_array_8(synd, synd_len);
    if (res > 0) {
        return false;
    }
    
    memset(msg, 0, *msg_len);
    
    *msg_len = *msg_len - nsym;
    memcpy(msg, msg_corrected, *msg_len);
    
    return true;
}
