#include <avr/io.h>
#include <avr/interrupt.h>

#define Rate_of_change 1

int button = 0;
int tcnt_value = 0;
int flag = 0, cnt_OVF = 0, isFirst = 1;
int option = 0; //0: decrease, 1: increase

void setPWM();
void setEInterrupt();
void setTC_for_OneSec();

ISR(TIMER0_OVF_vect){
	if(button == 0){
		TCNT0 = 255 - 125; // 125Hz
		tcnt_value = 255 - 125; 
		if(isFirst == 1){
			flag = tcnt_value;
			isFirst = 0;
		}
	}
	else{
		TCNT0 = 255 - 25; // 625Hz
		tcnt_value = 255 - 25;
		if(isFirst == 1){
			flag = tcnt_value;
			isFirst = 0;
		}
	}
}

ISR(TIMER2_OVF_vect){
	if(option == 1){
		if(cnt_OVF < 24){
			cnt_OVF++;
		}
		else if(cnt_OVF == 24){
			cnt_OVF++;
			TCNT2 = 255 - 130;
		}
		else if(cnt_OVF == 25){
			if(flag == 255){
				option = 0;
			}
			else{
				flag += Rate_of_change;
			}
			
			cnt_OVF = 0;
		}
	}
	else{
		if(cnt_OVF < 24){
			cnt_OVF++;
		}
		else if(cnt_OVF == 24){
			cnt_OVF++;
			TCNT2 = 255 - 130;
		}
		else if(cnt_OVF == 25){
			if(flag == tcnt_value){
				option = 1;
			}
			else{
				flag -= Rate_of_change;
			}
			
			cnt_OVF = 0;
		}
	}
}

ISR(INT0_vect){
	button = 0;
	isFirst = 1;
}

ISR(INT1_vect){
	button = 1;
	isFirst = 1;
}

int main(void){
	DDRB |= 0x10; // 0 0 0 1 0 0 0 0 : OC0 출력설정
	
	setEInterrupt();
	setTC_for_OneSec();
	setPWM();
	sei();
	
	while(1){
		OCR0 = flag; //기준값: tcnt_value
	}
}

void setPWM(){ // FastPWM설정
	TCCR0 |= 0x6F; // 0 1 1 0 1 1 1 1
	TIMSK |= 0x01; // 0 0 0 0 0 0 0 1
	TCNT0 = 255 - 125; //125Hz
	tcnt_value = 255 - 125;
	flag = tcnt_value;
}

void setEInterrupt(){ // 외부 인터럽트 설정
	EICRA |= 0x0A; // 0 0 0 0 1 0 1 0
	EIMSK |= 0x03; // EIMSK = EIMSK | ((1 << INT0) | (1 << INT1));
}

void setTC_for_OneSec(){ //Timer/Counter2
	TCCR2 |= 0x04; // 0 0 0 0 0 1 0 0
	TIMSK |= 0x40; // TIMSK = TIMSK | (1 << TOIE2);
}