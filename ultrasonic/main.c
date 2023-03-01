
#include "DIO_interface.h"
#include "TIMER1.h"
#include "LCD.h"

static volatile u8 edgeFlag = 0;
static volatile u16 firstEdgeICR = 0;
static volatile u16 secondEdgeICR = 0;
static void TIMER1_ICP1_ISR()
{
	if (edgeFlag == 0)
	{
		firstEdgeICR = ICR1;
		TIMER1_ICU_Edge (TIMER1_ICU_FALLING);
		edgeFlag = 1;
	}
	else if (edgeFlag == 1)
	{
		secondEdgeICR = ICR1;
		edgeFlag = 2;
		TIMER1_InputCaptureInterrupt_Disable ();
	}
}

int main()
{
	DIO_Init();
	TIMER1_Init(TIMER1_NORMAL_MODE, TIMER1_PRESCALER_8);
	LCD_Init();
	TIMER1_ICU_Edge (TIMER1_ICU_RAISING);
	sei();
	TIMER1_InputCaptureInterrupt_Enable ();
	TIMER1_InputCaptureINterruptSetCaller (TIMER1_ICP1_ISR);
	
	u16 time = 0;
	u8 distance = 0;
	while (1)
	{
		DIO_WritePin(PIND3, HIGH);
		_delay_us(10);
		DIO_WritePin(PIND3, LOW);
		
		while (edgeFlag < 2);

		time = secondEdgeICR - firstEdgeICR;
		distance = time / 58;
		edgeFlag = 0;
		TIMER1_InputCaptureInterrupt_Enable ();
		TIMER1_ICU_Edge (TIMER1_ICU_RAISING);
		
		LCD_GoTo(0, 0);
		LCD_WriteNumber(distance);
	}
}