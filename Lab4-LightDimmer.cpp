/*
 * LightDimmer.cpp
 *
 * Created: 23/03/2022 5:01:11 pm
 * Author : qhs7289
 */ 
//Libraries
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
//Outputs
#define lightOn (PORTB |= (1<<6))
#define lightOFF (PORTB &= ~(1<<6))
//Inputs
#define lightSwitch (PINA & (1<<0))
//Duty cycle variables
#define mark ADCH
char period = 0;

void setup(void)
{
	DDRA = 0x00;//PINA1 input
	DDRB = 0b01000000;//PORTB6 output
	ADCSRA = 0b11100111;//ADC on, Start conversion, free running mode on, slowest conversion speed
	ADMUX = 0b01100010;//Set ref voltage, left adjust result, set ADC2
}

int main(void)
{
    setup();//Run setup once
    while (1)//Loop the following code
    {
		if (!lightSwitch)//If the light switch is off, turn the light off.
			lightOFF;
		else if (mark > period)//If the value set by the pot is larger than the period turn the light on
			lightOn;
		else//Otherwise turn it off. This will change the duty cycle creating a dimming effect.
			lightOFF;
		period++;//Once period reaches 255, it will overflow and restart from 0
    }
}