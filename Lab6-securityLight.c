/*
 * Security_light_lab_6.c
 *
 * Created: 5/04/2022 3:16:13 pm
 * Author : qhs7289
 */ 
#define F_CPU 8000000UL//Set the clock to 8MHz
#include <avr/io.h>//AT90 library
#include <avr/interrupt.h>//Library for the interrupts
#include <util/delay.h>//Library for the delays

#define lightSwitch (PINA & (1<<0))//Poll to see if input PINA0 is on
#define lightOn (PORTB |= (1<<6))//Turn light on
#define lightOff (PORTB &= ~(1<<6))//Turn light off

char motionTimer = 0;//Counter for the timer

void setup(void);//Setup function
ISR (INT5_vect);//Initialize the interrupt

int main(void)//Main code
{
	setup();//Run Setup once
    while (1) //Main loop
    {
		if (lightSwitch)//If the light switch is on and the on counter is > 0
			lightOn;//Turn light on
		else
		{
			if(motionTimer > 0)//If the motion timer is greater than 0
			{
				lightOn;//Turn the light on
				motionTimer--;//Decrease the counter by 1
				_delay_ms(1000);//Delay of 1 second
			}
			else
				lightOff;//Otherwise turn the light off
		}
	}
}

ISR (INT5_vect)//Interrupt for the motion sensor
{
	motionTimer = 10;//Set the counter to 100
}

void setup(void)//Setup function
{
	DDRB = 0b01000000;//Set PORTB6
	DDRE = 0x03;//Set multiplexer
	cli();//Clear global interrupts
	EIMSK = 0b00100000;//Enable INT5
	EICRB = 0b00001100;//Rising edge for interrupt
	sei();//Enable global interrupts
}


