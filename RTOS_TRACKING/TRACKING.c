#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

#include "TRACKING.h"
#include "LIGHT_SENSOR.h"
#include "MOTOR.h"
#include "EX_INT.h"
#include "LCD.h"

volatile uint16_t Voltage = 200;
volatile uint16_t Line_R = 0;
volatile uint16_t Line_M = 0;
volatile uint16_t Line_L = 0;
volatile uint16_t Stop = 0;
volatile uint16_t PWM_turn = 0;
volatile uint16_t PWM_move = 0;

// static char* test = "a"; 
ISR(TIMER2_OVF_vect)
{
	motorDirection(LEFT_MOTOR, FORWARD);
	motorSpeed(LEFT_MOTOR, 100);
	motorDirection(RIGHT_MOTOR, FORWARD);
	motorSpeed(RIGHT_MOTOR, 10);
	// Voltage = sensor_values[0] / 5 + 4; 
	// Line_R = sensor_values[3] * 9 / 11;
	// Line_M = sensor_values[2] * 9 / 11;
	// Line_L = sensor_values[1] * 9 / 11;
	// PWM_turn = 185 - Voltage;
	// PWM_move = 185 - Voltage;

	// if ((Voltage < 140) && (LEFT_MOTOR_OCR == 0 || RIGHT_MOTOR_OCR == 0)) {
	// 	motion(STOP, 0);
	// 	PWM = 0;
	// }

	// if (Voltage > 140) {
	// 	if (Move_ok == 1)
 	// 	{		
	// 		motion(STOP, 0);
	// 	}
	// 	else if (Move_ok > 1)
 	// 	{
	// 		if (Line_M < 300) {		
	// 			motion(FORWARD, PWM_move);
	// 			Stop = 0;
	// 		} else if (Line_R < 300) {		
	// 			motion(TURN_RIGHT, PWM_turn);
	// 			Stop = 0;
	// 		} else if (Line_L < 300) {		
	// 			motion(TURN_LEFT, PWM_turn);
	// 			Stop = 0;
	// 		} else
	// 			Stop++;

	// 		if(Stop > 80) {
	// 			motion(BACKWARD, PWM_move);
	// 			Stop++;
	// 			if(Stop > 100 && Stop < 105)
	// 				motion(STOP, 0);
	// 			else if(Stop > 105) {
	// 				motion(TURN_RIGHT, PWM_turn);
	// 				Stop = 0;
	// 			}			
	// 		}
	// 	}
	// }
}


/**
 *	init timer2 for tracking line system
 */
void init_tracking_time2(void) {
	// Normal Mode, Prescalar: System Clock / 1024
    TCCR2 = (1 << CS22) | (1 << CS20);

    // Enable Timer/Counter 2 overflow interrupt
    TIMSK |= (1 << TOIE2);

}



