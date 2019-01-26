#include <avr/io.h>
#include <avr/interrupt.h>

int freq_10 = 1, freq_50 = -1;
int ctc_4 = 1, ctc_12 = -1;
int isFirst1 = 1, isFirst2 = 1;

void setTC_CTC1(); //TOP: OCR
void setTC_CTC2(); //TOP: ICR

void setEINT();

ISR(INT0_vect){ //change frequency
	freq_10 *= -1;
	freq_50 *= -1;
	
	isFirst2 = 1;
}

ISR(INT1_vect){ //change ctc mode
	ctc_4 *= -1;
	ctc_12 *= -1;
	
	isFirst1 = 1;
}

int main(void){
	DDRB |= 0x20; //OC1A Output set
	DDRA |= 0x01; //for CTC check LED
	
	sei();
	setEINT();
	
    while (1) {
		if(ctc_4 == 1 && isFirst1 == 1){
			setTC_CTC1();
			
			PORTA &= 0x00; //LED OFF: TOP == OCR
			isFirst1 = 0;
		}
		else if(ctc_12 == 1 && isFirst1 == 1){
			setTC_CTC2();
			
			PORTA |= 0x01; //LED ON: TOP == ICR
			isFirst1 = 0;
		}
		
		if(freq_10 == 1 && isFirst2 == 1){
			OCR1A = 12500;
			ICR1 = 12500;
			
			isFirst2 = 0;
		}
		else if(freq_50 == 1 && isFirst2 == 1){
			OCR1A = 2500;
			ICR1 = 2500;
			
			isFirst2 = 0;
		}
	}
}

void setEINT(){
	EICRA |= 0x0A; //(External)INT0, INT1 enable
	EIMSK |= 0x03; 
}

void setTC_CTC1(){
	TCCR1A &= 0x00;
	TCCR1B &= 0x00;
	TCCR1C &= 0x00;
	
	TCCR1A |= 0x40;
	TCCR1B |= 0x0B;
	TCCR1C |= 0x00;
	
	/*
	mode: CTC (TOP: OCR)
	Compare Output Mode: toggle 
	Prescaler: 64 (max: 250000)
	*/
	
	OCR1A = 12500; //10Hz
}

void setTC_CTC2(){
	TCCR1A &= 0x00;
	TCCR1B &= 0x00;
	TCCR1C &= 0x00;
	
	TCCR1A |= 0x40;
	TCCR1B |= 0x1B;
	TCCR1C |= 0x00;
	
	/*
	mode: CTC (TOP: ICR)
	Compare Output Mode: toggle
	Prescaler: 64 (max: 250000)
	*/
	
	ICR1 = 12500; //10Hz
}