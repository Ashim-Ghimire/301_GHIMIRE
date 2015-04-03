/*
 * DA0.asm
 *
 *  Created: 4/2/2015 4:35:17 PM
 *   Author: Aseem
 */ 


 .include "m328pdef.inc"	;includes definition file
 
 .org 0						;

ldi r16, 31					;loads 31 to r16
ldi r17, 36					;loads 36 to r17
adc r16, r17				;adds r16 and r17 with carry and stores to r16
ldi r17, 44					;loads 44 to r17
adc r16, r17				;adds r16 and r17 with carry and stores to r16
ldi r17, 54					;loads 54 to r17
adc r16, r17				;adds r16 and r17 with carry and stores to r16
ldi r17, 59					;loads 59 to r17
adc r16, r17				;adds r16 and r17 with carry and stores to r16
	
brvs HIGH_PORTB2	;branch if overflow flag is set

	LDI R18, 0X04	;loads 4 to r18
	OUT DDRB, R18	; sets direction output to PORTB.2 pin
	LDI R18, 0X00	; loads 0 to r18
	OUT PORTB, R18	; ouputs LOW to PORTB.2 pin
	JMP DONE		; jump to DONE

HIGH_PORTB2:
		ldi R18, 0X04	;loads 4 to r18
		OUT DDRB, R18	;sets direction OUTPUT to PORTB.2 pin
		OUT PORTB, R18	; outputs HIGH to PORTB.2 pin

DONE: JMP DONE		;infinite loop to DONE
