
#include "DIO_interface.h"
#include "EXI.h"
#include "TIMER0.h"

#include "LCD.h"


#define F_CPU 8000000
#include <util/delay.h>

static u8 changeFlag = 0;
static unsigned long long interruptsCounter = 0;
static u8 readFlag = 1;

void EXI_INT0_ISR ()
{
	changeFlag = !changeFlag;
	if (changeFlag == 1)
	{
		TIMER0_Set(0);
		interruptsCounter = 0;
	}
	else
	{
		readFlag = 0;
	}
}

void timer0OverflowISR ()
{
	interruptsCounter++;
}

int main ()
{
	DIO_Init();
	EXI_INT0Init(EXI_ANY_LOGICAL_CHANGE);
	TIMER0_Init(TIMER0_NORMAL_MODE,TIMER0_PRESCALER_1);
	
	LCD_Init( );
	
	sei();
	EXI_INT0Enable ();
	TIMER0_OverflowInterrupt_Enable ();
	
	EXI_INT0SetCallBack (EXI_INT0_ISR);
	TIMER0_overFlowINterruptSetCaller (timer0OverflowISR);
	
	LCD_WriteString("counter");
	u8 counter = 0;
	
	LCD_GoTo(0, 9);
	LCD_WriteString("time");
	
	static unsigned int pressedTime = 0;
	while (1)
	{
		if (readFlag == 0)
		{
			pressedTime = (TCNT0 + 256 * interruptsCounter) * 0.000000125;
			readFlag = 1;
		}
		LCD_GoTo(1, 9);
		LCD_WriteNumber(pressedTime);
		
		LCD_GoTo(1, 0);
		LCD_WriteNumber(counter);
		_delay_ms(200);
		counter++;
		if (counter == 10)
		{
			counter = 0;
		}
	}
}