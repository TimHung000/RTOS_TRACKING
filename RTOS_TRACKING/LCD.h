#ifndef _LCD_H
#define _LCD_H

#include <inttypes.h>

void OpenLCD(void);

void WriteCmdLCD( uint8_t LCD_CMD);

void WriteDataLCD( uint8_t LCD_CMD);

void putcLCD(uint8_t LCD_Char);

void LCD_CMD_W_Timing( void );

void LCD_DAT_W_Timing( void );

void LCD_Set_Cursor(uint8_t CurY, uint8_t CurX);

//void putrsLCD( const rom char *Str );

void putsLCD( char *Str);

void puthexLCD(uint16_t HEX_Val);

void LCD_L_Delay(void);

void LCD_S_Delay(void);

void LCD_CLR(void);

void putsi(int val,uint8_t field_length);

#endif // _LCD_H
