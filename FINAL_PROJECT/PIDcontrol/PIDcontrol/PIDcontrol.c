///PID controller of DC MOTOR


#define F_CPU 8000000UL
#define P_GAIN 0.8
#define I_GAIN 0.005
#define D_GAIN 0.01

#define BAUD 9600
#define BRC ((F_CPU/16/BAUD) - 1) //Asynchrnonous Normal Mode
//calculating Baud Rate to feed into UBBR
#define TX_BUFFER_SIZE 128

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>


double dutycycle = 0;
int set_rpm = 0,previous_error = 0;
int error, feedback_rpm = 0 , output;
int D_error = 0, I_error = 0;
int cliflag = 0;

unsigned int pulses;
char buffer[999];

void usart_init(unsigned int ubrr)	//initialize usart
{
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// Set frame format: 8data, 2stop bit
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void usart_send (unsigned char ch)	//send data to terminal
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = ch;
}

void USART_tx_string(char * data)	//send a string to the terminal
{
	while ((*data != '\0'))
	{
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = *data;
		data++;
	}
}


int main(void)
{
	
	DDRB = 0xFF;	//Set output to PORTB
	PORTB = 0x02;	//Constant 10 to PORTB1, PORTB0
	
	set_rpm = 100;
	
	
	usart_init(BRC);
	
	_delay_ms(100);
	snprintf(buffer,sizeof(buffer), "Displaying the motor speed in RPM\n\r");
	//USART_tx_string("Displaying the motor speed in RPM\n\r");	//Display the String
	USART_tx_string(buffer);
	
	PORTD = (1<<PORTD3); // sets 1 to INT1 to trigger interrupt on falling edge
	EIMSK = (1<<INT1); //enables the external interrupts
	//sei();
	DDRD = (1<<PORTD6); //send PWM
	//since we're dealing with only OC0A in the register, we're only gonna look at 0A bit of timer
	//use fast pwm
	
	TCCR0A = (1<<COM0A1) | (1<<WGM00) | (1<<WGM01); //Clears OC0A on compare Match, set OC0A at BOTTOM
	//TOP value is 0xff i.e. 255 for a 8 bit timer
	TIMSK0 = (1<<TOIE0); // interrupt set when there is an overflow
	OCR0A = (dutycycle/100.0)*255;	//fraction of 255 representing the amount of time the PWM is on
	sei();
	TCCR0B = (1<<CS00);	//No prescalar. THIS ONE ACTUALLY STARTS THE TIMER.
	
	previous_error = set_rpm - feedback_rpm;
	while (1)
	{
		
		
		dutycycle += 10;
		
		error = set_rpm - feedback_rpm;
		I_error += (error);
		D_error = (error - previous_error);
		
		output = (P_GAIN * error) + (I_GAIN * I_error) + (D_GAIN * D_error);
		previous_error = error;
		
		snprintf(buffer,sizeof(buffer), "The current speed is: %d RPM \r\n\n", OCR0A);
		//send the message through USART
		USART_tx_string(buffer);
		
		if(OCR0A + output > 255){
			OCR0A= 255;
		}
		else if(OCR0A + output < 0){
			OCR0A= 0;
		}
		else{
			OCR0A += output;
		}
		
		if(dutycycle >100)
		{
			dutycycle = 0;
		}
	}
}
ISR (TIMER0_OVF_vect)
{
	OCR0A =(dutycycle/100.0)* output; //it recalculates the duty cycle making PWM dynamic
}

ISR (INT1_vect)
{
	
	pulses++;
	feedback_rpm = (pulses/64)*60;
}

