/*
 * FanSpeedControl.c
 *
 * Created: 9/05/2022 8:01:02 am
 * Author : qhs7289
 */ 
//Libraries
#include <avr/io.h>//Library for the AT90
#include <avr/interrupt.h>//Library for interrupts
#include <LabBoard.h>//Library for the LCD screen
#include <stdio.h>//Library for the LCD functions
#include <util/delay.h>//Library for delay
//I/O
#define masterON (PINA & (1<<0))//Power switch
//Variables
int rpm;//Variable to hold the ADC value
int line1[20];//Variable for the LCD string
//Functions
void setup(void);


int main(void)//Main function
{
	setup();//Run setup once
    while (1)//Main loop
    {
	if(masterON)//On off switch for system
	{
		OCR1A = ADCH/5.1;//Limit the OCR1A value to 50
	}
	else
	{
		OCR1A = 0;//When the switch is off, the fan is off.
		SLCDDisplayOff();//Turn the screen off
	}
	SLCDHomeCursor();//set cursor to 0,0
	sprintf(line1,"RPM =       ");//Build string to write
	SLCDWriteString(line1);//write the string, clearing te previous reading
	TCNT0 = 0;//Reset the revolution counter
	_delay_ms(250);//Delay a short time
	rpm = (TCNT0/4)*60;//Convert TCNT0 to RPM
	SLCDDisplayOn();//Turn the screen on
	SLCDHomeCursor();//set cursor to 0,0
	sprintf(line1,"RPM = %d", rpm);//Create string for printing
	SLCDWriteString(line1);//Print the string
    }
}

void setup(void)
{	
	DDRB = 0x20;//port 5 output
	ADCSRA = 0b11100111;//Free running mode, slowest speed
	ADMUX = 0b01100001;//Set ref voltage, Left align result, input ADC1
	ICR1 = 49;//Set Period for 50us
	TCCR1A = 0b10000010;//clear on compare match, set at top
	TCCR1B = 0b00011010;//fast PWM with ICR1 as top, 1/8 prescale
	TCCR0B = 0b00000111;//T0 pin for clock input
	SLCDInit();//Initialize the screen
	SLCDClearScreen();//Clear any text currently on the screen
}
