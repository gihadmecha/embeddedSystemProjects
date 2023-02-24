
#include "DIO_interface.h"
#include "TIMER0.h"
#include "LCD.h"

#define		F_CPU	8000000
#include <util/delay.h>

void timer0OverflowISR ()
{
	static u8 interruptCounter = 0;
	static u8 secondPartsCounter = 0;
	interruptCounter++;
	if (interruptCounter == 122)
	{
		if (secondPartsCounter == 2)
		{
			DIO_TogglePin(PINC1);
		}
		else if (secondPartsCounter == 4)
		{
			DIO_TogglePin(PINC0);
			DIO_TogglePin(PINC1);
			secondPartsCounter = 0;
		}
		DIO_TogglePin(PINC2);
		TIMER0_Set(238);
		interruptCounter = 0;
		secondPartsCounter++;
	}
}

int main()
{
	DIO_Init();
	TIMER0_Init (TIMER0_NORMAL_MODE, TIMER0_PRESCALER_64);
	
	LCD_Init();
	
	sei();
	TIMER0_OverflowInterrupt_Enable ();
	
	TIMER0_overFlowINterruptSetCaller (timer0OverflowISR);
	
	LCD_WriteString("counter");
	unsigned char num = 0;
	TIMER0_Set(238);
	DIO_WritePin(PINC0, LOW);
	DIO_WritePin(PINC1, LOW);
	DIO_WritePin(PINC2, LOW);
	while (1)
	{
		//counter as a main code
		LCD_GoTo(1, 0);
		LCD_WriteNumber(num);
		_delay_ms(200);
		num++;
		if (num == 10)
		{
			num = 0;
		}	
	}
}