/*
 * DA3.c
 *
 * Created: 4/8/2015 12:13:40 PM
 *  Author: Aseem
 */ 

 #define F_CPU 8000000UL	//Clock speed
 #define BAUD 9600
 #define MYUBRR (F_CPU/(16*BAUD))-1
 
 #include <avr/io.h>
 #include <util/delay.h>
 #include <stdio.h>
 #include <inttypes.h>
 #include <avr/interrupt.h>
 
 void convert_to_digital(){
	 //start analog to digital conversion
	 ADCSRA |= (1<<ADSC);
	 while ((ADCSRA&(1<<ADIF))==0){
		 //wait for conversion to finish
	 }
 }
 
 void usart_init() {
	
	//enable receiver and transmitter
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	//set frame format: 8bit, 
	UCSR0C = (1<<UCSZ00) | (3<<UCSZ01);
	//set baud rate
	UBRR0 = 0x0033;
 }
 
 void usart_send(uint8_t ch){
	 //wait for buffer to be ready to write to
	 while(!(UCSR0A &(1<<UDRE0)));
	 UDR0 = ch;
 }
 
 int main(void)
 {
	DDRC = 0x00;	//input porb
	 DDRD = 0xFF;	//output port
	 
	 //variable for temperatures
	 	
	 //enable ADC, division factor 4
	ADCSRA = (1<<ADEN)|(1<<ADPS1); 
	 //REF0 and REF1 for "AVcc with external capacitor on AREF pin" mode
	ADMUX= 0X00;
	
	 usart_init();
	 
	 TCCR1A = 0X00;//gives system clock
	 TCCR1B = 0X0D;//enable CTC mode, prescalar set to 1024	
	 OCR1A = 0X1E84; //8MHz/1024 = 0x1E84
	 
	 TIMSK1 |= (1<<OCIE1A);//calls interrupt function
	 sei();
	 
	 while(1){
		  }
		  return 0;
 }
 
 ISR(TIMER1_COMPA_vect){
	 
	 uint8_t temp;	//temperature
	 convert_to_digital(); //converts to digital
	 temp = ADC;	//save to temp variable
	 usart_send(temp);	//sends to terminal
	 
 }