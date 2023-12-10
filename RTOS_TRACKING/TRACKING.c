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

ISR(TIMER2_OVF_vect) {
	Voltage = sensor_values[0] / 5 + 4; 

	if (Voltage < 140) {
		motorStop(LEFT_MOTOR);
		motorStop(RIGHT_MOTOR);
	}
	else {
		if (sensor_values[3] < 300) {		
			motorDirection(LEFT_MOTOR, BACKWARD);
			motorDirection(RIGHT_MOTOR, FORWARD);
			motorSpeed(LEFT_MOTOR, 10);
			motorSpeed(RIGHT_MOTOR, 10);
		} else if (sensor_values[1] < 300) {		
			motorDirection(LEFT_MOTOR, FORWARD);
			motorDirection(RIGHT_MOTOR, BACKWARD);
			motorSpeed(LEFT_MOTOR, 10);
			motorSpeed(RIGHT_MOTOR, 10);
		} else {		
			motorDirection(LEFT_MOTOR, FORWARD);
			motorDirection(RIGHT_MOTOR, FORWARD);
			motorSpeed(LEFT_MOTOR, 10);
			motorSpeed(RIGHT_MOTOR, 10);
		}
	}
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



