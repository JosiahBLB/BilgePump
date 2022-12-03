/*
 * TouchLamp.c
 *
 * Created: 14/05/2022 6:15:10 pm
 * Author : qhs7289
 */ 
#define F_CPU 8000000UL//Clock speed for delays
#include <avr/io.h>//Library for AT90
#include <util/delay.h>//Library for delays
#include <LabBoard.h>//Library for the lab boards
#include <stdio.h>//Library for the LCDs
//Input
#define touch (PINA & (1<<0))
//States
#define off 0
#define one_third 1
#define two_thirds 2
#define full 3
//Variables
char state = 0, output[20];

//Functions
void setup(void);

int main(void)
{
	setup();//Run setup once
    while (1)//Main loop
    {
		if (touch)//If lamp touched
		{
			_delay_ms(100);//Debounce
			while(touch);//Debounce
			state++;//Go to next state
			if(state > 3)//If the state exceeds maximum, go to start
				state = 0;//starting state
		}
		switch(state)//Switch between lighting states
		{
			case off://Off state
				OCR1B = 0;//Mark is 0
				SLCDClearScreen();//Clear screen
				sprintf(output,"Brightness: %d", ADCH);//Save the string
				SLCDHomeCursor();//Reset cursor position
				SLCDWriteString(output);//Print string to the LCD
			break;
			case one_third://1/3 brightness
				OCR1B = ICR1/3;//Mark is 1/3 of period
				SLCDClearScreen();//Clear screen
				sprintf(output,"Brightness: %d", ADCH);//Save the string
				SLCDHomeCursor();//Reset cursor position
				SLCDWriteString(output);//Print string to the LCD
			break;
			case two_thirds://2/3 brightness
				OCR1B = (ICR1*2)/3;//Mark is 2/3 of period
				SLCDClearScreen();//Clear screen
				sprintf(output,"Brightness: %d", ADCH);//Save the string
				SLCDHomeCursor();//Reset cursor position
				SLCDWriteString(output);//Print string to the LCD
			break;
			case full://full brightness
				OCR1B = ICR1;//Always on
				SLCDClearScreen();//Clear screen
				sprintf(output,"Brightness: %d", ADCH);//Save the string
				SLCDHomeCursor();//Reset cursor position
				SLCDWriteString(output);//Print string to the LCD
			break;
		}
    }
}

void setup(void)
{
	DDRE = 0x03;//Enable multiplexer
	PORTE = 0x01;//Choose buttons
	DDRB = 0b01000000;//Enable light as output
	ADCSRA = 0b11100111;//Enable ADC, start conversion, free running mode
	ADMUX = 0b01100000;//Set reference voltage, left align result
	TCCR1A = 0b00100010;//Set channel b to set on compare match and reset at top
	TCCR1B = 0b00011010;//Fast PWM with ICR1 as top
	ICR1 = 900;//Top value for ICR1
	SLCDInit();//Initialize the screen
	SLCDDisplayOn();//Turn display on
	SLCDClearScreen();//Clear any text currently on the LCD
}