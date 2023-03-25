//
//  main.c
//  Playground
//
//  Created by Stanislav Antonov on 18/01/2022.
//  Copyright © 2022 Sensory Ninja. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../include/hamming_encode.h"

#define TEMP_SENSOR_ID "OtHS6s"
#define RHUM_SENSOR_ID "vvAQu5"

#define MSG_LENGTH 16

const char* rhum_sensor_id = RHUM_SENSOR_ID;
const char* temp_sensor_id = TEMP_SENSOR_ID;

// the format: "<sensor_id>(6) <value>(2) <bat_status>(3)"
static uint8_t rhum_msg[MSG_LENGTH]; // relative humidity
static uint8_t temp_msg[MSG_LENGTH]; // temperature

void set_id_msg_chars(void) {
    strcpy((char *)rhum_msg, rhum_sensor_id);
    strcpy((char *)temp_msg, temp_sensor_id);
}

void set_temp_msg(double temp) {
    for (uint8_t i = 6; i < MSG_LENGTH; i++) {
        temp_msg[i] = ' ';
    }
    
    char temp_chars[6 + 1];
    sprintf(temp_chars, "%3.2f", temp);
    
    strcpy((char *)temp_msg + 6 + 1, temp_chars);
}

void set_rhum_msg(double rhum) {
    for (uint8_t i = 6; i < MSG_LENGTH; i++) {
        rhum_msg[i] = ' ';
    }
    
    char rhum_chars[6 + 1];
    sprintf(rhum_chars, "%3.2f", rhum);
    
    strcpy((char *)rhum_msg + 6 + 1, rhum_chars);
}

double HTU21D_ReadTemperature() {
    return 100.4;
}

double HTU21D_ReadRelativeHumidity() {
    return 99.0;
}

int main(int argc, const char * argv[]) {
    int n = 4;
    int size = 5;
    char data[] = { 0b10111011, 0b10111011, 0b10111011, 0b10111011, 0b10111011 };
    char code_words[size * 3];
    
    hamming_encode(data, code_words, size, n);
    
    printf("data: %s\n", data);
    printf("code words: %s\n", code_words);
    
    // char *cw = "0010111";
    // char *cw = "001011101111110";
    // int p = hamming_parity(15, cw);
    // printf("encoded: %d\n", p);
    
    return 0;
}
