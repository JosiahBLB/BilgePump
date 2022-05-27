/*
 * heaterController.cpp
 *
 * Created: 23/03/2022 6:27:57 pm
 * Author : Josiah Brough
 */ 

//Libraries
#include <avr/io.h>
//I/O's
#define fanON (PORTB |= (1<<5))//Turn fan on
#define fanOFF (PORTB &= ~(1<<5))//Turn fan off
#define heaterON (PORTB |= (1<<7))//Turn heater on
#define displaySwON (PINA & (1<<0))//Toggles the display between two settings
//ADC variables
#define conversionStart (ADCSRA |= (1<<ADSC))//Start the conversion
#define conversionRunning (ADCSRA & (1<<ADSC))//Check if the conversion is running
#define readTemp (ADMUX = 0b01100011)//Read the temperature value on ADC3
#define readVR1 (ADMUX = 0b01100001)//Read the Potentiometer value on ADC1
char VR1 = 0;//Set temperature value
char temp = 0;//Measured temperature value

void setup(void)
{
	DDRA = 0x00;//Set PINA to inputs
	DDRC = 0xFF;//Set PORTC to outputs
	DDRB = 0b10100000;//Set PORT7 and 5
	ADCSRA = 0b10000111;//ADEN, and slowest conversion rate
	ADMUX = 0b01100000;//Set reference voltage and ADLAR
	heaterON;//Turn heater on, this is here because it will stay on
}

int main(void)
{
    setup();
    while (1) 
    {
		readTemp;//Temperature sensor
		conversionStart;//Start conversion of ADC3
		while(conversionRunning);//Wait for the conversion to complete
		temp = ADCH;//Store this value
		
		readVR1;//VR1 is desired temperature
		conversionStart;//Start conversion of ADC31
		while(conversionRunning);//Wait for the conversion to complete
		VR1 = ADCH;//Store this value
		
		if (displaySwON)//If the PINA1 is active
			PORTC = VR1;//Display this value
		else
			PORTC = temp;//Otherwise if its off display this value
		
		if (temp > VR1)//If the measured temperature is greater than the set temperature
			fanON;	//turn the fan on
		else
			fanOFF;//otherwise turn the fan off
    }
}
