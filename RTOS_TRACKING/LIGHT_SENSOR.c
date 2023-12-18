#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

#include "LIGHT_SENSOR.h"


// 0: ADC0 -> votage; 1: ADC1 -> left; 2: ADC2 -> middle; 3: ADC3 -> right
volatile int16_t sensor_values[4] = {0, 0, 0, 0};

/**
 * Init ADC for light sensors
*/
void init_ADC(void) {
	// Set AVCC as the reference voltage
	ADMUX = (1 << REFS0);
	
	// Enable ADC, and set ADC prescalar to 128
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	// Clear the ADIF flag
	ADCSRA |= (1 << ADIF);
	
	// start conversion
	ADCSRA |= (1 << ADSC);

	// First conversion takes longer
    while (!(ADCSRA & (1 << ADIF))) {
        // Polling: Wait until ADIF becomes 1
    }

    // Clear the ADIF flag
	ADCSRA |= (1 << ADIF);
}


/**
 * Perform ADC Conversion and Update Sensor Values
 */
void perform_ADC_conversion(void) {
	
	while (1) {
		uint16_t tmp[4] = {0, 0, 0, 0};
		for (uint8_t i = 0; i < SAMPLE_COUNT; ++i) {
			for (uint8_t j = 0; j < SENSOR_COUNT; ++j) {
				ADCSRA |= (1 << ADSC);
				// Wait for the conversion to complete using ADIF flag
				while (!(ADCSRA & (1 << ADIF))) {}
				// Clear the ADIF flag by writing 1 to it
				ADCSRA |= (1 << ADIF);
				// Update sensor_values array
				// tmp[ADMUX & 0x03] += (uint16_t)(ADCL | ((uint16_t)ADCH << 8));
				tmp[j] += (uint16_t)ADCL | ((uint16_t)ADCH << 8);
				// Move to the next sensor (cycling through 0 to 3)
				ADMUX = (ADMUX & 0xFC) | ((ADMUX + 1) & 0x03);
			}
		}
		
		for (uint8_t i = 0; i < SAMPLE_COUNT; ++i) {
			sensor_values[i] = tmp[i] / SAMPLE_COUNT;
			tmp[i] = 0;
		}
		
	}
}