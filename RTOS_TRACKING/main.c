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
#include "kernel.h"

void outputLCD(void) {
	while(1) {
		char buffer[13];
		// 0: ADC0 -> voltage
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
	}
}

void motorControl() {
	while (1) {
		motorDirection(LEFT_MOTOR, FORWARD);
		motorSpeed(LEFT_MOTOR, 10);
		motorDirection(RIGHT_MOTOR, FORWARD);
		motorSpeed(RIGHT_MOTOR, 10);
	}
}

int main (void) {
	// External Interrupt for 5 buttons
	init_Ex_INTn();
	
	// ADC For light sensors
	init_ADC();
	
	// Timer used for collect light sensor data
	init_PWM_time3();
		
	// activate LCD 
 	OpenLCD();
	_delay_ms(2000);
	LCD_CLR();
	
	// ADC For light sensors
	init_ADC();
	
	// Timer used for time sharing
	init_sharing_time1();

 	// Timer for tracking system
	// init_tracking_time2();

	addTask((void*)&outputLCD, NULL);
	addTask((void*)&perform_ADC_conversion, NULL);
	addTask((void*)&motorControl, NULL);

	sei();

	while (1) {	}
	
	return 0;
}




