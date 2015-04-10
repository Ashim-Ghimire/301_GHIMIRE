/*
 * DA4.c
 *
 * Created: 4/9/2015 3:37:55 PM
 *  Author: ghimirea
 */ 


#define F_CPU 8000000L
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>


void delay(void){
	//holds the colors for this time
	TCNT1 = 0;
	while(TCNT1 < 0x4e20){	//count until 20000 in decimal 
		TCNT1++;
	}
}

void PWM_init(void)
{
	TCCR0A = (1<<WGM00) | (1<<WGM01) | (1<<COM0B1) | (1<<COM0A1); // Non-inverting Fast PWM
	TCCR0B = (1<<WGM02) | (1<<CS01); // no prescalar
	
	TCCR2A = (1<<WGM20) | (1<<WGM21) | (1<<COM2B1); // Non inverting Fast PWM
	TCCR2B = (1<<WGM22) | (1<<CS21); // no prescalar 
}

int main(void)
{
	DDRD = (1<<PORTD3) | (1<<PORTD5) | (1<<PORTD6); //set PORTB to display RGB LED
	int duty = 0;
	
	PWM_init();	//generates PWM
	
	while(1){
		for (duty=20; duty < 230; duty++){	//alter duty cycle for RED
			OCR0A = duty;	//output compare register 
			
			delay();	//delay subroutine
		}
		OCR0A = 0;	//clears the PWM
		
		for (duty=20; duty < 230; duty++){	//alter duty cycle for GREEN
			OCR0B = duty;
			delay();	//delay subroutine
		}
		OCR0B = 0;	//clears the PWM
		
		for (duty=20; duty < 230; duty++){	//alter duty cycle for BLUE
			OCR2B = duty;	
			delay();	//delay subroutine
		}
		OCR2B = 0;	//clears PWM
	}
	
	return 0;
}


