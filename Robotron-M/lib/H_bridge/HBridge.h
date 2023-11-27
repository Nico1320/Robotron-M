/*
 * motor.h
 *
 * Created: 11/27/2023 7:06:42 PM
 *  Author: brand
 */ 


#ifndef HBRIDGE_H_
#define HBRIDGE_H_

void initializeHbridge();
void setupTimer();
void setPWM(uint8_t, uint8_t);
void zeroRadii();
void goForward();
void goBackward();
void clearPrevious();

#endif /* MOTOR_H_ */