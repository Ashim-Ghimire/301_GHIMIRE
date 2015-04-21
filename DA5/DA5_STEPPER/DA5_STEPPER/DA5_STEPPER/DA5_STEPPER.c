/*
 * DA5_STEPPER.c
 *
 * Created: 4/18/2015 1:36:05 PM
 *  Author: ghimirea
 */ 

#define F_CPU 8000000UL //XTAL = 8MHZ
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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

int main(void)
{
	DDRB = 0xFF; //make PORTB output pin
	DDRC = 0x00; //make PORTC input
	TCCR1B = 0x0D;	//sets WGM12 = 1; CS12 = 1 and CS10 = 1
	
	adc_init();
	
    while(1)
    {
        adc_read();	//converts analog to digital
		
		if (ADC<90){
			ADC = 90;	//set min to 90
			
		}
		if (ADC>500){
			ADC = 500;	//set max to 500
		}
		
		OCR1A = ((ADC-80)*3.8) + 280;	//value from to potentiometer
		
		PORTB = 0X66;	//01100110 sequence
		delay_pot();
		PORTB = 0X33;	//00110011 sequence
		 
		delay_pot();
		PORTB = 0X99;	//10011001 sequence
	
		delay_pot();
		PORTB = 0XCC;	//11001100 sequence
		delay_pot();
    }
	return 0;
}