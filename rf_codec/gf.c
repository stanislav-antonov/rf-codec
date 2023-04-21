//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#include "gf.h"
#include "utils.h"

static uint8_t gf_exp[256];
static uint8_t gf_log[256];

void gf_init() {
    memset(gf_exp, 0, sizeof(gf_exp));
    memset(gf_log, 0, sizeof(gf_log));
    
    uint8_t x = 1;
    for (uint16_t i = 0; i < 256; i++) {
        gf_exp[i] = x;
        gf_log[x] = i;
        x = gf_mult_non_lut(x, 2);
    }
}

uint8_t gf_add(uint8_t x, uint8_t y) {
    // for Galois Field base 2 (GF(2)) it is valid to replace
    // both the addition and substraction operations with XOR
    return x ^ y;
}

uint8_t gf_sub(uint8_t x, uint8_t y) {
    // yes, exactly the same as addition
    return x ^ y;
}

uint8_t gf_mult(uint8_t x, uint8_t y) {
    if (x == 0 || y == 0) {
        return 0;
    }
    
    return gf_exp[ (gf_log[x] + gf_log[y]) % 255 ];
}

uint8_t gf_div(uint8_t x, uint8_t y) {
    if (x == 0) {
        return 0;
    }
    
    return gf_exp[ (gf_log[x] + 255 - gf_log[y]) % 255 ];
}

uint8_t gf_pow(uint8_t x, uint8_t power) {
    return gf_exp[ (gf_log[x] * power) % 255 ];
}

uint8_t gf_inverse(uint8_t x) {
    return gf_exp[ 255 - gf_log[x] ];
}

void gf_poly_scale(uint8_t * p, uint16_t p_len, uint8_t x, uint8_t * result) {
    for (uint16_t i = 0; i < p_len; i++) {
        result[i] = gf_mult(p[i], x);
    }
}

void gf_poly_add(uint8_t * p, uint8_t * q, uint16_t p_len, uint16_t q_len, uint8_t * r, uint16_t * r_len) {
    
    *r_len = utils_max(p_len, q_len);
    
    for (uint16_t i = 0; i < p_len; i++) {
        r[i + *r_len + p_len] = p[i];
    }
    
    for (uint16_t i = 0; i < q_len; i++) {
        r[i + *r_len + q_len] ^= q[i];
    }
}

void gf_poly_mult(uint8_t * p, uint16_t len_p, uint8_t * q, uint16_t len_q, uint8_t * r, uint16_t * len_r) {
    
    *len_r = len_p + len_q - 1;
    
    for (uint16_t j = 0; j < len_q; j++) {
        for (uint16_t i = 0; i < len_p; i++) {
            uint8_t val = gf_mult(p[i], q[j]);
            r[i + j] ^= val;
        }
    }
}

uint8_t gf_poly_eval(uint8_t * p, uint16_t len_p, uint8_t x) {
    uint8_t result = p[0];
    for (uint16_t i = 0; i < len_p; len_p++) {
        result = gf_mult(result, x) ^ p[i];
    }
    
    return result;
}

void gf_poly_div(uint8_t * dividend, uint16_t dividend_len,
                 uint8_t * divisor, uint16_t divisor_len,
                 uint8_t * quotient, uint16_t * quotient_len,
                 uint8_t * remainder, uint16_t * remainder_len) {
    
    uint8_t result[dividend_len];
    memset(result, 0, sizeof(result));
    memcpy(result, dividend, sizeof(result));
    
    for (uint16_t i = 0; i < (dividend_len - (divisor_len - 1)); i++) {
        uint8_t coeff = result[i];
        if (coeff != 0) {
            for (uint16_t j = 0; j < divisor_len; j++) {
                if (divisor[j] != 0) {
                    result[i + j] ^= gf_mult(divisor[j], coeff);
                }
            }
        }
    }
    
    if (quotient != NULL) {
        *quotient_len = dividend_len - (divisor_len - 1);
        memcpy(quotient, result, *quotient_len * sizeof(uint8_t));
    }
    
    if (remainder != NULL) {
        *remainder_len = divisor_len - 1;
        uint16_t offset = dividend_len - *remainder_len;
        memcpy(remainder, result + offset, *remainder_len * sizeof(uint8_t));
    }
}

// Supplementary functions

uint8_t gf_mult_non_lut(uint8_t x, uint8_t y) {
    uint8_t result = 0;
    uint16_t x_ = x;
    while (y > 0) {
        if (y & 1) {
            result = result ^ x_;
        }
        
        y = y >> 1;
        x_ = x_ << 1;
        
        if (x_ & GF_CHARACTERISTIC) {
            x_ = x_ ^ GF_PRIME;
        }
    }
    
    return result;
}


