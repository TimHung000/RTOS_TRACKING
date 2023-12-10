#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "MOTOR.h"	
#include "LCD.h"

/**
 *	Time3 PWM For Motor
 *	WGM33:30 = 0111 -> Fast PWM, 10 bit
 *	COMnA1/COMnB1/COMnC1 = 1 -> Clear OCnA/OCnB/OCnC on compare match, set OCnA/OCnB/OCnC at BOTTOM, (non-inverting mode)
 *	CSn2:0 = 100 -> clk/256(From prescalar)
 *	OCR3A = 0 -> this is used to control duty cycle, 0 means 0% duty cycle
*/
void init_PWM_time3(void) {
	TCCR3A = (1 << COM3A1) | (1 << COM3B1) | (1 << WGM31) | (1 << WGM30);
	TCCR3B = (1 << WGM32) | (1 << CS32);

	// initilize both left and right motor duty cycle to 0
	LEFT_MOTOR_OCR = 0;
	RIGHT_MOTOR_OCR = 0;
	

}

/**
 * change the motor direction
 * @motor: the motor you want to toggle. allow param: LEFT_MOTOR、RIGHT_MOTOR
 * @direction: forward spin or backward spin. allow param: FORWARD、 BACKWARD
 */
void motorDirection(uint8_t motor, uint8_t direction) {
	if (motor == LEFT_MOTOR) {
		if (direction == FORWARD) {
			MOTOR_CONTORL_PORT |= (1 << LEFT_MOTOR_FOWARD_PIN);
			MOTOR_CONTORL_PORT &= ~(1 << LEFT_MOTOR_BACKWARD_PIN);
		} else if (direction == BACKWARD) {
			MOTOR_CONTORL_PORT |= (1 << LEFT_MOTOR_BACKWARD_PIN);
			MOTOR_CONTORL_PORT &= ~(1 << LEFT_MOTOR_FOWARD_PIN);
		}
	} else if (motor == RIGHT_MOTOR) {
		if (direction == FORWARD) {
			MOTOR_CONTORL_PORT |= (1 << RIGHT_MOTOR_FOWARD_PIN);
			MOTOR_CONTORL_PORT &= ~(1 << RIGHT_MOTOR_BACKWARD_PIN);
		} else if (direction == BACKWARD) {
			MOTOR_CONTORL_PORT |= (1 << RIGHT_MOTOR_BACKWARD_PIN);
			MOTOR_CONTORL_PORT &= ~(1 << RIGHT_MOTOR_FOWARD_PIN);
		}		
	}
}

/**
 * change the motor speed
 * @motor: the motor you want to toggle. allow param: LEFT_MOTOR、RIGHT_MOTOR
 * @duty_cycle: 0~100
 * @return: output comparator register(OCR) value
 */
uint16_t motorSpeed(uint8_t motor, uint8_t duty_cycle) {
	// fast pwm, 10 bit
	uint16_t ocr_value;
	if (duty_cycle >= 100)
		ocr_value = 1023;
	else 
		ocr_value = (uint32_t)duty_cycle * 1024 / 100;
		
	if (motor == LEFT_MOTOR) {
		LEFT_MOTOR_OCR = ocr_value;
	} else if (motor == RIGHT_MOTOR) {
		RIGHT_MOTOR_OCR = ocr_value;
	}
	return ocr_value;
}

void motorStop(uint8_t motor) {
	if (motor == LEFT_MOTOR) {
		LEFT_MOTOR_OCR = 0;
	} else if (motor == RIGHT_MOTOR) {
		RIGHT_MOTOR_OCR = 0;
	}
}

void motion(uint8_t dir,uint8_t speed)
{
	if (dir == STOP)
	{
		OCR3A = 0;
		OCR3B = 0;
	}
	else if (dir == FORWARD)
	{
		CLRBIT(PORTB, PB0);
		SETBIT(PORTB, PB1);
		CLRBIT(PORTB, PB2);
		SETBIT(PORTB, PB3);
		OCR3A = speed*3;
		OCR3B = speed*3;
	}
	else if (dir == TURN_LEFT)
	{
		CLRBIT(PORTB, PB0);
		SETBIT(PORTB, PB1);
		CLRBIT(PORTB, PB3);
		SETBIT(PORTB, PB2);
		OCR3A = speed*3;
		OCR3B = speed*3;
	}
	else if (dir == BACKWARD)
	{
		CLRBIT(PORTB, PB1);
		SETBIT(PORTB, PB0);
		CLRBIT(PORTB, PB3);
		SETBIT(PORTB, PB2);
		OCR3A = speed*3;
		OCR3B = speed*3;
	}
	else if (dir == TURN_RIGHT)
	{
		CLRBIT(PORTB, PB1);
		SETBIT(PORTB, PB0);
		CLRBIT(PORTB, PB2);
		SETBIT(PORTB, PB3);
		OCR3A = speed*3;
		OCR3B = speed*3;
	}

}
