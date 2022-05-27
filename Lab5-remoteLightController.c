/*
 * RemoteLightController.c
 *
 * Created: 31/03/2022 7:25:07 am
 * Author : qhs7289
 */ 

#define F_CPU 8000000UL//Define the clock speed as 8MHz
#include <avr/io.h>//AT90 Library
#include <util/delay.h>//Delay library

#define lightOn (PORTB |= (1<<6))//Turn light on
#define lightOff (PORTB &= ~(1<<6))//Turn light off

#define rxbufferFull (UCSR1A & (1<<RXC1))//Poll if the receive buffer is full

char mark = 0;//Variable for the width of the high side of the period
char period = 0;//Variable for the counter


void setup(void);//declear setup function

int main(void)
{
	setup();//run setup
    while (1) //Aways loop
    {
		if (rxbufferFull)//if the receive buffer has new data
		{
			switch (UDR1)//Change to the case that matches to the new data
			{
				case 'O':
					mark = 0;//Aways off
					break;
				case 'Q':
					mark = 63;//On for a quarter of the period
					break;
				case 'H':
					mark = 127;//On for half of the period
					break;
				case 'T':
					mark = 191;//On for 3/4 of the period
					break;
				case 'F':
					mark = 255;//Always on
					break;
			}
		}

		if (mark > period)//if the mark is greater than the period
			lightOn;//turn the light on
		else
			lightOff;//otherwise turn the light off
		period++;//Counter increases by 1 through each loop, and resets when counter > 255
		_delay_ms(1);//Small delay
    }
}

void setup(void)
{
	DDRB = 0b01000000;//Set PORTB6 to output
	UCSR1B = 0b00010000;//enable receiver
	UCSR1C = 0b00001110;//2 stop bits, 8 bit character size
	UBRR1L = 51;//Baud rate set to 9600
}