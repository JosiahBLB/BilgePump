/*
 * AccurateColourControl.c
 *
 * Created: 25/05/2022 8:25:54 pm
 * Author : qhs7289
 */ 

//Libraries
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <labboard.h>
#include <stdio.h>
//Inputs
#define redButton (PINA & (1<<0))
#define greenButton (PINA & (1<<1))
#define blueButton (PINA & (1<<2))
//Status Register status's & TWI
#define sendingComplete (TWCR & (1<<TWINT))
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
#define enableAll (0x3F)
#define fullBrightness (0x3F)
//Variables
char red, green, blue, brightness;
//functions
char twiWriteSequence(char slaveAddress, char registerAddress, char data);
void writeLCD(void);
unsigned char twiStart(void);
char twiWrite(char data);
void twiStop(void);
void setup(void);

int main(void)
{
	setup();//Run setup once
    while (1) 
    {
		PORTE = 0x01;//Set PINA to buttons
		_delay_ms(50);//Small delay to allow the hardware to update
		if(!redButton)//if the red button is pressed
		{
			_delay_ms(100);//debounce
			while(!redButton)
			{
				_delay_ms(100);//debounce
				red = brightness;//set the red to the current brightness value
			}
		}
		if(!greenButton)//if the green button is pressed
		{
			_delay_ms(100);//debounce
			while(!greenButton)
			{
				_delay_ms(100);//debounce
				green = brightness;//set the green to the current brightness value
			}
		}
		if(!blueButton)//if the blue button is pressed
		{
			_delay_ms(100);//debounce
			while(!blueButton)
			{
				_delay_ms(100);//debounce
				blue = brightness;//set the blue to the current brightness value
			}
		}

		PORTE = 0x00;//Enable Switches
		_delay_ms(50);//Give hardware time to update
		brightness = (PINA & 0x3F);//Set the brightness variable to the current switch input
		writeLCD();//Write the RGB values to the screen
		twiWriteSequence(RGB_LED, redLED_Brightness, red);//Send values to TWI writing sequence
		twiWriteSequence(RGB_LED, greenLED_Brightness, green);//Send values to TWI writing sequence
		twiWriteSequence(RGB_LED, blueLED_Brightness, blue);//Send values to TWI writing sequence
		
    }
}

void setup(void)
{
	DDRE = 0x03;//Enable multiplexer
	TWBR = 0x20;//Set clock to 333kHz
	twiWriteSequence(RGB_LED, LEDcontrol, enableAll);//Turn on all LED's
	SLCDInit();//Initialize the LCD
	SLCDDisplayOn();//Turn the LCD on
	SLCDClearScreen();//Clear the LCD screen
}

char twiWriteSequence(char slaveAddress, char registerAddress, char data)//Main TWI sequence
{
	char error = 0;
	if(twiStart() == !startOk)//Check if start condition has been received, if it has... continue
	error = 1;
	else if(twiWrite(slaveAddress) == !repeatStartOk)//Repeated start recieved? if so, continue
	error = 1;
	else if(twiWrite(registerAddress) == !addressWriteAckRx)//Write acknowledge recieved? if so, continue
	error = 1;
	else if(twiWrite(data) == !dataAckOk)//Data acknowledge recieved? if so, continue
	error = 1;
	twiStop();//Disable TWI function
	return (error);//return error value
}

char twiWrite(char data)//Function for writing data to the TWI
{

	TWDR = data;//Send Data byte
	TWCR = (1<<TWINT) | (1<<TWEN);//Clear interrupt flag
	while(!sendingComplete);//wait for transmission to be complete
	return(status);//Return the status
}

void twiStop(void)
{
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);//Clear interrupt flag, start bit and enable TWI
	char x = 0;//reset counter
	while (x < 50)//Small delay
	{
		asm("NOP");
		x++;
	}
}

unsigned char twiStart(void)//For starting the TWI
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);//Clear interrupt flag, start bit and enable TWI
	while(!sendingComplete);//Wait for the transmission to be complete
	return(status);//Return the status to the function that called it
}

void writeLCD(void)//Function for the LCD
{
	char printRed[20], printGreen[20], printBlue[20];//Variables for the sprintf function
	SLCDSetCursorPosition(0,0);//Set the cursor to the home position
	sprintf(printRed,"Red: %d", red);//Save a string to the printRed variable
	SLCDWriteString(printRed);//Output the red value to the screen
	
	SLCDSetCursorPosition(1,0);//Set the cursor to the home position
	sprintf(printGreen,"Green: %d", green);//Save a string to the printGreen variable
	SLCDWriteString(printGreen);//Output the green value to the screen
		
	SLCDSetCursorPosition(2,0);//Set the cursor to the home position
	sprintf(printBlue,"Blue: %d", blue);//Save a string to the printBlue variable
	SLCDWriteString(printBlue);//Output the blue value to the screen
}