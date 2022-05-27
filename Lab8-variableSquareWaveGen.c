/*
 * VariableSquareWaveGen.c
 *
 * Created: 6/05/2022 11:52:41 am
 * Author : qhs7289
 */ 

//Libraries
#include <avr/io.h>
#include <avr/interrupt.h>
//Variable
char VR1;
//Functions
void setup (void);
ISR(TIMER0_COMPA_vect);

int main(void)//Main Function
{
	setup();//Run setup once
    while (1) 
    {
	}
}

void setup(void)
{
	DDRB = 0b00010000;//Enable speaker for output
	ADCSRA = 0b11100111;//Enable ADC, Start conversion, Free running mode, lowest conversion speed
	ADMUX = 0b01100001;//Ref voltage, Left align result, ADC1 enable
	TCCR2A = 0b01000000;//Normal mode, toggle on compare match
	TCCR2B = 0b00000010;//1/8 prescale
	TIMSK2 = 0b00000010;//enable interrupts on A
	sei();
}

ISR(TIMER2_COMPA_vect)//Interrupt for timer compare match A
{
	if (ADCH > 199)
			VR1 = 200;//10kHz maximum
		else
			VR1 = ADCH;
	OCR2A += 50 + VR1;//2kHz minimum + Variable resistor value
}