
#include "DIO_interface.h"
#include "TIMER1.h"
#include "LCD.h"

static volatile u8 edgeFlag = 0;
static volatile u16 TCNT1Read1 = 0;
static volatile u16 TCNT1Read2 = 0;
static volatile u16 TCNT1Read3 = 0;
void TIMER1_ICP1_Interrupt ()
{
	if (edgeFlag == 0)
	{
		edgeFlag = 1;
		TIMER1_ICU_Edge (TIMER1_ICU_FALLING);
		TCNT1Read1 = ICR1;
	}
	else if (edgeFlag == 1)
	{
		edgeFlag = 2;
		TIMER1_ICU_Edge (TIMER1_ICU_RAISING);
		TCNT1Read2 = ICR1;
	}
	else if (edgeFlag == 2)
	{
		edgeFlag = 3;
		TCNT1Read3 = ICR1;
	}
}

int main()
{
	DIO_Init();
	TIMER1_Init(TIMER1_NORMAL_MODE, TIMER1_PRESCALER_8);
	LCD_Init();

	sei();
	TIMER1_InputCaptureInterrupt_Enable ();
	TIMER1_InputCaptureINterruptSetCaller (TIMER1_ICP1_Interrupt);
	
	TIMER1_ICU_Edge (TIMER1_ICU_RAISING);

	TCNT1 = 0;
	u16 T_on = 0;
	u16 T_off = 0;
	u16 T = 0;
	u8 dutyCycle = 0;
	unsigned int frequency = 0;
	while (1)
	{
		if (edgeFlag == 3)
		{
			T_on = TCNT1Read2 - TCNT1Read1;
			T_off = TCNT1Read3 - TCNT1Read2;
			T = T_on + T_off;
			dutyCycle = (T_on * 1.0 / T) * 100.0;
			frequency = 1.0 / (T / 1000000.0); 
			
			LCD_GoTo(0, 0);
			LCD_WriteNumber(T_on);
			LCD_WriteString("  ");
			LCD_GoTo(0, 6);
			LCD_WriteNumber(T_off);
			LCD_WriteString("  ");
			LCD_GoTo(0, 11);
			LCD_WriteNumber(T);
			LCD_WriteString("  ");
			
			LCD_GoTo(1, 0);
			LCD_WriteNumber_4Digit((u32)dutyCycle);
			LCD_GoTo(1, 7);
			LCD_WriteNumber_4Digit((u32)frequency);
		}
	}
}