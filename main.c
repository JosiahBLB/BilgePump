/*
 * TWI.c
 *
 * Created: 25/05/2022 6:11:23 pm
 * Author : qhs7289
 */ 

#include <avr/io.h>

#define sendingComplete (TWCR & (1<<TWINT))
#define enableAll (0x3F)
#define fullBrightness (0x3F)
#define red 0b00000000
//Status Register
#define status (TWSR & 0xF8)
#define startOk 0x08
#define repeatStartOk 0x10
#define addressWriteAckRx 0x18
#define dataAckOk 0x28
#define addressReadAckRx 0x40
//RGB LED Registers
#define RGB_LED 0xCC
#define LEDcontrol 0x03
#define blueLED_Brightness 0x02
#define redLED_Brightness 0x01
#define greenLED_Brightness 0x00
//Functions
char twiWriteSequence(char slaveAddress, char registerAddress, char data);
unsigned char twiStart(void);
char ADCreading(void);
char twiWrite(char data);
void twiStop(void);
void setup(void);

int main(void)
{
	setup();//Run once
    while (1) 
    {
		twiWriteSequence(RGB_LED, LEDcontrol, ADCreading());//Turn on the colour chosen by the ADC
    }
}

char ADCreading(void)//LED colour function
{
	char colour;
	if(ADCH < 64)//if less than ADCH is under 64 then turn on red LED
		colour = 0b00001100;
	if(ADCH > 63 && ADCH < 192)//if ADCH is between 63 and 192 LED to green
		colour = 0b00000011;
	if(ADCH > 191)//if ADCH is above 191 then set LED to blue
		colour = 0b00110000;
	return(colour);//return the colour based on conditions
}

char twiWriteSequence(char slaveAddress, char registerAddress, char data)//TWI write sequence
{	
	char error = 0;
	if(twiStart() == !startOk)//Run TWI start sequence and return the status code, if start ok then continue
		error = 1;
	else if(twiWrite(slaveAddress) == !repeatStartOk)//Run TWI write sequence and return the status code, if repeat start ok then continue
		error = 1;
	else if(twiWrite(registerAddress) == !addressWriteAckRx)//Run TWI write sequence and return the status code, if write ack then continue
		error = 1;
	else if(twiWrite(data) == !dataAckOk)//Run TWI write sequence and return the status code, if data ack ok then continue
		error = 1;
	twiStop();//Disable TWI sequence
		return (error);//return error code
}

void setup(void)
{
	TWBR = 0x20;//Set clock to 333kHz
	TWSR = 0;
	ADCSRA = 0b11100111;//Enable adc, Free running mode, start conversion, slowest conversion speed
	ADMUX = 0b01100001;//Set reference voltage, left align result, enable ADC 1
	twiWriteSequence(RGB_LED, redLED_Brightness, fullBrightness);//Set the red LED to full brightness
	twiWriteSequence(RGB_LED, blueLED_Brightness, fullBrightness);//Set the blue LED to full brightness
	twiWriteSequence(RGB_LED, greenLED_Brightness, fullBrightness);//Set the green LED to full brightness
}

char twiWrite(char data)//TWI write function
{
	TWDR = data;//Send Data
	TWCR = (1<<TWINT) | (1<<TWEN);//Clear interrupt, Enable TWI
	while(!sendingComplete);//Wait for transmission to be complete
	return(status);//Return the status code
}

void twiStop(void)
{
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);//Clear interrupt, stop condition, TWI enable
	char x = 0;
	while (x < 50)//Short delay
	{
		asm("NOP");
		x++;
	}
}

unsigned char twiStart(void)
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);//Clear interrupt, start condition, TWI enable
	while(!sendingComplete);//wait for transmission to complete
	return(status);//return the status code
}	