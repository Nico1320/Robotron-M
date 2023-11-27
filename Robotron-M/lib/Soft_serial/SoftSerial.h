/*
 * SoftSerial.h
 *
 * Created: 21-9-2021 20:07:38
 *  Author: Hugo Arends
 */ 
#ifndef SOFTSERIAL_H_
#define SOFTSERIAL_H_

/* Prototypes */
void usart0_init(void);
char usart0_receive(void);
void usart0_transmit(char data);
unsigned char usart0_nUnread(void);
void usart0_transmitStr(char *str);

#endif /* SOFTSERIAL_H_ */