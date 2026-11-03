/*****************************************************************************
 *	    File: Lab08.c
 *  Lab Name: RS232 Communication Server
 *    Author: Dr. Greg Nordstrom
 *   Created: 12/30/17
 *  Mod Date: 3/28/19, 1/17/21
 *  Modified: Tanner Ozanich
 *   ModDate: 10/20/26
 * 
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This program implements a communication server. Normally the server
 * simply echoes what was sent. However, there are two cases where
 * different behavior occurs:
 *    1. Pressing the ESC causes a soft reset (redisplays the title text)
 *    2. Typing the letters 'end' contiguously displays 'Bye' and halts
 *
 *****************************************************************************/

// define CPU speed (7.3728 MHz for the ATmega128A)
#define F_CPU   7372800UL

#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

#include <avr/io.h>
#include <util/delay.h>
#include "UARTLibrary.h"  // Double quotes for local library.

#define BAUDRATE 9600 // standard values from 1200 to 115200
#define DATABITS    8 // 5-8
#define STOPBITS    1 // 1,2
#define PARITY      0 // 0=none, 1=odd, 2=even

void printStr(unsigned char *cp);

int main(void)
{
	uart_init(BAUDRATE, DATABITS, STOPBITS, PARITY);
	
	DDRC |= (1 << 3);
	PORTC |= (1 << 3);
	
	
	unsigned char greeting[86] = 
	"\r\nRS232 Communications Server\r\n"
	"Status: Active\r\n"
	"Tanner Ozanich\r\n"
	"EECE3624\r\n"
	"Fall 2026\r\n";
	
	printStr(greeting);
	
	unsigned char lastChar = 0;
	unsigned char goodbye[10] = "\r\n\r\nbye!\r\n";
	
	while(1){
		unsigned char data = uart_rx();
		
		PORTC &= ~(1 << 3);
		_delay_ms(75);
		PORTC |= (1 << 3);
		
		if (data == 0x1B){
			printStr(greeting);
			lastChar = 0;
		}else if (data == CR){
			uart_tx(LF);
			lastChar = 0;
		}else if (data == 'e'){
			lastChar ='e';
		}else if ((lastChar == 'e') && (data == 'n')){
			lastChar = 'n';
		}else if ((lastChar =='n') && (data == 'd')){
			uart_tx('d');
			printStr(goodbye);
			while(1){
			}
		}else{
			lastChar = 0;
		}
		
		uart_tx(data);
	}
	
	return 0;
}

void printStr(unsigned char *cp){
	while(*cp != '\0'){
		uart_tx(*cp);
		cp++;
	}
}
