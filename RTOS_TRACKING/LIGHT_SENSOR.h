#ifndef _LIGHT_SENSOR_H
#define _LIGHT_SENSOR_H

#define SAMPLE_COUNT 5
#define SENSOR_COUNT 4

// 0: ADC0 -> votage; 1: ADC1 -> left; 2: ADC2 -> middle; 3: ADC3 -> right
extern volatile int16_t sensor_values[4];

/**
 * Init ADC for light sensors
 */
void init_ADC(void);
/**
 * collect the light sensor data
 */
void perform_ADC_conversion(void);

# endif // _LIGHT_SENSOR_