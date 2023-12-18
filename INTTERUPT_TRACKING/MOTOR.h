#ifndef _MOTOR_H
#define _MOTOR_H

#include <avr/io.h>

#define STOP 		0
#define FORWARD 	1
#define TURN_LEFT 	2
#define BACKWARD	3
#define TURN_RIGHT 	4

#define LEFT_MOTOR  0
#define RIGHT_MOTOR 1

#define MOTOR_CONTORL_PORT          PORTB
#define LEFT_MOTOR_FOWARD_PIN       PB1
#define LEFT_MOTOR_BACKWARD_PIN 	PB0
#define LEFT_MOTOR_OCR				OCR3A
#define RIGHT_MOTOR_FOWARD_PIN 		PB3
#define RIGHT_MOTOR_BACKWARD_PIN 	PB2
#define RIGHT_MOTOR_OCR				OCR3B

/**
 *	PB0 -> left  motor backward
 *	PB1 -> left  motor forward
 *	PB2 -> right motor forward
 *	PB3 -> right motor backward
 *  OC1A(PB5)
 *  OC1B(PB6)
 *  OC1C(PB7)
 *  OC3A(PE3)
 *  OC3B(PE4)
 *  OC3C(PE5)
 */

/**
 *	Timer3 PWM For Motor
 *	WGM33:30 = 0111 -> Fast PWM, 10 bit
 *	COMnA1/COMnB1/COMnC1 = 1 -> Clear OCnA/OCnB/OCnC on compare match, set OCnA/OCnB/OCnC at BOTTOM, (non-inverting mode)
 *	CSn2:0 = 100 -> clk/256(From prescalar)
 *	OCR3A = 0 -> this is used to control duty cycle, 0 means 0% duty cycle
 */
void init_PWM_time3(void);

/**
 * change the motor direction
 * @motor: the motor you want to toggle. allow param: LEFT_MOTOR、RIGHT_MOTOR
 * @direction: forward spin or backward spin. allow param: FORWARD、 BACKWARD
 */
void motorDirection(uint8_t motor, uint8_t direction);

/**
 * change the motor speed
 * @motor: the motor you want to toggle. allow param: LEFT_MOTOR、RIGHT_MOTOR
 * @duty_cycle: 0~100
 * @return: output comparator register(OCR) value
 */
uint16_t motorSpeed(uint8_t motor, uint8_t duty_cycle);
void motorStop(uint8_t motor);

void motion(uint8_t dir,uint8_t speed);

#endif /* _MOTOR_H_ */
