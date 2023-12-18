#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/iom128.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>

#include "LCD.h"
#include "MOTOR.h"
#include "EX_INT.h"
#include "LIGHT_SENSOR.h"
#include "TRACKING.h"

int main (void) {
	/* For PWM */
	DDRB = 0xFF; //
	DDRE = 0x1F; // LEFT_MOTOR_OCR, RIGHT_MOTOR_OCR

	// activate LCD
	OpenLCD();
	_delay_ms(2000);
	LCD_CLR();
	
	// Timer used for collect light sensor data
	init_PWM_time3();

	// ADC For light sensors
	init_ADC();

	// Timer for tracking system
	// init_tracking_time2();

	// External Interrupt for 5 buttons
	init_Ex_INTn();

	// motorDirection(LEFT_MOTOR, FORWARD);
	// motorSpeed(LEFT_MOTOR, 10);
	// motorDirection(RIGHT_MOTOR, FORWARD);
	// motorSpeed(RIGHT_MOTOR, 10);

	sei();

	while (1) {
		// cli();
		char buffer[13];
		// 0: ADC0 -> middle front
		LCD_Set_Cursor(0,0);
		sprintf(buffer, "V:%4d", sensor_values[0]);
		putsLCD(buffer);

		// 1: ADC1 -> left
		LCD_Set_Cursor(0,8);
		sprintf(buffer, "L:%4d", sensor_values[1]);
		putsLCD(buffer);

		// 2: ADC2 -> middle back
		LCD_Set_Cursor(1,0);
		sprintf(buffer, "M:%4d", sensor_values[2]);
		putsLCD(buffer);

		// 3: ADC3 -> right
		LCD_Set_Cursor(1,8);
		sprintf(buffer, "R:%4d", sensor_values[3]);
		putsLCD(buffer);
		
		// sei();
	}
}




