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
	TWCR |= (1 << TWEA | 1 << TWEN | 1 << TWIE); //Enable acknowledgement, enable Two wire interface and enable interrupt
	
	//Initialize attributes
	txCount_ = 0;
	PSoCstatus_ = 0;
	TXbyte3_ = 0;
}

void PSoC::initiateAddress()
{
	TWAR = ((PINC & 0b00000111) << 1); //Initiate address register with values from PC0, 1 and 2. The address is in bit 7..1 therefore it needs to be bit-shifted 1 time
}

void PSoC::PSoCHandler(uint8_t& timervalMSBRef, uint8_t& timervalLSBRef, bool& playerDoneRef, uint8_t& sendToPlayerRef, uint8_t& rxDataRef, bool& newDataReadyRef)
{
	//Initialize third byte to transmit
	if (playerDoneRef == 1)
	{
		TXbyte3_ = (sendToPlayerRef | (0b10000000));
	}
	else
	{
		TXbyte3_ = sendToPlayerRef;
	}
	
	//Mask the TWI status from the TWI status register
	PSoCstatus_ = (TWSR & TWSR_MASK);
	
	switch (PSoCstatus_)
	{
		case OWN_SLA_AND_R_RECEIVED: //Transmit first byte of data
		{
			TWDR = timervalMSBRef;
			TWCR |= (1 << TWEA); //Enable acknowledge. Transmit first data byte
			txCount_++; // Increment number of bytes transmitted
			break;
		}
		
		case TWDR_TRANSMITTED_ACK_RECEIVED: //Transmit second byte of data
		{
			if(txCount_ == 1)
			{
				TWDR = timervalLSBRef;
				TWCR |= (1 << TWEA); //Enable acknowledge. Transmit second data byte
				txCount_++; // Increment number of bytes transmitted
				break;
			}
			else if(txCount_ == 2)
			{
				TWDR = TXbyte3_;
				TWCR &= ~(1 << TWEA); //Disable acknowledge. Transmit last data byte
				txCount_ = 0; //Reset TXcount after last byte has been sent
				break;
			}
		}
		
		case TWDR_TRANSMITTED_NACK_RECEIVED: //Both bytes transmitted, enable acknowledge 
		{
			TWCR |= (1 << TWEA); //Enable acknowledge. Go back to listening
			break;
		}
		
		case OWN_SLA_AND_W_RECEIVED: //Disable acknowledge to indicate that we only wish to receive 1 byte of data
		{
			TWCR &= ~(1 << TWEA); //Disable acknowledge. Receive last byte
			break;
		}
		
		case TWDR_RECEIVED_NACK_RETURNED: //1 byte received, enable acknowledge
		{
			rxDataRef = TWDR;
			newDataReadyRef = true;
			TWCR |= (1 << TWEA); //Enable acknowledge. Go back to listening
			break;
		}
		
	}
	
	//Reset interrupt flag
	TWCR |= (1 << TWINT);
	
	return;
}
