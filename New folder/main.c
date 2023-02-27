
#include "DIO_interface.h"
#include "TIMER0_SERVICES.h"
#include "LCD.h"

void timer0CompareMatchISR ()
{
	OCR0 = OCR0 + 10;
	DIO_TogglePin(PINC0);
}

int main()
{
	DIO_Init();
	TIMER0_Init(TIMER0_NORMAL_MODE, TIMER0_PRESCALER_8);
	LCD_Init();
	
	sei();
	TIMER0_CompareMatchInterrupt_Enable ();
	
	TIMER0_compareMatchINterruptSetCaller (timer0CompareMatchISR);
	
	OCR0 = 0;
	
	while (1)
	{
		LCD_GoTo(0,0);
		LCD_WriteNumber_4Digit(OCR0);
	}
}