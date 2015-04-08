/*
 * DA2.c
 *
 * Created: 4/5/2015 5:34:03 PM
 *  Author: Aseem
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
void delay_subroutine(void);	//delay subroutine

int main(void)
{
	DDRB = 0XFF;	//set PORTB as output
	DDRC = 0xFF;	//set PORTC as output
	
	PORTB = 0X20;	//sets PORTB.5 to display the waveform with 0.5 period and 50% DC
	
	EIMSK = (1<<INT0);	//enable external interrupt 0
	EICRA |= (1<<ISC11); // rising edge generates an interrupt request
	
	TCNT0 = 0X00; // 8-bit counter 
	sei(); //enable external interrupts
	
	 while(1)
    {
        delay_subroutine();
		PORTB = 0X20; //display the delay on PORTB.5
		TCNT0++;	//8 bit counter counting every rising edge
		
		PORTC = TCNT0;	//displaying the state of counter on PORTC
		
		if((TCNT0 % 5)==0)	//checks if the 5th rising pulse is encountered
		{
			PORTB ^= 0X10;	//toggle PORTB.4 
							//10th bit of the LED bar
		}
		
    }
	return 0;
}


void delay_subroutine(void) {
	/*
	TCNT= 2^16 - (0.25 sec at 8MHz)/ Prescalar
	Prescalar = 2^(-2)*2^3*10^6/2^16 = 30.57
	Use Prescalar greater than 30
	*/
	TCCR1A = 0; //9-bit PWM
	TCCR1B = 0X03; // setting to 011
					//divides the clock by 64
	TCNT1 = 0x85EE;	// TCNT = 34286, tick of system clock
	
	while ((TIFR1&0X01)==0x00)	//loops when Bit0 of TIFR1 is 0
		{
			//loop
		}
		TIFR1 = 0X01; // clears the interrupt flag
					//by writing 1 to it
}

ISR (INT0_vect)
{
	TCNT2++ ;	//Timer counter 2
	if (TCNT2 == 0X0002)	//checks if INT0 occurs
	{
		PORTB ^= (1<<4);	//toggle PC4
		TCNT2 = 0;	//clears counter
	}
}