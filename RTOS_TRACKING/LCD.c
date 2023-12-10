#include <avr/io.h>
#include <avr/iom128.h>
#include <util/delay.h>
#include <inttypes.h>
#include "LCD.h"


/*
 *    I/O ports control LCD data
 *    PORTA[4:7]-->DB[4:7] : Higher order 4 lines data bus with bidirectional
 *					       : DB7 can be used as a BUSY flag
 *    PORTA0 -> LCD-RS     : LCD Toggle Command/Data Register bit (0, command mode; 1, Data mode)
 *    PORTA1 -> LCD-RW     : LCD Toggle Read/Write bit (0, write operation; 1, read operation)
 *    PORTA2 -> LCD-E      : LCD Enable bit
**/

#define LCD_RS			PA0
#define LCD_RW			PA1
#define LCD_E			PA2
#define DIR_LCD_RS		DDA0
#define DIR_LCD_RW		DDA1
#define DIR_LCD_E		DDA2

#define LCD_DATA		PORTA		// PORTA[4:7] as LCD DB[4:7]
#define DIR_LCD_DATA	DDRA

//  LCD Module commands
#define DISP_2Line_8Bit	0x38		// 0b00111000
#define DISP_2Line_4Bit	0x28		// 0b00101000
#define DISP_ON			0x0C		// Display on
#define DISP_ON_C		0x0E		// Display on, Cursor on
#define DISP_ON_B		0x0F		// Display on, Cursor on, Blink cursor
#define DISP_OFF		0x08		// Display off
#define CLR_DISP		0x01		// Clear the Display
#define ENTRY_INC		0x06		//
#define ENTRY_INC_S		0x07		//
#define ENTRY_DEC		0x04		//
#define ENTRY_DEC_S		0x05		//
#define DD_RAM_ADDR		0x80		// Least Significant 7-bit are for address
#define DD_RAM_UL		0x80		// Upper Left coner of the Display	

unsigned char 	Temp_CMD ;
unsigned char 	Str_Temp ;
unsigned char	Out_Mask ;

void OpenLCD(void)
{
	uint8_t tmpData;
	DIR_LCD_DATA = DIR_LCD_DATA | 0xF7;	// LCD DB[4:7] & RS & R/W & E are output function
	tmpData = PORTA;
	tmpData &= ~((1<<LCD_E) | (1<<LCD_RS) | (1<<LCD_RW));
	PORTA = tmpData;
	LCD_DATA = 0x00;					// LCD DB[4:7] & RS & R/W --> Low

	_delay_ms(15);						// Set E pin as output

	LCD_DATA = 0x30;	//0b00110000   // Setup for 4-bit Data Bus Mode
	LCD_CMD_W_Timing();
	_delay_ms(6);

	LCD_DATA = 0x30;	//0b00110000 
	LCD_CMD_W_Timing();
	_delay_us(100);

	LCD_DATA = 0x30;	//0b00110000
	LCD_CMD_W_Timing();
	_delay_ms(1);

	LCD_DATA = 0x20;	//0b00100000 ;
	LCD_CMD_W_Timing();
	_delay_ms(1);

	WriteCmdLCD(DISP_2Line_4Bit);
	_delay_ms(1);

	WriteCmdLCD(DISP_ON);
	_delay_ms(1);

	WriteCmdLCD(CLR_DISP);
	_delay_ms(1);

	WriteCmdLCD(ENTRY_INC);
	_delay_ms(1); 		
}

//***********************************************
// Write Command to LCD module
//
void WriteCmdLCD( uint8_t LCD_CMD) 
{
	Temp_CMD = (LCD_CMD & 0xF0);
	LCD_DATA = (LCD_DATA & 0x0F)|Temp_CMD;
	LCD_CMD_W_Timing();
	Temp_CMD = (LCD_CMD & 0x0F)<<4;			// Send low nibble to LCD bus
	LCD_DATA = (LCD_DATA & 0x0F)|Temp_CMD;
	LCD_CMD_W_Timing ();

	_delay_ms(1);
}

//***********************************************
// Write Data to LCD module
//
void WriteDataLCD( uint8_t LCD_CMD) 
{
	Temp_CMD = (LCD_CMD & 0xF0);
	LCD_DATA = (LCD_DATA & 0x0F)|Temp_CMD;
	LCD_DAT_W_Timing();

	Temp_CMD = (LCD_CMD & 0x0F)<<4;			// Send low nibble to LCD bus
	LCD_DATA = (LCD_DATA & 0x0F)|Temp_CMD;
	LCD_DAT_W_Timing();

	_delay_ms(1);
}

void putcLCD(uint8_t LCD_Char)
{
	WriteDataLCD(LCD_Char);
}

void LCD_CMD_W_Timing( void )
{
	uint8_t tmpTiming;
	tmpTiming = PORTA;
	tmpTiming &= ~((1<<LCD_RS)|(1<<LCD_RW));	// Set for Command Input
	tmpTiming |= (1<<LCD_E);
	PORTA = tmpTiming;
	asm volatile("nop\n\t""nop\n\t"::);			//Nop();
	tmpTiming &= ~(1<<LCD_E);
	PORTA = tmpTiming;
}

void LCD_DAT_W_Timing( void )
{
	uint8_t tmpTiming;
	tmpTiming = PORTA;
	tmpTiming |= (1<<LCD_RS);					// Set for Data Input
	tmpTiming &= ~(1<<LCD_RW);
	tmpTiming |= (1<<LCD_E);
	PORTA = tmpTiming;
	asm volatile("nop\n\t""nop\n\t"::);			//Nop();
	tmpTiming &= ~(1<<LCD_E);
	PORTA = tmpTiming;
}

//***********************************************
//     Set Cursor position on LCD module
//			CurY = Line (0 or 1)
//      	CurX = Position ( 0 to 15)
//
void LCD_Set_Cursor(uint8_t CurY, uint8_t CurX)
{
	WriteCmdLCD( 0x80 + CurY * 0x40 + CurX);
	_delay_us(150);
}

//***********************************************
//    Put a RAM string to LCD Module
//
void putsLCD( char *Str)
{
	while (1)
	{
		Str_Temp = *Str;

		if (Str_Temp != 0x00 )
		{
			WriteDataLCD(Str_Temp);
			Str ++;
		}
		else
			return;
	}
}

// *****************************************************************************
// Display HEX Number on LCD 
//******************************************************************************
void puthexLCD(uint16_t HEX_Val)
{
	uint16_t Temp_HEX;

	Temp_HEX = (HEX_Val >> 12) & 0x0f;
	if ( Temp_HEX > 9 )Temp_HEX += 0x37;
    else Temp_HEX += 0x30;
	WriteDataLCD(Temp_HEX);

	Temp_HEX = (HEX_Val >> 8) & 0x0f;
	if ( Temp_HEX > 9 )Temp_HEX += 0x37;
    else Temp_HEX += 0x30;
	WriteDataLCD(Temp_HEX);

	Temp_HEX = (HEX_Val >> 4) & 0x0f;
	if ( Temp_HEX > 9 )Temp_HEX += 0x37;
    else Temp_HEX += 0x30;
	WriteDataLCD(Temp_HEX);

	Temp_HEX = HEX_Val  & 0x0f;
	if ( Temp_HEX > 9 )Temp_HEX += 0x37;
    else Temp_HEX += 0x30;
	WriteDataLCD(Temp_HEX);
}

// *****************************************************************************
// Long Delay 
//******************************************************************************
void LCD_L_Delay(void)
{
	uint8_t i, j;
	for (i=0; i<255; i++)	
	{
		for (j=0; j<255; j++)	
		{
			asm volatile("nop\n\t");
		}
	}		
}

// ****************************************************************************
// Short Delay
//*****************************************************************************
void LCD_S_Delay(void)
{
	uint8_t i, j;
	for (i=0; i<10; i++)	
	{
		for (j=0; j<255; j++)	
		{
			asm volatile("nop\n\t");
		}
	}		
}

//*****************************************************************************
//clear display  subroutine
//*****************************************************************************
void LCD_CLR(void)
{
   WriteCmdLCD(0x01);     //  this clear display all 
   _delay_ms(1);
 }

//****************************************************************************
//   number  display     
//****************************************************************************
void putsi(int val,uint8_t field_length)    //val display num  field for cloumd
{
	char str[5] = {0,0,0,0,0};
	uint8_t i = 4, j = 0;
	while(val)
	{
		str[i] = val % 10;
		val = val / 10;
		i--;
	}
	if(field_length > 51)
	{
		while(str[j]==0) j++;
	}
	else
	{
		j = 5 - field_length;
	}

	if(val<0)
	{
		WriteDataLCD('-');
	}
	for(i=j;i<5;i++)
	{
    	WriteDataLCD('0'+str[i]);
	}
}

