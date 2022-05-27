/*
 * MusicalTunes.c
 *
 * Created: 6/05/2022 1:22:27 pm
 * Author : qhs7289
 */ 

//Libraries
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//Inputs and outputs
#define button0 !(PINA & (1<<0))
#define button1 !(PINA & (1<<1))
#define button2 !(PINA & (1<<2))
#define buzzerON (DDRB |= (1<<4))
#define buzzerOFF (DDRB &= ~(1<<4))
//Songs names for switch case
#define none 0
#define furElise 1
#define doA_Doe 2
#define lionsSleep 3
//Notes
#define C7 239
#define D7 213
#define E7 190
#define F7 179
#define G7 150
#define A7 142
#define B7 127
#define D8S 101
#define E8 95
//Variables
char FE_state, DAD_state, WMS_state; 
char note, song;
//Functions
void setup(void);
void playNote(char newNote);
ISR(TIMER2_COMPA_vect);

int main(void)
{
	setup();//Run Setup once
    while (1)//Main loop 
    {
		if (button0)//change song depending on what button you press
			song = furElise;
		else if (button1)
			song = doA_Doe;
		else if (button2)
			song = lionsSleep;
		else
			song = none;

		switch(song)//Go to song depending on which button you pressed
			{
				case furElise:
					playNote(E8);//Sequence of notes
					playNote(D8S);
					playNote(E8);
					playNote(D8S);
					playNote(E8);
					playNote(B7);
					playNote(D7);
					playNote(C7);
					playNote(A7);
				break;
				
				case doA_Doe:
					playNote(C7);
					playNote(D7);
					playNote(E7);
					playNote(C7);
					playNote(E7);
					playNote(C7);
					playNote(E7);
				break;
				
				case lionsSleep:
					playNote(F7);
					playNote(G7);
					playNote(A7);
					playNote(G7);
					playNote(A7);
					playNote(B7);
					playNote(A7);
					playNote(G7);
					playNote(F7);
					
					playNote(G7);
					playNote(A7);
					playNote(G7);
					playNote(F7);
					playNote(A7);
					playNote(G7);
				break;
				
				case 0:
					buzzerOFF;
				break;
			}
    }
}

void setup(void)
{
	DDRE = 0x03;//Enable outputs for the muliplexer
	PORTE = 0x01;//Enable buttons
	DDRB = 0b00010000;//Enable output
	TCCR2A = 0b01000000;//Normal mode, toggle 
	TCCR2B = 0b00000010;//1/8 prescale
	TIMSK2 = 0b00000010;//Enable interrupt on A
	sei();//enable global interrupts
}

void playNote(char newNote)
{
	note = newNote;//change note
	buzzerON;//Turn buzzer on
	_delay_ms(300);//Mark
	buzzerOFF;//Turn buzzer off
	_delay_ms(30);//Space
}

ISR(TIMER2_COMPA_vect)
{
		OCR2A += note;//Set frequency
}