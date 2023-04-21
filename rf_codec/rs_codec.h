//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#ifndef rs_codec_h
#define rs_codec_h

#include <stdio.h>
#include "gf.h"

void rs_init(void);

void rs_generator_poly(uint8_t nsym, uint8_t * g, uint16_t * len_g);

void rs_encode(uint8_t nsym, uint8_t * msg_in, uint16_t msg_in_len, uint8_t * msg_out, uint16_t * msg_out_len);

#endif /* rs_codec_h */
