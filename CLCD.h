#ifndef CLCD_H_
#define CLCD_H_


#define RS 0x04
#define RW 0x02
#define E 0x01

#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02

#define ENTRY_NO_SHIFT 0b00000110
#define ENTRY_SHIFT_LEFT 0b0000111
#define ENTRY_SHIFT_RIGHT 0b0000101

/*
Entry mode
S = 1 : move screen
I/D = 1: cursor move right
*/

#define DISPLAY_ON 0b00001100 
#define DISPLAY_OFF 0b00001000 
#define DISPLAY_CURSOR_ON 0b00001010 
#define DISPLAY_BLINK_ON 0b00001001

#define SHIFT_DISPLAY_CURSOR_RIGHT 0b00010100
#define SHIFT_DISPLAY_CURSOR_LEFT 0b00010000 
#define SHIFT_DISPLAY_SCREEN_RIGHT 0b00011000
#define SHIFT_DISPLAY_SCREEN_LEFT 0b00011100 

#define CLCD_CG_RAM_BASE 0x40 //0100 0000
#define CLCD_DD_RAM_BASE 0x80 //1000 0000

#define FUNC_DATA_LINE_4 0b00100000
#define FUNC_DATA_LINE_8 0b00110000
#define FUNC_DISPLAY_LINE_1 0b00100000
#define FUNC_DISPLAY_LINE_2 0b00101000
#define FUNC_FONT_SMALL 0b00100000
#define FUNC_FONT_BIG 0b00100100

void CLCD_DataTransmitter(char data);
void CLCD_Controller(char ctl);
void CLCD_initalizer(void);
void CLCD_putstr(char addr, char *str);

#endif