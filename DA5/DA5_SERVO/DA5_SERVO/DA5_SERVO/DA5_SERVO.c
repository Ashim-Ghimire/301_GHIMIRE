/*
 * DA5_SERVO.c
 *
 * Created: 4/20/2015 2:24:31 PM
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
	ADMUX = 0X00;
}

void adc_read()
{
	ADCSRA |= (1<<ADSC);	//analog to digital
	while ((ADCSRA&(1<<ADIF))== 0)
	{	//wait for the conversion
		
	}
}



int main(void)
{
	DDRB = 0xFF; //make PORTB output pin
	DDRC = 0x00; //make PORTC input
	
	//TOP = ICR1;
	//output compare OC1A 8 bit non inverted PWM
	//Clear OC1A on Compare Match, set OC1A at TOP
	//Fast PWM
	//ICR1 = 19999 defines 50Hz pwm
	ICR1=19999;
	TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(0<<COM1B1)|(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);
	TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|
	(0<<CS12)|(1<<CS11)|(0<<CS10);
	
	adc_init();	//enables ADC 
	
    while(1)
    {
        adc_read();	//convert analog to digital
		
		if (ADC<90){
			ADC = 620;	//set min to 620
			
		}
		if (ADC>500){
			ADC = 2100;	//set max to 2100
		}
		
		OCR1A = ((ADC-80)*3.8) + 620;	//value from to potentiometer
		
    }
}