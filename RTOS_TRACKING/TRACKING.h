#ifndef _TRACKING_H
#define _TRACKING_H


extern volatile uint16_t Voltage;
extern volatile uint16_t Line_R;
extern volatile uint16_t Line_M;
extern volatile uint16_t Line_L;
extern volatile uint16_t Stop;
extern volatile uint16_t PWM_turn;
extern volatile uint16_t PWM_move;

/**
 *	Timer2 for tracking line system
 */
void init_tracking_time2(void);

#endif