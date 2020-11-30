/* 
* PSoC.cpp
*
* Created: 30-10-2020 11:37:40
* Author: HansK
*/
#define OWN_SLA_AND_R_RECEIVED 0xA8
#define OWN_SLA_AND_W_RECEIVED 0x60
#define TWDR_TRANSMITTED_ACK_RECEIVED 0xB8
#define TWDR_TRANSMITTED_NACK_RECEIVED 0xC0
#define TWDR_RECEIVED_NACK_RETURNED 0x88
#define TWSR_MASK 0b11111000

#include "PSoC.h"
#include <avr/io.h>

// default constructor
PSoC::PSoC()
{
	initPSoC();
	initiateAddress();
} //PSoC

void PSoC::initPSoC()
{
	DDRC &= ~(1 << PC0 | 1 << PC1 | 1 << PC2); //Set address pins to input
	
	TWCR |= (1 << TWEA | 1 << TWEN | 1 << TWIE); //Enable acknowledgement, enable Two wire interface and enable interrupt. 
}

void PSoC::initiateAddress()
{
	/*
	The address from the pins is incremented by 1 to ensure that the address corresponds to the address in the PSoC.
	PSoC addresses: 1 - 8
	ATmega8 pins: 0 - 7
	*/
	TWAR = (((PINC & 0b00000111) + 1) << 1); //Initiate address register with values from PC0, 1 and 2. The address is in bit 7..1 therefore it needs to be bit-shifted 1 time
}
