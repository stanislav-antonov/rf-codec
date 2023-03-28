//
//  Copyright © 2023 Sensory Ninja. All rights reserved.
//

#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

uint16_t utils_pack(char* data, uint16_t length, uint16_t* packed);

uint16_t utils_unpack(uint16_t* data, uint16_t length, char* unpacked);

#endif
