
#include <stdio.h>
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "includes/rgb.h"


#define R 13 
#define G 12 
#define B 11 
#define BRIGHTNESS 50
#define LOOP_SLEEP 10
#define MAX_COLOUR_VALUE 255
#define MAX_PWM_LEVEL 65535

void setup_rgb()
{
    // Tell the LED pins that the PWM is in charge of its value.
    gpio_set_function(R, GPIO_FUNC_PWM);
    gpio_set_function(G, GPIO_FUNC_PWM);
    gpio_set_function(B, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(R);

    pwm_config config = pwm_get_default_config();

    pwm_init(slice_num, &config, true);

    slice_num = pwm_gpio_to_slice_num(G);
    pwm_init(slice_num, &config, true);

    slice_num = pwm_gpio_to_slice_num(B);
    pwm_init(slice_num, &config, true);
}
void show_rgb(int r, int g, int b)
{
    pwm_set_gpio_level(R, ~(MAX_PWM_LEVEL * r * BRIGHTNESS / 100));
    pwm_set_gpio_level(G, ~(MAX_PWM_LEVEL * g * BRIGHTNESS / 100));
    pwm_set_gpio_level(B, ~(MAX_PWM_LEVEL * b * BRIGHTNESS / 100));
}
void rgb_off()
{
    pwm_set_gpio_level(R, ~(0));
    pwm_set_gpio_level(G, ~(0));
    pwm_set_gpio_level(B, ~(0));
}