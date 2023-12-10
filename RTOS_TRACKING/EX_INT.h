#ifndef _EX_INT_H
#define _EX_INT_H

extern uint8_t PWM;
extern uint8_t Menu;
extern uint8_t Move_ok;
extern uint8_t Menu_Counter;

/*
 *
 * Initiate External Interupt Vector
 * PD0 -> INT0, PD1 -> INT1, PE5 -> INT5
 * PE6 -> INT6, PE7 -> INT7
 * 
**/
void init_Ex_INTn(void);

#endif
