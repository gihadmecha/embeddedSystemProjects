
#include "DIO_interface.h"
#include "TIMER0_SERVICES.h"
#include "LCD.h"

int main()
{
	DIO_Init();
	TIMER0_Init(TIMER0_CTC_MODE, TIMER0_PRESCALER_8);
	TIMER0_OCMode (TIMER0_OC0_TOGGLE);
	LCD_Init();
	
	u8 availableFrequency = TIMER0_generatePWM_CTC_50dutyCycle (5100);
	LCD_GoTo(0, 0);
	LCD_WriteNumber(availableFrequency);
    
	LCD_GoTo(1, 0);
	LCD_WriteNumber(OCR0);
	
	while (1)
	{
	}
	
}