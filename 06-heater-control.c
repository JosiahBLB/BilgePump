/*
 * HeaterControl.c
 *
 * Created: 6/04/2022 5:56:45 pm
 * Author : qhs7289
 */ 

//Libraries
#include <avr/io.h>
#include <avr/interrupt.h>
//I/O's
#define fanON (PORTB |= (1<<5))//Turn fan on
#define fanOFF (PORTB &= ~(1<<5))//Turn fan off
#define heaterON (PORTB |= (1<<7))//Turn heater on
#define displaySwON (PINA & (1<<0))//Toggles the display between two settings
//ADC variables
#define StartConversion (ADCSRA |= (1<<ADSC))//Start the conversion
#define conversionRunning (ADCSRA & (1<<ADSC))//Check if the conversion is running
#define readTemp (ADMUX = 0b01100011)//Read the temperature value on ADC3
#define readSetTemp (ADMUX = 0b01100001)//Read the Potentiometer value on ADC1
char setTemp, temp, state;//Declare variables for pot value, current temp and state machine state
//Function and interrupts
void setup(void);//declare setup function
ISR(ADC_vect);//declare the ADC interrupt

int main(void)
{
    setup();//Run the Setup function once
    while (1) 
    {
		if (displaySwON)//If the PINA1 is active
			PORTC = setTemp;//Display this value
		else
			PORTC = temp;//Otherwise if its off display this value
		
		if (temp > setTemp)//If the measured temperature is greater than the set temperature
			fanON;	//turn the fan on
		else
			fanOFF;//otherwise turn the fan off
    }
}

void setup(void)
{
	DDRA = 0x00;//Set PINA to inputs
	DDRC = 0xFF;//Set PORTC to outputs
	DDRB = 0b10100000;//Set PORT7 and 5
	DDRE = 0x03;//Set the multiplexer, defaults to switches
	ADCSRA = 0b11001111;//ADEN, and slowest conversion rate, enable interrupt
	ADMUX = 0b01100000;//Set reference voltage and ADLAR
	cli();//Clears global interrupts
	sei();//Enables global interrupts
	heaterON;//Turn heater on, this is here because it will stay on
}

ISR(ADC_vect)//ADC interrupt, this runs when a conversion is complete
{
	switch (state)//State machine for switching between the ADC readings
	{
	case 1:
		temp = ADCH;//Update the temp value
		readSetTemp;//Change the ADMUX to the other ADC
		state = 0;//Change state so it updates the Pot value on ADC1 next time
		StartConversion;//Start the conversion
	break;
		
	case 0:
		setTemp = ADCH;//Update the temp value
		readTemp;//Change the ADMUX to the other ADC
		state = 1;//Change the state so it updates the temperature sensor value on ADC3 next time
		StartConversion;//Start the conversion
	break;
	}
}