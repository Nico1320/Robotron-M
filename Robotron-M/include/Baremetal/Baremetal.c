/*
 * CFile1.c
 *
 * Created: 12/1/2023 2:24:51 PM
 *  Author: brand
 
#define _BV(bit) (1 << (bit)) //Set bit


#include <avr/eeprom.h>

 uint8_t *address = (uint8_t *) 0;
 eeprom_update_byte(address, 5);
 
 // Store the 16-bit value 12345 in EEPROM slots 5 and 6:
 
 eeprom_update_word((uint16_t *) 5, 12345);
 
 // Store a character array (string) in EEPROM slots 16-28:
 char *stringPointer = (char *) 16;
 char myString[] = "hello world.";
 eeprom_update_block(myString, stringPointer, sizeof(myString));
 return (0);
 */
#define Bomama 1