
#include "DIO_interface.h"
#include "lCD.h"
#include "EXI.h"
#include "TIMER0_SERVICES.h"

#define  F_CPU	8000000
#include <util/delay.h>

static u8 edgeFlag = 0;

void EXI_INT0_ISR ()
{
	if (edgeFlag == 0)
	{
		TIMER0_start ();
		edgeFlag = 1;
		EXI_INT0Init(EXI_FALLING_EDGE);
	}
	else if (edgeFlag == 1)
	{
		TIMER0_stop ();
		TIMER0_start ();
		EXI_INT0Init(EXI_RAISING_EDGE);
		edgeFlag = 2;
	}
	else if (edgeFlag == 2)
	{
		TIMER0_stop ();
		edgeFlag = 3;
	}
}

int main()
{
	DIO_Init();
	LCD_Init();
	EXI_INT0Init(EXI_RAISING_EDGE);
	TIMER0_Init(TIMER0_NORMAL_MODE, TIMER0_PRESCALER_1);
	
	sei();
	EXI_INT0Enable ();
	TIMER0_OverflowInterrupt_Enable ();
	
	EXI_INT0SetCallBack (EXI_INT0_ISR);
	TIMER0_overflowInterrupt_Init();
	
	LCD_WriteString("count");
	
	LCD_GoTo(0, 7);
	LCD_WriteString("duty");
	LCD_GoTo(0, 13);
	LCD_WriteString("Fre");
	
	u8 counter = 0;
	unsigned long T_on = 0;
	unsigned long T_off = 0;
	unsigned long T = 0;
	unsigned int dutyCycle = 0;
	unsigned int frequency = 0;
	
	while (1)
	{
		if (edgeFlag == 2)
		{
			T_on = TIMER0_Read_us ();
		}
		else if (edgeFlag == 3)
		{
			T_off = TIMER0_Read_us ();
			
			T = T_on + T_off;         //us
			dutyCycle = ((double)T_on / T) * 100;
			frequency = 1 / (T / 1000000);      //HZ
		}
		
		LCD_GoTo(1, 7);
		LCD_WriteNumber(dutyCycle);
		LCD_GoTo(1, 13);
		LCD_WriteNumber(frequency);
		
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