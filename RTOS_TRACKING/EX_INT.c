#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "MOTOR.h"
#include "LCD.h"
#include "EX_INT.h"

/*
 *
 * Initiate External Interupt Vector
 * PD0 -> INT0, PD1 -> INT1, PE5 -> INT5
 * PE6 -> INT6, PE7 -> INT7
 * 
**/
void init_Ex_INTn(void) {
    // Set INT0 (PD0), INT1 (PD1), INT5 (PE5), INT6 (PE6), and INT7 (PE7) as inputs
	DDRD &= ~((1 << PD0) | (1 << PD1));
	DDRE &= ~((1 << PE5) | (1 << PE6) | (1 << PE7));
	PORTD |= (1 << PD0) | (1 << PD1);
	PORTE |= (1 << PE5) | (1 << PE6) | (1 << PE7);
	
	// Interupt trigger method
    // INT0: Falling Edge
    EICRA &= ~(1 << ISC00);
	EICRA |= (1 << ISC01);
    // INT1: Rising Edge
    EICRA &= (1 << ISC10);
    EICRA |= (1 << ISC11);
    // INT5: Falling Edge
    EICRB &= ~(1 << ISC50);
	EICRB |= (1 << ISC51);
    // INT6: Falling Edge
    EICRB &= ~(1 << ISC60);
    EICRB |= (1 << ISC61);
    // INT7: Falling Edge
    EICRB &= ~(1 << ISC70);
	EICRB |= (1 << ISC71);

	// Clear all interrupt flags in EIFR
    EIFR = (1 << INTF0) | (1 << INTF1) | (1 << INTF5) | (1 << INTF6) | (1 << INTF7);
	
	// Enable external interrupts on INT0, INT1, INT5, INT6, and INT7
    EIMSK |= (1 << INT0) | (1 << INT1) | (1 << INT5) | (1 << INT6) | (1 << INT7);

	// LED next to 5 buttons
	DDRG = 0x1F;
	PORTG = 0x1F;
}

ISR(INT0_vect) {
	// PORTG = 0x1F;
	PORTG &= ~(1 << PG0);
	// PWM = PWM + 5;
	// if (PWM > 250)
	// {
	// 	PWM = 0;
	// }	
	// motion(STOP, 0);
	// Menu_Counter = 0;
	// Menu = 0;
	// _delay_ms(100);	
	// EIFR |= (1<<INTF0);
}

ISR(INT1_vect) {
	// PORTG = 0x1F;
	PORTG &= ~(1 << PG1);
	// if (Menu == 0)
	// {
	// 	motion(1,PWM);
	// }
	// if (++Menu_Counter > 50)
	// {
	// 	Menu = 1;
	// 	Menu_Counter = 0;
	// 	Move_ok = 1;
	// 	LCD_CLR();
	// 	motion(STOP, 0);
	// }
	// _delay_ms(100);	
	// EIFR |= (1<<INTF1);
}


ISR(INT5_vect) {
	// PORTG = 0x1F;
	PORTG &= ~(1 << PG2);
	// if (Menu == 0)
	// {
	// 	motion(TURN_LEFT, PWM);
	// }
	// if (++Menu_Counter > 50)
	// {
	// 	Menu = 2;
	// 	Menu_Counter = 0;
	// 	Move_ok = 1;
	// 	LCD_CLR();
	// 	motion(STOP, 0);
	// }
	// _delay_ms(100);	
	// EIFR |= (1<<INTF5);
}

ISR(INT6_vect) {
	// PORTG = 0x1F;
	PORTG &= ~(1 << PG3);
	// if (Menu == 0)
	// {
	// 	motion(BACKWARD, PWM);
	// }
	// if (++Menu_Counter > 50)
	// {
	// 	Menu = 3;
	// 	Menu_Counter = 0;
	// 	Move_ok = 1;
	// 	LCD_CLR();
	// 	motion(STOP, 0);
	// }
	// _delay_ms(100);	
	// EIFR |= (1<<INTF6);
}

ISR(INT7_vect) {
	// PORTG = 0x1F;
	PORTG &= ~(1 << PG4);
	// if (Menu == 0)
	// {
	// 	motion(TURN_RIGHT, PWM);
	// }
	// if (++Menu_Counter > 50)
	// {
	// 	Move_ok = Move_ok * -1;
	// 	Menu_Counter = 0;
	// 	LCD_CLR();
	// 	motion(0,0);
	// }
	// _delay_ms(100);	
	// EIFR |= (1<<INTF7);
}