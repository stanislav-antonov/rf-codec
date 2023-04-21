//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#ifndef gf_h
#define gf_h

#include <stdio.h>
#include <string.h>
#include "utils.h"

#define GF_PRIME 0x11d
#define GF_CHARACTERISTIC 256

void gf_init(void);

uint8_t gf_add(uint8_t x, uint8_t y);

uint8_t gf_sub(uint8_t x, uint8_t y);

uint8_t gf_mult(uint8_t x, uint8_t y);

uint8_t gf_div(uint8_t x, uint8_t y);

uint8_t gf_pow(uint8_t x, uint8_t power);

uint8_t gf_inverse(uint8_t x);

void gf_poly_scale(uint8_t * p, uint16_t length, uint8_t x, uint8_t * result);

void gf_poly_add(uint8_t * p, uint8_t * q, uint16_t len_p, uint16_t len_q, uint8_t * r, uint16_t * len_r);

void gf_poly_mult(uint8_t * p, uint16_t len_p, uint8_t * q, uint16_t len_q, uint8_t * r, uint16_t * len_r);

uint8_t gf_poly_eval(uint8_t * p, uint16_t len_p, uint8_t x);

void gf_poly_div(uint8_t * dividend, uint16_t dividend_len,
                 uint8_t * divisor, uint16_t divisor_len,
                 uint8_t * quotient, uint16_t * quotient_len,
                 uint8_t * remainder, uint16_t * remainder_len);

uint8_t gf_mult_non_lut(uint8_t x, uint8_t y);

uint16_t gf_mult_carryless(uint8_t x, uint8_t y);

uint8_t gf_bit_length(uint16_t n);

uint8_t gf_div_carryless(uint16_t dividend, uint16_t divisor);

#endif /* gf_h */
