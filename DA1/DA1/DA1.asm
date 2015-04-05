/*
 * DA1.asm
 *
 *  Created: 4/3/2015 6:41:26 PM
 *   Author: Aseem
 */ 

.INCLUDE "m328pdef.inc"		;definition file
			
			; RAMEND = 0x08FF
			; RAMBEGIN = 0x0100
			; RAM MIDDLE = (08FF + 0100)/2 = 0x04FF
.MACRO init_space 
	LDI ZL, LOW(0X04FF)	;stores FF as address to the Z-Low register
	LDI ZH, HIGH(0X04FF) ;stores 04 as address to the Z-high register
.ENDMACRO

init_space		;sets z-pointer to address 0x04ff
	LDI R16, 11				; stores 11 to R16
	LDI R18, 5				; stores 5 to R18

	LDI R19, 25				; counter to count 25 numbers

	
NUMBERS_TO_PARSE:
		ADD R16, R18		;R16= R16 + R18 ; our 25 numbers to be pushed to stack
		ST Z+ , R16			;store numbers to Z stack
		DEC R19				;counter = counter -1
		CPI R19, 0			; compare if the counter has reached 0
		BRNE NUMBERS_TO_PARSE	;if not zero, keep putting numbers 
								;to the stack until the stack has 25 numbers

init_space		;resets the Z-pointer to 0x04ff
	LDI R19, 25		;re-initialize the counter to 25

NEW_NUMBER:
	LD R16, Z+		;load the value Z-register is pointing to in R16
	MOV R17, R16	;move it to a temporary register R17
	SUBI R17, -3	;add 3 to the number

;loop to see if the number is div by 3
DIV_THREE:
	SUBI R17, 3		;subtract 3 to the number
	BRLO DIV_SEVEN	;if R17<0 => not divisible by3 => check if divisible by 7
	CPI R17, 0		;check if R17 = 0
	BRNE DIV_THREE	;if not zero, keep looping

;loop to sum the numbers divisible by 3
SUM_THREE:		
	ADC R24, R16	;add with carry the LSB to R24
	BRCC DIV_SEVEN	;branch if no carrry to check if the num is also div by 7
	INC R23			;if carry is set, increment the MSB by 1

;loop to see if the number is div by 7
DIV_SEVEN:
	MOV R17, R16	;move the popped number to a temp register R17
	SUBI R17, -7	;add 7 to the number
DIV_SEVEN_LOOP:
	SUBI R17, 7	;subtract 7 to the number
	BRLO NEXT_NUMBER ;if R17<0 => not divisible by => get new number
	CPI R17, 0		;check if R17 = 0
	BRNE DIV_SEVEN_LOOP  ; if not zero, keep looping

;loop to sum the numbers divisible by 7
SUM_SEVEN:
	ADC R21, R16	;add with carry the LSB to R21
	BRCC NEXT_NUMBER	;branch if no carry to get next number
	INC R20			;if carry is set, increment the MSB by 1

NEXT_NUMBER:
	SUBI R19, 1		;decrement counter 
	CPI R19, 0		;check if the 25 numbers have been parsed
	BRNE NEW_NUMBER	;if not, get the next number

DONE: JMP DONE	;Infinite loop after
				;those 25 numbers are calculated

