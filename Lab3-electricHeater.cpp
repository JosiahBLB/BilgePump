/*
 * Electric_Heater_Lab_3.cpp
 *
 * Created: 17/03/2022 9:21:54 pm
 * Author : Josiah Brough
 */ 

#include <avr/io.h>

//Inputs
#define powerSwitch (PINA & (1<<0))//Isolator
#define tiltSwitch (PINA & (1<<1))//Tilt safety mechanism
#define highLowSwitch (PINA & (1<<2))//High low toggle
//Outputs
#define elementHigh (PORTC = 0x03)//Both elements on
#define elementLow (PORTC = 0x01)//One element on
#define elementOff (PORTC = 0x00)//Both elements off
//Switch case
#define off 0
#define on 1
#define tilt 2
char heaterState = off;

void setup(void)//Setup function
{
	DDRA = 0x00;//PINA all inputs
	DDRC = 0x03;//PORTC bit 0 and 1 set
	DDRE = 0x03;//This is here just so switches are chosen
}
int main(void)//Main function
{
	setup();//Run setup once
    while (1)//Runs continuously
    {
		switch (heaterState)//Choose case depending on which state heater is in
		{
		case on://When heater state is set to on
			if (tiltSwitch)//if the tilt switch is activated
			heaterState = tilt;//go to tilt state
			if (!powerSwitch)//If the power is turned off
			heaterState = off;//Go to off state
			if (highLowSwitch)//If the high low switch is on
			elementHigh;//Set both elements on
			else
			elementLow;//Otherwise set only one element on
			break;
		
		case off://When heater state is set to off
			elementOff;//Turn both elements off
			if (powerSwitch)//If the power switch is turned on
			heaterState = on;//Go to on state
			break;
			
		case tilt://When heater state is set to tilt
			elementOff;//Turn element off
			if (!tiltSwitch)//If the tilt switch is turned off
			heaterState = off;//Go to off state
			break;
		}
    }
}