// rgb_led_strip.h
#ifndef RGB_LED_STRIP_H
#define RGB_LED_STRIP_H

#include <avr/io.h>
#include <stdint.h>

typedef struct rgb_color
{
	uint8_t red, green, blue;
} rgb_color;

void led_strip_init(volatile uint8_t *port, volatile uint8_t *ddr, uint8_t pin);
void led_strip_write(rgb_color *colors, uint16_t count);

#endif // RGB_LED_STRIP_H
