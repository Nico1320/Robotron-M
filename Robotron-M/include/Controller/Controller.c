// Inlcude libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <stdbool.h>
#include <math.h>

// Include modules
#include "./H_bridge/HBridge.h"
#include "./Soft_serial/SoftSerial.h"
#include "./Ultrasonic/Ultrasonic.h"
#include "./Display/lcd.h"
#include "./LEDrgb/ws28xx.h"
#include "./IR/IR.h"
#include "./Controller/Controller.h"
#include "./Baremetal/Baremetal.h"
//#include "./Bit_bang/ShiftRegister.h"

// Defines
CarMode_t mode;

// IR control
#define SENSOR_LEFT 0
//#define SENSOR_MIDDLE 1
#define SENSOR_RIGHT 2
int leftIrRead = 0;
int rightIrRead = 0;
static volatile uint16_t DEBOUNCE_TIME_LEFT = 55;
static volatile uint16_t DEBOUNCE_TIME_RIGHT = 55;
volatile uint32_t lastDebounceTimeLeft = 0;
volatile uint32_t lastDebounceTimeRight = 0;
bool allowReadL = false;
bool allowReadR = false;
bool rightcint;
bool leftcint;

//Ultrasonic control
#define MINIMUM_DISTANCE 9

// RGB Led control
#define LED_COUNT 15
rgb_color colors[LED_COUNT];

//Control Flags
bool serialFlag;
bool ModeActive = false;

// ADC Button
#define BUTTONS 3
#define RESOLUTION 1023
#define DEBOUNCE_TIME 60
int buttonstate = 0;
bool ButtonPressed;
float avg = 341;
volatile uint32_t millis = 0;
volatile uint32_t lastDebounceTime = 0;

// Display control
#define TIMER2_FREQ 4
bool ScreenHome = false;
bool lcd_refresh = true;

//Debug stuff



// Function to set a solid color for specified LEDs
void setSolidColor(rgb_color color, uint16_t leds[], uint8_t num_leds) {
	for (uint8_t i = 0; i < num_leds; i++) {
		uint16_t index = leds[i];
		if (index < LED_COUNT) {
			colors[index] = color;
		}
	}
	led_strip_write(colors, LED_COUNT);
}

// Function to clear all LEDs
void setClear() {
	for (uint16_t i = 0; i < LED_COUNT; i++) {
		colors[i] = (rgb_color){0, 0, 0};
	}
	led_strip_write(colors, LED_COUNT);
}

// Function for red color for specified LEDs
void setRed(uint16_t leds[], uint8_t num_leds) {
	setSolidColor((rgb_color){255, 0, 0}, leds, num_leds);
}

// Function for green color for specified LEDs
void setGreen(uint16_t leds[], uint8_t num_leds) {
	setSolidColor((rgb_color){0, 255, 0}, leds, num_leds);
}

// Function for blue color for specified LEDs
void setBlue(uint16_t leds[], uint8_t num_leds) {
	setSolidColor((rgb_color){0, 0, 255}, leds, num_leds);
}

// Function for orange color for specified LEDs
void setOrange(uint16_t leds[], uint8_t num_leds) {
	setSolidColor((rgb_color){255, 40, 0}, leds, num_leds);
}

// Function for pink color for specified LEDs
void setPink(uint16_t leds[], uint8_t num_leds) {
	setSolidColor((rgb_color){255, 0, 40}, leds, num_leds);
}

// Function for purple color for specified LEDs
void setPurple(uint16_t leds[], uint8_t num_leds) {
	setSolidColor((rgb_color){80, 0, 255}, leds, num_leds);
}

  
void Serialcheck() {
	while (!serialFlag) {
		systemDataPrint("SerialNotDet");
		serialFlag = usart0_receive();
		if (serialFlag) {
			systemDataPrint("SerialDet");
			usart0_transmit_str("Receive ready!\r\n");
			_delay_ms(3000);
			break;
		}
	}
}

void manualMode() {
	//Serialcheck();
			uint16_t red_leds[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
			uint8_t num_red_leds = sizeof(red_leds) / sizeof(red_leds[0]);
			setRed(red_leds, num_red_leds);
	// Manual mode 
	ScreenHome = false;
	if (usart0_nUnread() > 0) {
	char data = usart0_receive();
	
		switch (data) {
			case 'F':
			goForward();
			setPWM(255, 255);
			break;

			case 'G':
			setPWM(200, 200);
			goBackward();
			break;

			case 'L':
			setPWM(200, 200);
			zeroRadii(0);
			break;

			case 'R':
			setPWM(200, 200);
			zeroRadii(1);
			break;
			default:
			usart0_transmit_str("Invalid input\r\n");
			clearPrevious(0);
			break;
		}
	}

}


void automaticMode() {
	//uint8_t ultra = ultrasonicRead();
	//usart0_transmit_str(dtostrf(ultra, 0, 0, ultrares));
	//usart0_transmit_str("\r\n");
}




void slaveMode() {
	ScreenHome = false;

			uint16_t green_leds[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
			uint8_t num_green_leds = sizeof(green_leds) / sizeof(green_leds[0]);
			setGreen(green_leds, num_green_leds);
		volatile uint8_t ulsRead = 30; // It seems like ulsRead should be updated inside the loop

		if (!irSensorRead(SENSOR_LEFT) && ((millis - lastDebounceTimeLeft) < DEBOUNCE_TIME_LEFT) && leftcint) {
			allowReadL = false;
			//usart0_transmit_str("False\r\n");
		}
		else  {
			allowReadL = true;
			leftcint = irSensorRead(SENSOR_LEFT);
			lastDebounceTimeLeft = millis;
			//usart0_transmit_str("True\r\n");
			
		}
		
		if (!irSensorRead(SENSOR_RIGHT) && ((millis - lastDebounceTimeRight) < DEBOUNCE_TIME_RIGHT) && rightcint) {
			allowReadR = false;
		}
		else {
			allowReadR = true;
			rightcint = irSensorRead(SENSOR_RIGHT);
			lastDebounceTimeRight = millis;
		}
		
		if (allowReadL) {
			leftIrRead = irSensorRead(SENSOR_LEFT);
		}
		
		if (allowReadR) {
			rightIrRead = irSensorRead(SENSOR_RIGHT);
		} 
		
	
		if (ulsRead > MINIMUM_DISTANCE)		 {
			// Right turn
			if (leftIrRead) {
				//goForward();
				zeroRadii(false);
				usart0_transmit_str("left\r\n");
				setPWM(140, 140);
				//turningRatio(1.8, 180);
			}
			// Go straight
			else if (!leftIrRead && !rightIrRead) {
				goForward(); // possible setPWM refactor also
				turningRatio(1, 120);
				usart0_transmit_str("straight\r\n");
				
			}
			// Left turn
			else if (rightIrRead) {
				//goForward();
				zeroRadii(true);
				usart0_transmit_str("right\r\n");
				setPWM(140, 140);
				//turningRatio(0.2, 180); // discuss the exact value and use #define + possible setPWM() refactor
				}
		}
}


void initializeModules() {
	usart0_init();
	// Display init
	lcd_init(LCD_ON_DISPLAY);
	lcd_backlight(0);
	_delay_ms(500);
	lcd_backlight(1);
	_delay_ms(500);

	usart0_transmit_str("Main loop running!\r\n");
	ultrasonicInit();
	initFreerunningADC();
	initTimer1();
	initTimer2();
	initializeHbridge();
	setupTimer();
	
	sei();
}

void buttonADC() {
	if (val > 853) {
		} else {
		if (val < 171) {
			buttonstate = 1;
			debounceButton();
			} else if (val > 171 && val < 512) {
			buttonstate = 2;
			debounceButton();
			} else if (val > 512 && val < 853) {
			buttonstate = 3;
			debounceButton();
			//usart0_transmit_str("button pressed!");
		}

		if (ButtonPressed && !ModeActive) {
			switch (buttonstate) {
				case 1:
				mode = Manual;  
				ModeActive = true;
				lcd_refresh = true;
				break;

				case 2:
				mode = Slave; 
				ModeActive = true;
				lcd_refresh = true;
				break;

				case 3:
				mode = Automatic;  
				ModeActive = true;
				lcd_refresh = true;
				break;

				default:
				mode = 0;
				break;
			}
			} else if (ButtonPressed && ModeActive) {
			switch (buttonstate) {
				default:
				mode = 0;
				lcd_refresh = true;
				ModeActive = false;
				clearPrevious();
				break;
			}
		}
	}
	// Clear ADC interrupt flag
	ADCSRA |= (1 << ADIF);
}

void Modeselect() {
	while (1) {
		
		//Debounce LCD and Button board
		//debounceLCD();
		buttonADC();
		
		// Main menu
		if (!ModeActive && !ScreenHome && lcd_refresh) {
			clearPrevious();
			lcd_refresh = false;
			mode = NoMode;
			systemDataPrint("Modeselect");
			ScreenHome = true;
			uint16_t pink_leds[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
			uint8_t num_pink_leds = sizeof(pink_leds) / sizeof(pink_leds[0]);
			setPink(pink_leds, num_pink_leds);
		}
		
		
		// Vehicle telemetrics menu
		else if (ModeActive && !ScreenHome && lcd_refresh) {
			lcd_refresh = false;
			systemDataPrint("Telemetrics");
			void LEDcntrl();
		}
		
		ADCSRA |= (1 << ADSC); // Start first ADC conversion
		
		//Switch case for mode selection
		switch (mode) {
			case Automatic:
			automaticMode();
			break;
			
			case Slave:
			slaveMode();
			break;
			
			case Manual:
			manualMode();
			break;
			case NoMode:
			ModeActive = false;
			break;
		}
	}
}

void systemDataPrint(char *pdisplayMode){
	
	if(strcmp(pdisplayMode,"Telemetrics") == 0){
		
		char ultrasonicNumtoStr[20];
		char carDirectionStr[30];
		char telemetricsDataStrLcdReady[80];
		// unit for the final telemetrics text
		char distanceUnit [10]= " cm";

		// ultrasonic read then conversion to string for the display
		int ultrasonicNum = ultrasonicRead();
		sprintf(ultrasonicNumtoStr, "%d", ultrasonicNum);

		// car direction read based on the PWM ratio then conversion to string for the display
		float carDirectionNum = OCR0A / OCR0B;

		// determining car direction using the OCR ratio then converting the value to a string
		if (carDirectionNum == 1)
		{
			strcpy(carDirectionStr, "Forward  ");
		}
		else if (carDirectionNum > 1)
		{
			strcpy(carDirectionStr, "Right  ");
		}
		else if (carDirectionNum < 1)
		{
			strcpy(carDirectionStr, "Left  ");
		}

		// Concatenating direction, distance and unit strings into the final telemetrics string
		strcpy(telemetricsDataStrLcdReady, carDirectionStr);
		strcat(telemetricsDataStrLcdReady, ultrasonicNumtoStr);
		strcat(telemetricsDataStrLcdReady, distanceUnit);

		
		if (lcd_refresh) {lcd_clrscr();}
		lcd_gotoxy(0, 0);
		lcd_puts("  Dir  Dist");
		lcd_gotoxy(0, 1);
		lcd_puts(telemetricsDataStrLcdReady);
	}
	
	else if(strcmp(pdisplayMode, "SerialNotDet") == 0){
		lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts(" Serial not ");
		lcd_gotoxy(0, 1);
		lcd_puts(" Detected");
		serialFlag = usart0_receive();
	}
	
	else if(strcmp(pdisplayMode, "SerialDet") == 0){
		if (lcd_refresh) {lcd_clrscr();}
		lcd_gotoxy(0, 0);
		lcd_puts(" Serial");
		lcd_gotoxy(0, 1);
		lcd_puts("Detected!");
	}
	
	else if(strcmp(pdisplayMode, "Modeselect") == 0){
		//ModeSelectmenu
		if (lcd_refresh) {lcd_clrscr();}
		lcd_gotoxy(0, 0);
		lcd_puts(" Mode Selection");
		lcd_gotoxy(0, 1);
		lcd_puts("  A |  B |  C ");
	}
}




// millis counter thang
void initTimer1() {
	// Set Timer 1 in CTC mode with a prescaler of 64
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);
	
	// Set the compare match value for 1ms interval
	OCR1A = 2499;  // For a 16MHz clock and prescaler 64
	
	// Enable the compare match interrupt
	TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
	// Increment the milliseconds counter
	millis++;
}

void debounceButton() {	
	if (millis - lastDebounceTime > DEBOUNCE_TIME) {
		ButtonPressed = true;
		lastDebounceTime = millis;
		//usart0_transmit_str("Debouncedddd!\r\n");
	}
	else {
		ButtonPressed = false;
		//usart0_transmit_str("Sad :(\r\n");
	}
}


void LEDcntrl() {
	float carDirectionNum = OCR0A / OCR0B;
			if (carDirectionNum == 1)
			{

				uint16_t green_leds[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
				uint8_t num_green_leds = sizeof(green_leds) / sizeof(green_leds[0]);
				setGreen(green_leds, num_green_leds);
			}
			else if (carDirectionNum > 1)
			{
				uint16_t turnL_leds[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
				uint8_t num_turnL_leds = sizeof(turnL_leds) / sizeof(turnL_leds[0]);
				setOrange(turnL_leds, num_turnL_leds);
				
			}
			else if (carDirectionNum < 1)
			{
				uint16_t turnR_leds[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
				uint8_t num_turnR_leds = sizeof(turnR_leds) / sizeof(turnR_leds[0]);
				setOrange(turnR_leds, num_turnR_leds);
			}
}

void initTimer2() {
    // Set Timer/Counter 2 in CTC mode with a prescaler of 256
    TCCR2A = 0;
    TCCR2B = (1 << WGM21) | (1 << CS22);

    // Set the compare match value for 30 Hz frequency
    OCR2A = F_CPU / (256UL * TIMER2_FREQ) - 1;

    // Enable the compare match interrupt
    TIMSK2 |= (1 << OCIE2A);
}

ISR(TIMER2_COMPA_vect) {
	lcd_refresh = true;
}
