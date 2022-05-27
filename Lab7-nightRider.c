/*
 * NightRider.c
 *
 * Created: 13/04/2022 6:15:15 pm
 * Author : qhs7289
 */
 
//Libraries
#include <avr/io.h>//AT90 Library
#include <avr/interrupt.h>//Interrupt library
//Inputs
#define button0 !(PINA & (1<<0))//Poll if button 1 is pushed
#define button1 !(PINA & (1<<1))//Poll if button 2 is pushed
#define button2 !(PINA & (1<<2))//Poll if button 3 is pushed
//Variables
#define growShrink 0 //LED sequence 1
#define rightScroll 1 //LED sequence 2
#define lineBounce 2 //LED sequence 3
char modeSelect, button; //Variables for if and switch statements
unsigned char LEDstate, tick;//Timing, unsigned sets it to 0-255

ISR(TIMER0_COMPA_vect);//Initializing counter compare interrupt
void setup(void);//Initializing setup function
void sequence(char LEDState1, char LEDState2, char LEDState3, char LEDState4 , char LEDState5, char LEDState6, char LEDState7, char LEDState8);//Initializing Sequence function

int main(void)//Main function
{
	setup();//Run setup once
    while (1)//Do the following on repeat
    {
		if(button0)//If button 1 is pushed
			modeSelect = growShrink;//Set mode to the grow and shrink LED sequence
		else
			if(button1)//If button 2 is pushed
			modeSelect = rightScroll;//Set mode to the right scrolling LED sequence
		else
			if(button2)//If button 3 is pushed
			modeSelect = lineBounce;//Set mode to the bouncing line LED sequence
		
		switch(modeSelect)//Read which mode is selected and go to that case
		{	
			case growShrink://Play grow and shrink sequence
				sequence(0b00010000, 0b00111000, 0b01111100, 0b11111110, 0b11111111, 0b01111110, 0b00111000, 0b00010000);//Each state of the sequence sent to the function
				break;
			case rightScroll://Play right scrolling sequence
				sequence(0b11100000, 0b01110000, 0b00111000, 0b00011100, 0b00001110, 0b00000111, 0b10000011, 0b11000001);//Each state of the sequence sent to the function
				break;
			case lineBounce://Play bouncing line sequence
				sequence(0b11110000, 0b01111000, 0b00111100, 0b00011110, 0b00001111, 0b00011110, 0b00111100, 0b01111000);//Each state of the sequence sent to the function
				break;
		}
    }
}

void setup(void)
{
	DDRC = 0xFF;//Set PORTC to outputs
	TCCR0B = 0b00000100;//Set the prescaler to 256
	TIMSK0 = 0b00000010;//Enable output compare interrupt
	DDRE = 0x03;//Set PIN 2-0 of the multiplexer to outputs
	PORTE = 0b00000001;//Chose the buttons for inputs
	sei();//enable global interrupts
}

void sequence(char LEDState1, char LEDState2, char LEDState3, char LEDState4 , char LEDState5, char LEDState6, char LEDState7, char LEDState8)//Sequence function, receives 8 states of the sequence
{
	switch (LEDstate)//Checks what the current state of sequence out of 8 options is, and sets PORTC to display the LED's of that state.
	{
		case 0:
			PORTC = LEDState1;
			break;
		case 1:
			PORTC = LEDState2;
			break;
		case 2:
			PORTC = LEDState3;
			break;
		case 3:
			PORTC = LEDState4;
			break;
		case 4:
			PORTC = LEDState5;
			break;
		case 5:
			PORTC = LEDState6;
			break;
		case 6:
			PORTC = LEDState7;
			break;
		case 7:
			PORTC = LEDState8;
			break;

	}
}

ISR(TIMER0_COMPA_vect)//Timer compare match interrupt
{
	OCR0A = OCR0A + 250;//Each time 250 is reached 250 is added to the current value, this can overflow and will interrupt within another 250 cycles.
	tick++;//Add one to the compare match counter
	if(tick > 30)//If it is greater than 30, meaning that that 1/4 of a second has passed
	{
		tick = 0;//Set compare match counter to 0
		LEDstate++;//Add one to the LED counter. This is for cycling through 8 different states within the Switch case function
	}
	if (LEDstate > 7)//If the LED counter has gone through all 8 states
		LEDstate = 0;//Reset the counter
}