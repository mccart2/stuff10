/*
 * GccApplication1.c
 *
 * Created: 4/20/2019 3:10:33 PM
 * Author : Student
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#define ADC0 0x0 // reading
#define AVCC 5000 //5V
#define PRECISION 1024
#define scale (AVCC/PRECISION)// 5000/1024

uint16_t readADC(uint8_t ADCchannel);
int main(void)
{
	uint16_t volt; // declare variable
	DDRB = 0xFF;
	
	//setting the ADC co
	ADMUX |= (1<<REFS0); // AVCC with ext capacitor at AREF pin and input ADC0
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //enable ADC, set pre-scaler to 128
	
	//Setting the timer
	TCCR1A |= (1<<WGM11)|(1<<COM1A1);//fast PWM mode,  toggle on compare
	TCCR1B |= (1<<WGM12) | (1<<WGM13) | (1<<CS10); //1 prescaler
	ICR1 = 0xFFFF; //top clock value
	
	OCR1A = 7999;//calculate OCR1A for 0.001(0) then 0.002 = 15999 (90)
	
	while (1)
	{
		volt = readADC(ADC0); //read voltage from this pin
		volt = ((scale*volt)/10) + 2; //convert the voltage
		_delay_ms(500);
		if (volt <= 5)
		{
			OCR1A = 7999;
		}
		else if (volt>=20)
		{
			OCR1A = 17599; // max degree
		}
		else
		{
			continue;
		}
		_delay_ms(500);
	}
}

uint16_t readADC(uint8_t ADCchannel)
{
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);//reset the ADMUX to adjust reading
	ADCSRA |= (1 << ADSC);//start conversion
	while(ADCSRA & (1<<ADSC));	//Wait until ADC conversation complete. ADSC will be ZERO after completion.
	return ADC; //return ADC value
}