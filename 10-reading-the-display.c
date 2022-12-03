/*
 * ReadingTheDisplay.c
 *
 * Created: 18/05/2022 6:05:19 pm
 * Author : qhs7289
 */ 
//Libraries
#include <avr/io.h>
#include <labboard.h>
#include <stdio.h>
//I/0
#define ceOn (PORTB |= (1<<0))
#define ceOff (PORTB &= ~(1<<0))
#define spiFinished (SPSR & (1<<7))
//Functions
void setup(void);
void writeRTC(char address, char data);
unsigned char readRTC(char address);
unsigned char BCDToDecimal(char bcdByte);
unsigned char DecimaltoBCD(char bcdByte);
void writeLCD(char hours, char minutes, char seconds);
//Variables
char hours_BCD, minutes_BCD, seconds_BCD;
char hours_d, minutes_d, seconds_d;
char currentTime[20];

int main(void)//Main function
{
	setup();//Run setupt once
    while (1) 
    {
	hours_BCD = readRTC(0x02);//read hours from the timer clock
	minutes_BCD = readRTC(0x01);//read minutes from the timer clock
	seconds_BCD = readRTC(0x00);//read seconds from the timer clock
	
	hours_d = BCDToDecimal(hours_BCD);//Convert the hours from BCD to decimal format
	minutes_d = BCDToDecimal(minutes_BCD);//Convert the minutes from BCD to decimal format
	seconds_d = BCDToDecimal(seconds_BCD);//Convert the seconds from BCD to decimal format
	
	writeLCD(hours_d, minutes_d, seconds_d);//Write the current time to the LCD
    }
}

void setup(void)
{
	DDRB = 0b00000111;//Enable Port 2:0 for output
	SPSR = 0b00000000;
	SPCR = 0b01010111;//Enable, master, clock edge, clock rate
	writeRTC(0x8F, 0x00);//disable write protection
	writeRTC(0x82, 0x01);//Set hours
	writeRTC(0x81, 0x59);//Set minutes
	writeRTC(0x80, 0x50);//Set seconds
	SLCDInit();//Initialize the LCD
	SLCDDisplayOn();//Turn the LCD on
	SLCDClearScreen();//Clear the LCD screen
}

void writeRTC(char address, char data)
{
	ceOn;//Enable the chip
	SPDR = address;//Send the address you want to write to
	while(!spiFinished);//wait for all bits to be sent
	SPDR = data;//Send the data that you want to write to that address
	while(!spiFinished);//wait for all bits to be sent
	ceOff;//Turn chip off
}

unsigned char readRTC(char address)
{
	char data;//To allow the return function to work properly
	ceOn;//Enable the SPI chip
	SPDR = address;//Write the address that you want to read
	while(!spiFinished);//Wait for the address to be sent
	SPDR = 0x00;//Send a rubbish byte to allow clock pulses to retrieve wanted data from slave
	while(!spiFinished);//wait for data to be recieved
	data = SPDR;//Save the data
	ceOff;//Turn chip off
	return (data);//Return the data value
}

void writeLCD(char hours, char minutes, char seconds)
{	
	SLCDSetCursorPosition(0,0);//Set the cursor to the home position
	sprintf(currentTime, "%s%d:%s%d:%s%d", hours<10?"0":"", hours, minutes<10?"0":"", minutes, seconds<10?"0":"", seconds);//Save a string with the time variables
	SLCDWriteString(currentTime);//Output the time to the screen
}

unsigned char BCDToDecimal(char bcdByte)
{
	return ((((bcdByte & 0xF0)>>4)*10)+(bcdByte & 0x0F));//Coverts BCD to Decimal
}
/*Left side gets right shifted by 4 and then multiplied by 10 to give us a 10's decimal number.
The right side which represents the ones is now added with the converted left side. This gives us our final decimal number.
E.g
BCD: 59 = 0b01011001
0b01010000 != 5 -> right shift by 4 -> 0b00000101 = 5 -> 5*10=50
0b00001001 = 9  -> 50+9 = 59 in decimal.*/

unsigned char DecimaltoBCD(char decimalByte)
{
	return (((decimalByte/10)<<4)|(decimalByte % 10));//Converts decimal to BCD
}
/*For the left hex number divide by 10 to get rid of the ones (char data type, not a float) and left shift that by 4. (0xN0)
Modulo 10 gives the remainder when divided by ten which will just give back the ones digit. (0x0N)
Then add the two results to get (0xNN)*/
