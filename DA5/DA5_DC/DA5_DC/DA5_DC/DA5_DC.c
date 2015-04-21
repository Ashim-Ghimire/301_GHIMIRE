/*
 * DA5_DC.c
 *
 * Created: 4/14/2015 11:43:41 AM
 *  Author: ghimirea
	DC Motor
 */ 

#define F_CPU 8000000UL //XTAL = 8MHZ
#include <avr/io.h>
#include <util/delay.h>
//#define ENABLE 0

void adc_init()
{
	ADCSRA = (1<<ADEN) | (1<<ADPS1);	//enable ADC
	//ADMUX = 0X00;	
}

void adc_read()
{
	ADCSRA |= (1<<ADSC);	//analog to digital
	while ((ADCSRA&(1<<ADIF))== 0)
	{	//wait for the conversion
		
	}

}
void delay_pot()
{
	while ((TIFR1 & (1<<OCF1A)) == 0)	//control delay
	{
		
	}
	TIFR1 |= (1<<OCF1A);
}
int main()
{
	DDRB = 0xFF; //make PORTB output pin
	DDRC = 0x00; //make PORTC input
	
	TCCR1B = 0x0D;	//sets WGM12 = 1; CS12 = 1 and CS10 = 1
	
	adc_init();	//initialize PC.0 as input
	
	while(1)
	{
		
		adc_read();	//converts analog to digital
		
		OCR1A = 950;	//set a constant value for output compare register
		
		delay_pot();	//delay
		
		if (ADC<90){
			ADC = 90;	//set min to 90
				
		} 
		
		if (ADC>500){
			ADC = 500;	//set max to 500
			
		}
		
		OCR1A = ((ADC-80)*3.8) + 280;	//value from to potentiometer
		
		PORTB ^= 0x01;	//Toggle PB.0
		
		delay_pot();	//delay
		
		
		PORTB ^= 0x01;	//Toggle PB.0
	}
	return 0 ;
}