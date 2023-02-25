
#include "DIO_interface.h"
#include "TIMER0.h"
#include "ADC.h"
#include "POTENIOMETER.h"
#include "LCD.h"

int main ()
{
	DIO_Init();
	ADC_Init (ADC_AVCC , ADC_SCALER_64 , ADC_HIGHACCURACY);
	TIMER0_Init(TIMER0_FAST_PWM_MODE, TIMER0_PRESCALER_1);
	LCD_Init();
	sei();
	ADC_Enable();
	ADC_interruptEnable();
	TIMER0_OCMode (TIMER0_OC0_CLEAR);
	u8 potentiometer = 0;
	u8 ocr0Value = 0;
	while (1)
	{
		ADC_startConversion (ADC_CHANNEL7);
		potentiometer = POTENIOMETER_GetPresentage();
		LCD_GoTo(0, 0);
		LCD_WriteNumber_4Digit(potentiometer);
		ocr0Value = 256 * potentiometer / 100; 
		TIMER0_CompareUnitSet(ocr0Value);
		LCD_GoTo(1, 0);
		LCD_WriteNumber_4Digit(ocr0Value);
	}
}