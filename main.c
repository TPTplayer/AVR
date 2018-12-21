#include <avr/io.h>
#include <avr/interrupt.h>

int button = 0;
int flag = 0;

ISR(TIMER0_COMP_vect){ //compare match 인터럽트 루틴
	if(flag == 0){
		flag = 1;
	}
	else{
		flag = 0;
	}
}

ISR(INT0_vect){ //1번
	button = 0;
}

ISR(INT1_vect){ //2번
	button = 1;
}


void setting_CTC(void); //CTC세팅
void setting_EInterrupt(void); //외부 인터럽트 세팅

int main(void){
	DDRA |= 0xFF;
	DDRB |= 0xFF;
	
	setting_CTC();
	setting_EInterrupt();
	
	sei(); // global interrupt enable
	
	while(1){
		if(button == 0){ //1KHz
			OCR0 = 250;
		}
		else if(button == 1){ //10KHz
			OCR0 = 25;
		}
		
		if(flag == 1){
			PORTA |= 0xFF;
		}
		else{
			PORTA &= 0x00;
		}
	}
}

void setting_CTC(void){
	TCCR0 |= 0x1B; // 0 0 0 1 1 0 1 1
	TIMSK |= 0x02; // 0 0 0 0 0 0 1 0
	//TIMSK = TIMSK | (1 << OCIE0); 
	
	OCR0 = 250; //1KHz
}

void setting_EInterrupt(void){
	EICRA |= 0x0A; //0 0 0 0 1 0 1 0 (0, 1번을 falling edge시 인터럽트)
	EIMSK |= 0x03; //0 0 0 0 0 0 1 1 (0, 1번의 인터럽트 허용)
}

