
/*
#include "DIO_interface.h"
#include "EXI.h"
#include "TIMER0.h"

#include "LCD.h"

#define F_CPU 8000000
#include <util/delay.h>

static unsigned long overflowCounter = 0;
static unsigned long overflowCounterRequired = 0;
static u8 TCNT0Required = 0;

void EXI_INT0_ISR ()
{
	static u8 changeFlag = 0;
	
	changeFlag = !changeFlag;
	if (changeFlag == 1)
	{
		TIMER0_Set(0);
		overflowCounter = 0;
	}
	else
	{
		TCNT0Required = TCNT0;
		overflowCounterRequired = overflowCounter;
	}
}

void timer0OverflowISR ()
{
	overflowCounter++;
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
	
	unsigned long pressedTime = 0;
	while (1)
	{

		pressedTime = (TCNT0Required + 256 * overflowCounterRequired) * 0.000000125;
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
*/

/**********************************************************************************************/

#include "DIO_interface.h"
#include "EXI.h"
#include "TIMER0_SERVICES.h"

#include "LCD.h"

#define F_CPU 8000000
#include <util/delay.h>

void EXI_INT0_ISR ()
{
	static u8 changeFlag = 0;
	
	changeFlag = !changeFlag;
	if (changeFlag == 1)
	{
		TIMER0_start ();
	}
	else
	{
		TIMER0_stop ();
	}
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
	TIMER0_overflowInterrupt_Init();
	
	LCD_WriteString("counter");
	u8 counter = 0;
	
	LCD_GoTo(0, 9);
	LCD_WriteString("time");
	
	unsigned long pressedTime = 0;
	while (1)
	{

		pressedTime = TIMER0_Read_sec ();
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