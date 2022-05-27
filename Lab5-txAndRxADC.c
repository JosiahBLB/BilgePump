/*
 * TxRxADC_V3.c
 *
 * Created: 30/03/2022 6:44:05 pm
 * Author : qhs7289
 */ 

//Libraries
#define f_cpu 8000000ul //set clock speed to 8MHz
#include <avr/io.h>//AT90 library
#include <labboard.h>//LCD library
#include <stdio.h>//library that I've included for the sprint() function
//Input
#define heaterSwitch (PINA & (1<<0))//Poll if heater switch is on
#define lightSwitch (PINA & (1<<1))//Poll if light switch is on
//Output
#define heaterOn (PORTB |= (1<<7))//Turn on heater
#define heaterOff (PORTB &= ~(1<<7))//Turn heater off
#define lightOn	(PORTB |= (1<<6))//Turn light on
#define lightOff (PORTB &= ~(1<<6))//Turn light off
//PORTC Switch-case
#define displayLDR 0b10000000 //Used to name each switch case
#define displayPot2 0b01000000//See switch case for explanation 
#define displayPot1 0b00100000
#define displayTemp 0b00010000
//ADC
#define channelLDR 0b01100000//change channel to ADC0
#define channelPot2 0b01100001//change channel to ADC1
#define channelPot1 0b01100010//change channel to ADC2
#define channelTemp 0b01100011//change channel to ADC03
#define startConversion (ADCSRA |= (1<<ADSC))//Start the ADC conversion
#define conversionRunning (ADCSRA & (1<<ADSC))//Poll to see if conversion is complete
//UART
#define rxBufferFull (UCSR1A & (1<<RXC1))//Check if the rx buffer is full
#define txBufferEmpty (UCSR1A & (1<<UDRE1))//Check if the tx buffer is empty
//data
char txLDR, rxLDR;//rx and tx variable for each ADC
char txPot2, rxPot2;
char txPot1, rxPot1; 
char txTemp, rxTemp;
//LCD
char line1[20];//variable for each line so that a string-
char line2[20];//can be written to the LCD
char line3[20];
char line4[20];
//Functions
void setup(void);//setup function
char readADC(char channel);//reading ADC function that will have the channel passed to it
void sendReceive(void);//function for tx and rx data transfer
void runLCD(char LDR, char pot2, char pot1, char temp);//LCD function that will have ADC values passed to it

int main(void)//Main function
{
	setup();//Run the setup once
	while(1)//Always repeat
	{
		sendReceive();//Run tx and rx sequence to send and receive ADC values
		runLCD(rxLDR, rxPot2, rxPot1, rxTemp);//Run LCD sequence to display values onto the LCD
		switch (PINA & 0xF0)//Bitwise '&' to go to the case that is true
		{
			case displayLDR: PORTC = rxLDR; break; //this will display LDR value on PORTC when PINA = 10000000
			case displayPot1: PORTC = rxPot1; break;//this will display Pot1 value on PORTC when PINA = 01000000	
			case displayPot2: PORTC = rxPot2; break;//this will display Pot2 value on PORTC when PINA = 00100000
			case displayTemp: PORTC = rxTemp; break;//this will display Temperature value on PORTC when PINA = 00010000
			default: PORTC = 0x00; break;//When none of the case are true, it will default to this case
		}
		
		if (heaterSwitch)//if the heater switch is turned on
		heaterOn;//turn heater on
		else
		heaterOff;//turn heater off
		
		if (lightSwitch)//if the light switch is turned on
		lightOn;//turn light on
		else
		lightOff;//turn light off
		
	}
}

void setup(void)//Setup for I/O, ADC, LCD, and USART
{
	DDRC = 0xFF;//PORTC all outputs
	DDRB = 0b11000000;//PORTB bit 6 and 7 outputs
	ADCSRA = 0b10000111;//enable the ADCSRA and set to slowest conversion speed
	ADMUX = 0b01100000;//Set reference voltage and Left align result
	UCSR1B = 0b00011000;//Enable rx and tx
	UCSR1C = 0b00000110;//Set characters size to 8 bits
	UBRR1L = 8;//set baud rate to 57.6k
	SLCDInit();//Initialize LCD display
	SLCDDisplayOn();//Turn the LCD back-light on
	SLCDClearScreen();//Clear the screen
}

void sendReceive(void)
{
	txLDR = readADC(channelLDR); //read LDR
	while(!txBufferEmpty); //wait for the tx to be empty
	UDR1 = txLDR;	//send the data
	while(!rxBufferFull); //wait for the data to be received
	rxLDR = UDR1; //store the received data
		
	txPot2 = readADC(channelPot2); //read LDR
	while(!txBufferEmpty); //wait for the tx to be ready
	UDR1 = txPot2;	//send the data
	while(!rxBufferFull); //wait for the data to be received
	rxPot2 = UDR1; //store the received data
		
	txPot1 = readADC(channelPot1); //read LDR
	while(!txBufferEmpty); //wait for the tx to be ready
	UDR1 = txPot1;	//send the data
	while(!rxBufferFull); //wait for the data to be received
	rxPot1 = UDR1; //store the received data
		
	txTemp = readADC(channelTemp); //read LDR
	while(!txBufferEmpty); //wait for the tx to be ready
	UDR1 = txTemp;	//send the data
	while(!rxBufferFull); //wait for the data to be received
	rxTemp = UDR1; //store the received data
}

char readADC(char channel)
{
	ADMUX = channel;//Changes the channel to the one that was passed in the function
	startConversion;//starts conversion
	while(conversionRunning);//waits for the conversion
	return(ADCH);//return the adc value back to where the function was called
}

void runLCD(char LDR, char pot2, char pot1, char temp)//run with passed variables
{	
	SLCDSetCursorPosition(0,0);//set the cursor position to the first line
	sprintf(line1,"LDR =%X",LDR);//Save what's in quotation marks with variable in hex and save this as a string to variable line1
	SLCDWriteString(line1);//Write the variable line1
	
	SLCDSetCursorPosition(1,0);//set the cursor position to the second line
	sprintf(line2,"Pot 2 =%X",pot2);//Save what's in quotation marks with variable in hex and save this as a string to variable line2
	SLCDWriteString(line2);//Write the variable line2
	
	SLCDSetCursorPosition(2,0);//set the cursor position to the third line
	sprintf(line3,"Pot 1 =%X",pot1);//Save what's in quotation marks with variable in hex and save this as a string to variable line3
	SLCDWriteString(line3);//Write the variable line3
	
	SLCDSetCursorPosition(3,0);//set the cursor position to the fourth line
	sprintf(line4,"Temp =%X",temp);//Save what's in quotation marks with variable in hex and save this as a string to variable line4
	SLCDWriteString(line4);//Write the variable line4
}
