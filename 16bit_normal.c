#include <avr/io.h>
#include <avr/interrupt.h>

void setTC();
int flag = -1;

ISR(TIMER1_OVF_vect){
	flag *= -1;
	TCNT1 = 49910; // {(2^16) - 1} - (16000000/1024)
}

int main(void){
	DDRA |= 0x01;
	sei();
	setTC();
		
	while (1) {
		if(flag == 1){
			PORTA |= 0x01;
		}
		else{
			PORTA &= 0x00;
		}
	}
}

void setTC(){
	TCCR1A |= 0x00;
	TCCR1B |= 0x05;
	TCCR1C |= 0x00;
	
	TIMSK = TIMSK | (1 << TOIE1);
	TCNT1 = 49910; // {(2^16) - 1} - (16000000/1024)
}


