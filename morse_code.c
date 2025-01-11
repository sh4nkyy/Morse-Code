#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "includes/seven_segment.h"
#include "hardware/pwm.h"
#include "includes/buzzer.h"
#include "includes/rgb.h"
#include "includes/potentiometer.h"

#define BUTTON_PIN 16 // Pin 21 (GPIO 16)
#define BUTTON_PIN2 15

// Declare global variables for button press timing
int pressed = 0;
int not_pressed = -1;
int signalDiff = 10;
int letterDiff = 70;
char text[5] = "";
int idx = 0;
int averageDiff = 40;
bool buttonPressed = false;
int dotOrDash = 25;
int attempt = 0;


// Function declarations
void welcome_message();
void init_all();
char checkButton(int time);
void beep(int time, int frequency);
void error();
void initialize();
void inputAlter();
void decisionPrompt();
void resetComponents();
void terminateProgram();
void buzzerMusic();

// Main function
void main()
{
    // Setup system and peripherals
    timer_hw->dbgpause = 0;
    stdio_init_all();
    init_all();
    welcome_message();

    while (true) // Use the running flag to control the loop
    {
        inputAlter();
        if (gpio_get(BUTTON_PIN))
        {
            pressed = clock();
            while (gpio_get(BUTTON_PIN))
            {
                // Wait for button release
            }
            int time = clock() - pressed;
            if (time > letterDiff)
            {
                error();
                initialize();
            }
            else
            {
                text[idx] = checkButton(time);
                idx++;
                not_pressed = clock();
            }
        }
        if (not_pressed != -1)
        {
            int time = clock() - not_pressed;
            if (time > letterDiff || idx == 5)
            {
                printf("\n%s\n", text);
                bool checked = decoder(text);
                initialize();
                if(checked){
                    attempt++; // Increment attempt count
                }
            }
            // Check if 4 attempts have been completed
            if (attempt == 4)
            {   
                char* resultString = getResult();
                printf("The decoded message is: ");
                for(int i = 0; i < 4; i++){
                    printf("%c",resultString[i]);
                }
                printf("\n");
                buzzerMusic();
                decisionPrompt(); // Handle decision prompt
            }
        }
    }
}


void buzzerMusic()
{
    printf("4 attempts done!");
    beep(50, 440); // Beep 1
    sleep_ms(200);
    beep(50, 440); // Beep 2
    sleep_ms(200);
    beep(50, 440); // Beep 3
    sleep_ms(200);
    beep(50, 660); // Beep 4
    sleep_ms(200);
    beep(50, 880); // Beep 5
    sleep_ms(200);
    beep(50, 660); // Beep 6
    sleep_ms(200);
    beep(100, 440); // Beep 7
}

// CheckButton function to determine dot or dash
char checkButton(int time)
{
    if (time < dotOrDash)
    {
        printf(" DOT ");
        beep(100, 262);
        return '*';
    }
    else
    {
        printf(" DASH ");
        beep(300, 262);
        return '-';
    }
}

void initialize()
{
    memset(text, 0, sizeof(text));
    idx = 0;
    not_pressed = -1;
}

void welcome_message()
{
    seven_segment_on();
    sleep_ms(500);
    seven_segment_off();
    printf("WELCOME\n");
}

void init_all()
{
    gpio_init(BUTTON_PIN);
    gpio_init(BUTTON_PIN2);             // Initializes the GPIO pin for the button
    gpio_set_dir(BUTTON_PIN, GPIO_IN);  // Sets the pin direction as input
    gpio_set_dir(BUTTON_PIN2, GPIO_IN);
    gpio_pull_down(BUTTON_PIN);
    gpio_pull_down(BUTTON_PIN2);         
    seven_segment_init();
    potentiometer_init();
    buzzer_init();
    setup_rgb();
    rgb_off();
}

void inputAlter()
{
    unsigned int value = potentiometer_read(4);
    if (letterDiff != 60 + 5 * value)
    {
        letterDiff = 60 + 5 * value;
        signalDiff = 6 + 2 * value;
        printf("Letter gap: %d0ms, Signal gap: %d0ms\n", letterDiff, signalDiff);
    }
}

void decisionPrompt()
{
    printf("\n4 Valid Attempts Completed!\n");
    printf("Press Left Button (Yes) to continue or Right Button (No) to exit.\n");

    // Wait for user input
    while (attempt == 4)
    {
        if (gpio_get(BUTTON_PIN))
        {
            // Left button pressed (Yes), reset components and continue
            resetComponents();
            break;
        }
        else if (gpio_get(BUTTON_PIN2))
        {
            // Right button pressed (No), terminate program
            terminateProgram();
    }
}
}
void resetComponents()
{
    // Reset attempt count and components
    show_rgb(0, 0, 255);
    attempt = 0;
    sleep_ms(1000);
    printf("Components reset. Continuing...\n");
    rgb_off(); // Turn off the RGB LED
}

void terminateProgram()
{
    // Turn off all components
    show_rgb(255,0,0);
    sleep_ms(500);
    printf("Terminating program...\n");
    rgb_off();
    buzzer_disable();
    seven_segment_off();
    sleep_ms(500); // Set the flag to false to exit the loop
    printf("Program has terminated.\n");
    exit(9999);
}

bool is_button_pressed(uint button_pin) {
    if (gpio_get(button_pin)) {
        sleep_ms(50); // Wait to debounce
        return gpio_get(button_pin); // Confirm the press
    }
    return false;
}
