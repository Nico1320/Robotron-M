#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <stdbool.h>
// Include modules
#include "./H_bridge/HBridge.h"
#include "./Soft_serial/SoftSerial.h"
#include "./Ultrasonic/Ultrasonic.h"
#include "./Display/lcd.h"

//#include "./Baremetal/Baremetal.h"
//#include "./Bit_bang/ShiftRegister.h"

// Defines
#define DELAY_TIME_MS 35 // Set the sampling time in milliseconds
bool serialFlag;

volatile int keyPressed = 0;
//Function prototypes
void manualMode();
void automaticMode();
void slaveMode();
void setupMC();

void manualMode() {
	// Wait for USART data to become available
	while (usart0_nUnread() == 0) {
		if (!serialFlag) {
			// Display message when serial communication is not detected
			lcd_clrscr();
			lcd_gotoxy(0, 0);
			lcd_puts(" Serial not ");
			lcd_gotoxy(0, 1);
			lcd_puts(" Detected");
			serialFlag = true;
			} else if (usart0_nUnread() > 0) {
			// Display message when serial communication is detected
			lcd_clrscr();
			lcd_gotoxy(0, 0);
			lcd_puts(" Serial");
			lcd_gotoxy(0, 1);
			lcd_puts("Detected!");
			serialFlag = false;
			break;
		}
	}

	// Continuous operation loop
	while (!serialFlag) {
		// Check for available USART data
		if (usart0_nUnread() > 0) {
			char data = usart0_receive();
			switch (data) {
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

			// Set flag and introduce delay
			keyPressed = 1;
			_delay_ms(DELAY_TIME_MS);
			} else {
			// Clear previous state if flag is set
			if (keyPressed) {
				clearPrevious();
				keyPressed = 0; // Reset the flag
			}
		}
	}

	// Clear USART registers after the loop
	clearUSART();
}


void automaticMode() {
	return 0;
}

void slaveMode() {
	return 0;
}

void setupMC() {
	usart0_init();

	initializeHbridge();
	setupTimer();

	// Display init
	lcd_init(LCD_ON_DISPLAY);
	lcd_backlight(0);
	_delay_ms(500);
	lcd_backlight(1);
	_delay_ms(500);

	usart0_transmit_str("Main loop running!\r\n");
//	ultrasonicInit();
	sei();
}