#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

#include "LCD.h"


// 0: ADC0 -> middle front; 1: ADC1 -> left; 2: ADC2 -> middle back; 3: ADC3 -> right
volatile int16_t sensor_values[4] = {0, 0, 0, 0};
static volatile uint8_t is_first_conversion = 1;

/**
 * Init ADC for light sensors
*/
void init_ADC(void) {
	// Set AVCC as the reference voltage
	ADMUX = (1 << REFS0);
	
	// Enable ADC,enable ADC interrupt, and set ADC prescaler to 128
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	// start conversion
	ADCSRA |= (1 << ADSC);
}

// ADC Conversion Complete Interrupt Service Routine
ISR(ADC_vect) {
	if (is_first_conversion) {
		ADCSRA |= (1 << ADSC);
		is_first_conversion = 0;
		return;
	}

    sensor_values[ADMUX & 0x03] = (int16_t)ADCL;
	sensor_values[ADMUX & 0x03] += (int16_t)((uint16_t)ADCH << 8); 

    if ((ADMUX & 0x03) == 3) {


    }

    // Move to the next sensor (cycling through 0 to 3)
    ADMUX = (ADMUX & 0xFC) | ((ADMUX + 1) & 0x03);

	// start next conversion
	ADCSRA |= (1 << ADSC);
}