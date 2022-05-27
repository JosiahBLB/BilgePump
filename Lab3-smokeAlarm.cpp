/*
 * SmokeAlarm_Lab_3.cpp
 *
 * Created: 16/03/2022 6:40:21 pm
 * Author : Josiah Brough
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

//Inputs
#define alarmON (PORTE |= (1<<2))//Turn alarm on
#define alarmOFF (PORTE &= ~(1<<2))//Turn alarm off
//Outputs
#define test !(PINA & (1<<0))//Test button
#define smokeDetect !(PINA & (1<<1))//Smoke detected (button)
//Switch Case
#define Off 0
#define On 1
char alarmState = Off;

void setup(void)
{
	DDRA = 0x00;//Set PORTA to inputs
	DDRE = 0x07;//Set first three bits
	PORTE = 0x01;//Set to PORTA to buttons
}

int main(void)//Main function
{
    setup();//Run setup function once
    while (1) //Runs continuously
    {
		switch (alarmState)//Switch depend on alarm state
		{
		case On://Alarm state on
			_delay_ms(250);//On off on off gives pulsing alarm of 1/4 seconds
			alarmOFF;
			_delay_ms(250);
			alarmON;
			if (!smokeDetect && !test)//If both buttons are not currently pressed go to off state
			alarmState = Off;//Go to alarm off state
			break;
			
		case Off://Alarm state off
			alarmOFF;//Turn alarm off
			if (smokeDetect || test)//If either of the buttons are pressed
			alarmState = On;//Go to alarm state on
			break;
		}
    }
}

