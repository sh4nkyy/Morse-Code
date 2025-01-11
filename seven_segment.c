#include "includes/seven_segment.h"
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "includes/rgb.h"
#include "includes/buzzer.h"
#include <stdbool.h>
uint8_t values[] = {

    0b11111100, // 0
    0b01100000, // 1
    0b11011010, // 2
    0b11110010, // 3
    0b01100110, // 4
    0b10110110, // 5
    0b10111110, // 6
    0b11100000, // 7
    0b11111110, // 8 
    0b11110110, // 9

    0b11101110, // A
    0b00111110, // B
    0b10011100, // C
    0b01111010, // D
    0b10011110, // E
    0b10001110, // F
    0b11110110, // G
    0b01101110, // H
    0b01100000, // I
    0b01111000, // J
    0b01101110, // K
    0b00011100, // L
    0b10101000, // M
    0b00101010, // N
    0b11111100, // O
    0b11001110, // P
    0b11100110, // Q
    0b00001010, // R
    0b10110110, // S
    0b00011110, // T
    0b01111100, // U
    0b00111000, // V
    0b01010100, // W
    0b01101110, // X
    0b01110110, // Y
    0b11011010  // Z
};

#define DOT "."
#define DASH "-"

// Morse code array for A-Z and 0-9
char morse_code[][10] = {
    "-----", // 0
    "*----", // 1
    "**---", // 2
    "***--", // 3
    "****-", // 4
    "*****", // 5
    "-****", // 6
    "--***", // 7
    "---**", // 8
    "----*", // 9
    "*-",    // A
    "-***",  // B
    "-*-*",  // C
    "-**",   // D
    "*",     // E
    "**-*",  // F
    "--*",   // G
    "****",  // H
    "**",    // I
    "*---",  // J
    "-*-",   // K
    "*-**",  // L
    "--",    // M
    "-*",    // N
    "---",   // O
    "*--*",  // P
    "--*-",  // Q
    "*-*",   // R
    "***",   // S
    "-",     // T
    "**-",   // U
    "***-",  // V
    "*--",   // W
    "-**-",  // X
    "-*--",  // Y
    "--**",  // Z
};


const char symbols[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 
    'U', 'V', 'W', 'X', 'Y', 'Z'
};

char resultString[5] = "";
int count = 0;

// ------------------------------------------------------------------

void seven_segment_init() {
    for (unsigned int i = 0; i < ALL_SEGMENTS_COUNT; i++) {
        gpio_init(ALL_SEGMENTS[i]);
        gpio_set_dir(ALL_SEGMENTS[i], GPIO_OUT);
    }
}

void seven_segment_off() {
    // This display is common-anode, which means we need to turn a GPIO
    // pin ON for it to appear OFF on the display. 
    for (unsigned int i = 0; i < ALL_SEGMENTS_COUNT; i++) {
        gpio_put(ALL_SEGMENTS[i], true);
    }
}

void seven_segment_on() {
    for (unsigned int i = 0; i < ALL_SEGMENTS_COUNT; i++) {
        gpio_put(ALL_SEGMENTS[i], false);
    }
}

void seven_segment_show(unsigned int number) {
    for (unsigned int i = 0; i < 8; i++) {
        unsigned int segmentBit = 1 << (7 - i);
        bool illuminateSegment = (segmentBit & values[number]);

        gpio_put(
            ALL_SEGMENTS[i],
            !illuminateSegment
        );
    }
}

bool decoder(char* code) {
    bool checked = false;
    for(int i = 0; i < 36; i++){
        if (strcmp(morse_code[i],code) == 0){
            seven_segment_show(i);
            resultString[count] = symbols[i];
            count++;
            printf("%c\n",symbols[i]);
            checked = true;
            show_rgb(0,1,0);
            sleep_ms(1000);
            rgb_off();
            return true;
        }
    }
    error();
    return checked;

}

void error(){
    printf("Error MESSAGE\n");
    seven_segment_on();
    show_rgb(1,0,0);
    beep(1000,175);
    rgb_off();
}

void beep(int time, int frequency) {
    buzzer_init();
    buzzer_enable(frequency);  // Turn on the buzzer
    sleep_ms(time);    // Wait for the specified duration
    buzzer_disable();  // Turn off the buzzer
}

char* getResult(){
    count = 0;
    return resultString;
}