#include <avr/io.h>
#include <util/delay.h>

#include "CLCD.h"

/*
A2: RS
A1: RW
A0: E
*/

/*It is a function based on 8bit data bus.*/

void CLCD_DataTransmitter(char data){
	_delay_ms(1);
	PORTA = RS; 
	_delay_us(1); 
	PORTA = (RS | E); 
	_delay_us(1);
	PORTC = data; 
	_delay_us(1);
	PORTA = RW;	
}

void CLCD_Controller(char ctl){
	_delay_ms(30);
	PORTA = 0x00; //RW clear
	_delay_us(1);
	PORTA = E;
	_delay_us(1);
	PORTC = ctl;
	_delay_us(1);
	PORTA = (RS | RW);
}

void CLCD_initalizer(void){
	_delay_ms(50);
	CLCD_Controller(FUNC_DISPLAY_LINE_2 | FUNC_DATA_LINE_8);
	_delay_us(40);
	CLCD_Controller(DISPLAY_ON);
	_delay_us(40);
	CLCD_Controller(CLEAR_DISPLAY);
	_delay_ms(2);
	CLCD_Controller(ENTRY_NO_SHIFT);
}
/*
void CLCD_input_font(char *font, int font_num){
	for(int i = 0; i < font_num; i++){
		CLCD_Controller(CLCD_CG_RAM_BASE | font);
		
	}
}
*/

void CLCD_putstr(char addr, char *str){
	int char_count = 0;
	
	CLCD_Controller(addr | CLCD_DD_RAM_BASE);
	while(*str != '\0'){
		if(((addr | CLCD_DD_RAM_BASE) + char_count) == 0x90){
			CLCD_Controller(CLCD_DD_RAM_BASE | 0x40);
		}
		CLCD_DataTransmitter(*str);
		
		str += 1;
		char_count += 1;
	}
}

