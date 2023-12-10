#ifndef _LIGHT_SENSOR_H
#define _LIGHT_SENSOR_H

// 0: ADC0 -> middle front; 1: ADC1 -> left; 2: ADC2 -> middle back; 3: ADC3 -> right
extern volatile int16_t sensor_values[4];

/**
 * Init ADC for light sensors
 */
void init_ADC(void);

# endif // _LIGHT_SENSOR_