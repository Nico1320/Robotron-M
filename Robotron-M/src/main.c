#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include "./H_bridge/HBridge.h"
#include "./Soft_serial/SoftSerial.h"
//#include "./Baremetal/Baremetal.h"
//#include "./Bit_bang/ShiftRegister.h"
#include "./Display/lcd.h"
#include "./Ultrasonic/Ultrasonic.h"
extern void lcd_backlight(char on);  

#define DELAY_TIME_MS 35 // Set the sampling time in milliseconds

uint8_t keyPressed = 0;

int main(void) {
	
	usart0_init();
	sei();
	initializeHbridge();
	setupTimer();
	lcd_init(LCD_ON_DISPLAY);
	lcd_backlight(0);
	_delay_ms(500);
	lcd_backlight(1);
	_delay_ms(500);
	usart0_transmit_str("Main loop running!\r\n");
	ultrasonicInit();

	while (1)
	{
		char mystr[16];
		float distance = ultrasonicRead();
		lcd_clrscr();
        lcd_gotoxy(0, 0);
		lcd_puts_P(" Distance: ");
		lcd_gotoxy(10, 0);
	    lcd_puts(dtostrf(distance, 0, 2, mystr));
		lcd_gotoxy(0, 1);
		lcd_puts("Speed");
		_delay_ms(1000);
		
		
		
		if (usart0_nUnread() > 0)
		{
			char data = usart0_receive();
			switch (data)
			{
				case 'w':
				setPWM(255, 255);
				goForward();
				
				break;

				case 's':
				setPWM(200, 200);
				goBackward();
				break;

				case 'a':
				setPWM(200, 200);
				zeroRadii(0);
				break;

				case 'd':
				setPWM(200, 200);
				zeroRadii(1);
				break;

				default:
				usart0_transmit_str("Invalid input\r\n");
				break;
			}

			// Set the flag to indicate a key was pressed
			keyPressed = 1;

			// Introduce a delay for sampling time
			_delay_ms(DELAY_TIME_MS);
		}
		else
		{
			// Check if no keys are pressed after a key press
			if (keyPressed)
			{
				clearPrevious();
				keyPressed = 0; // Reset the flag
			}
		}
	}

	// Clear USART registers after the loop	
	clearUSART();
	return 0;
}
