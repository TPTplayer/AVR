#include <avr/io.h>
#include <avr/interrupt.h>

int no14 = 1, no15 = -1;
int freq = 0;
int isFirst_mode = 1, isFirst_freq = 1;

ISR(INT0_vect){ //mode
	no14 *= -1;
	no15 *= -1;
	
	isFirst_mode = 1;
}

ISR(INT1_vect){ //freq1 (50Hz)
	freq = 5000;
	
	isFirst_freq = 1;
}

ISR(INT2_vect){ //freq2 (100Hz)
	freq = 2500;
	
	isFirst_freq = 1;
}

ISR(INT3_vect){ //freq3 (1KHz)
	freq = 250;
	
	isFirst_freq = 1;
}

void setTC1(); //Fast PWM: No.14
void setTC2(); //Fast PWM: No.15
void setEINT(); //External Interrupt Set Function

void setFreq(int val, int mode_num);
void getDuty(int val, int mode_num);

void pinSet();

int main(void){
	pinSet();
	DDRD &= 0xF0; //for External Interrupt
	DDRA |= 0x01; //for LED
	setEINT();
	sei();
	
	while(1){
		/*mode change*/
		if(no14 == 1 && isFirst_mode == 1){
			setTC1();
			getDuty(freq, 14);
			isFirst_mode = 0;
			
			PORTA &= 0xFE; //LED off
		}
		else if(no15 == 1 && isFirst_mode == 1){
			setTC2();
			getDuty(freq, 15);
			isFirst_mode = 0;
			
			PORTA |= 0x01; //LED on
		}
		
		/*select frequency*/
		if(isFirst_freq == 1 && no14 == 1){
			setFreq(freq, 14);
			getDuty(freq, 14);	
		}
		else if(isFirst_freq == 1 && no15 == 1){
			setFreq(freq, 15);
			getDuty(freq, 15);
		}
	}
}

void setFreq(int val, int mode_num){
	if(mode_num == 14){ 
		ICR1 = val;
	}	
	else if(mode_num == 15){
		OCR1A = val;
	}
}

void getDuty(int val, int mode_num){
	if(mode_num == 14){
		OCR1A = val/10; //10%
		OCR1B = val/2; //50%
		OCR1C = (val/10)*8; // 80%
	}
	else if(mode_num == 15){ 
		OCR1B = val/2; //50%
		OCR1C = (val/10)*8; // 80%
	}
}

void setEINT(){
	EICRA |= 0xAA;
	EIMSK |= 0x0F;		
}

void setTC1(){
	TCCR1A &= 0x00;
	TCCR1B &= 0x00;
	TCCR1C &= 0x00;
	
	TCCR1A |= 0xAA;
	TCCR1B |= 0x1B;
	TCCR1C |= 0x00;
	
	/*
	mode: Fast PWM(TOP: ICR1)
	compare match: falling edge(non-inverting)
	prescaler: 64
	output pin: OC1A, OC1B, OC1C
	*/
}

void setTC2(){
	TCCR1A &= 0x00;
	TCCR1B &= 0x00;
	TCCR1C &= 0x00;
	
	TCCR1A |= 0xAB;
	TCCR1B |= 0x1B;
	TCCR1C |= 0x00;
	
	/*
	mode Fast PWM(TOP: OCR1A)
	compare match: falling edge(non-inverting)
	prescaler: 64
	output pin: OC1A, OC1B, OC1C
	*/
}

void pinSet(){
	DDRA |= 0xFF;
	DDRB |= 0xFF;
	DDRC |= 0xFF;
	DDRD |= 0xFF;
	DDRE |= 0xFF;
	DDRF |= 0xFF;
	DDRG |= 0x1F;
	
	PORTA |= 0x00;
	PORTB |= 0x00;
	PORTC |= 0x00;
	PORTD |= 0x00;
	PORTE |= 0x00;
	PORTF |= 0x00;
	PORTG |= 0x00;
}