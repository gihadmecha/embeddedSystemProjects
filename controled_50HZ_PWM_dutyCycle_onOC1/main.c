
#include "DIO_interface.h"
#include "TIMER1.h"
#include "LCD.h"
#include "ADC.h"

int main()
{
	DIO_Init();
	TIMER1_Init(TIMER1_FAST_PWM_ICR1_TOP_MODE, TIMER1_PRESCALER_8);
	TIMER1_OC1AMode(TIMER1_OC1_CLEAR);
	LCD_Init();
	ADC_Init(ADC_AVCC, ADC_SCALER_64, ADC_HIGHACCURACY);
	ADC_Enable();
	
	TIMER1_TopSet_ICFree(19999);
	
	u16 OCR1AValue = 0;
	u16 ADCRead = 0;
	while (1)
	{
		ADC_startConversion(ADC_CHANNEL7);
		ADCRead = ADC_Read_polling(ADC_CHANNEL7);
		OCR1AValue = ADCRead * 19999.0 / 1023;
		TIMER1_CompareUnit1ASet(OCR1AValue);
		
		LCD_GoTo(0, 0);
		LCD_WriteNumber_4Digit(ADCRead);
		LCD_GoTo(1, 0);
		//LCD_WriteNumber_4Digit(OCR1AValue);
		
		LCD_GoTo(1, 0);
		LCD_WriteNumber(OCR1AValue);
		LCD_WriteString("  ");
	}
}