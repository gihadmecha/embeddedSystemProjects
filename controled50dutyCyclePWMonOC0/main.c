
#include "DIO_interface.h"
#include "TIMER0_SERVICES.h"
#include "ADC.h"
#include "LCD.h"

int main()
{
	DIO_Init();
	TIMER0_Init(TIMER0_CTC_MODE, TIMER0_PRESCALER_8);
	TIMER0_OCMode (TIMER0_OC0_TOGGLE);
	ADC_Init(ADC_AVCC, ADC_SCALER_64, ADC_HIGHACCURACY);
	LCD_Init();
	ADC_Enable();
	
	u16 ADCRead = 0;
	u8 OCR0Value = 0;
	while(1)
	{
		ADC_startConversion (ADC_CHANNEL7);
		ADCRead = ADC_Read_polling(ADC_CHANNEL7);
		
		OCR0Value = 255 - (ADCRead * 255.0 / 1023);
		TIMER0_CompareUnitSet(OCR0Value);
		
		LCD_GoTo(0, 0);
		LCD_WriteNumber_4Digit(ADCRead);

		LCD_GoTo(1,0);
		LCD_WriteNumber_4Digit(TIMER0_OCR0Read());
	}
}