//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#ifndef rs_codec_h
#define rs_codec_h

#include <stdio.h>
#include <stdbool.h>
#include "gf.h"
#include "utils.h"

void rs_init(void);
void rs_encode(uint8_t nsym, uint8_t * msg_in, uint16_t msg_in_len, uint8_t * msg_out, uint16_t * msg_out_len);
bool rs_message_is_corrupted(uint8_t * msg, uint16_t msg_len, uint8_t nsym);
bool rs_correct_msg(uint8_t * msg, uint16_t * msg_len, uint8_t nsym);

#endif /* rs_codec_h */
