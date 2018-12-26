#include <avr/io.h>
#include <avr/interrupt.h>

int button = 0;
int tcnt_value;

void setPWM();
void setEInterrupt();

ISR(TIMER0_OVF_vect){
	if(button == 0){
		TCNT0 = 255 - 250; //2KHz
		tcnt_value = 255 - 250; 
	}
	else{
		TCNT0 = 255 - 25; //20KHz
		tcnt_value = 255 - 250;
	}
}

ISR(INT0_vect){
	button = 0;
}

ISR(INT1_vect){
	button = 1;
}

int main(void){
	int i = 0;
	DDRB |= 0x10; // 0 0 0 1 0 0 0 0 : OC0 출력설정
	
	setEInterrupt();
	setPWM();
	sei();
	
	while(1){
		OCR0 = tcnt_value;
			
		for(i = tcnt_value; i < 0xFF*120; i++){
			if((i - tcnt_value) % 120 == 0){
				OCR0++;
			}
		}
			
		for(i = tcnt_value; i < 0xFF*120; i++){
			if((i - tcnt_value) % 120 == 0){
				OCR0--;
			}
		}
	}
}

void setPWM(){ // FastPWM설정
	TCCR0 |= 0x6B; // 0 1 1 0 1 0 0 1
	TIMSK |= 0x01; // 0 0 0 0 0 0 0 1
	TCNT0 = 255 - 250; //2KHz
}

void setEInterrupt(){ // 외부 인터럽트 설정
	EICRA |= 0x0A; // 0 0 0 0 1 0 1 0
	EIMSK |= 0x03; // EIMSK = EIMSK | ((1 << INT0) | (1 << INT1));
}
