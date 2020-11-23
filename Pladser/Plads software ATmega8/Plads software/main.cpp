/*
 * Plads software
 *
 * Author : HansK
 */ 
//Define system constants
#define OVERFLOW_FREQ 30.518
#define TIME_RESOLUTION 100
#define BONUS_TIME 500 //In 1/100 seconds
#define BLINK_TOGGLE_OVERFLOWS 10

//I2C constants
#define OWN_SLA_AND_R_RECEIVED 0xA8
#define OWN_SLA_AND_W_RECEIVED 0x60
#define TWDR_TRANSMITTED_ACK_RECEIVED 0xB8
#define TWDR_TRANSMITTED_NACK_RECEIVED 0xC0
#define TWDR_RECEIVED_NACK_RETURNED 0x88
#define TWSR_MASK 0b11111000

//Include libraries
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <avr/interrupt.h>
using namespace std;

//Include classes
#include "PSoC.h"
#include "Timer.h"
#include "Servo.h"
#include "Sensor.h"
#include "interface.h"

//Create global variable to hold the timer value
static double timerValDouble = 0;

//Create global value to hold the address of the game station. This variable is set when the PSoC object is initialized
static int stationAddress = 0;

//Create I2C variables
static uint8_t sendToPlayer = 0; //Player to play next
static uint8_t rxData = 0; //Received I2C data
static uint8_t timerVal_LSB = 0; //Holds the LSB of the timer value to send to the main controller
static uint8_t timerVal_MSB = 0; //Holds the MSB of the timer value to send to the main controller
static bool playerDone = true; //Current player done

//Create enum to hold the status of the program
enum status{stopped, standby, started, awaitingUserHit, userHitConfirmed};
static status gameStatus = stopped;


int main(void)
{	
	
	//Create servo object
	Servo ReleaseServo;
	
	//Create sensor object
	Sensor ballHitSensor;
	
	//Create timer object
	Timer gameTimer;
	
	//Create PSoC object
	PSoC mainController;
	
	//Get station address from TWAR I2C register. This register was initiated in the PSoC constructor
	stationAddress = (TWAR >> 1); //The address is in bit 7..1 therefore it needs to be bit-shifted 1 time
	
	//Create interface object
	interface userInterface;
	
	//Enable global interrupt
	sei();
	
	
	
	//Main loop
    while (1) 
    {
		//Switch on gameStatus 
		switch (gameStatus)
		{
			//The station is stopping. Clear variables, turn on red LED and stop timer.
			case stopped:
			{
				//Clear variables
				sendToPlayer = 0;  
				timerVal_LSB = 0; 
				timerVal_MSB = 0; 
				timerValDouble = 0;
				playerDone = false;
				
				
				//Stop game timer
				gameTimer.stop();		
				
				//Set game status to stopped, and wait for start command
				gameStatus = standby;
				
				break;
			}
			
			//The station is stopped and waiting for a startcommand
			case standby:
			{
				//Turn on red RGB
				userInterface.RGBred();
				break;
			}
			
			//The station is started
			case started:
			{
				//Release ball
				ReleaseServo.releaseBall();
				
				//Start game timer
				gameTimer.start();
				
				//Turn on green blink RGB
				userInterface.RGBgreenBlink();
				
				//Set game status to awaitingUserHit
				gameStatus = awaitingUserHit;
				
				break;
			}
			
			//Ball has been released and station is waiting for the user to hit
			case awaitingUserHit:
			{
				//Check if bonustime is over
				if (timerValDouble > BONUS_TIME)
				{
					userInterface.RGBgreen();
				}
				
				//Check if sensor has been activated
				if (ballHitSensor.getSensorVal() == true)
				{
					//If there is a hit, game status changes to userHitConfirmed
					gameStatus = userHitConfirmed;
					
					//Stop the game timer
					gameTimer.stop();
				}
				
				break;
			}
			
			//The user has hit. The station determines if the user needs to choose a player to send to, og the next player will be choosen automaticly
			case userHitConfirmed:
			{				
				//Check if hit was before or after bonustime
				if (timerValDouble < BONUS_TIME)
				{
					//Create temporary variable for interface status
					static int interfaceStatus;
					
					//Turn on blue RGB
					userInterface.RGBblue();
					
					//Get user input from interface
					interfaceStatus = userInterface.getKnapstatus();
					
					if (interfaceStatus != 0)
					{
						sendToPlayer = interfaceStatus + stationAddress;
					}
				} 
				else
				{
					//Set send to player to the player next to you
					sendToPlayer = stationAddress + 1;
				}
				
				//If the choosen player results in a address > 8, sendToPlayer will have to be substracted with 8
				if (sendToPlayer > 8)
				{
					sendToPlayer -= 8;
				}
				
				//Check if a valid player has been selected
				if (sendToPlayer > 0)
				{
					playerDone = true;
				}
				
				break;
			}
		}
			
    }
	
}




ISR(TWI_vect)
{	
	// Create variable to keep track of the transmit sequence
	static uint8_t txCount = 0;
	
	//Mask the TWI status from the TWI status register and switch-case on this
	switch (TWSR & TWSR_MASK)
	{
		case OWN_SLA_AND_R_RECEIVED: //Transmit first byte of data
		{
			TWDR = (uint8_t)(((uint16_t)round(timerValDouble)) >> 8); //Transfer MSB of timervalue to data register
			TWCR |= (1 << TWEA); //Enable acknowledge. Transmit first data byte
			txCount++; // Increment number of bytes transmitted
			break;
		}
		
		case TWDR_TRANSMITTED_ACK_RECEIVED: //Transmit second byte of data
		{
			if(txCount == 1)
			{
				TWDR = (uint8_t)((uint16_t)round(timerValDouble)); //Transfer MSB of timervalue to data register
				TWCR |= (1 << TWEA); //Enable acknowledge. Transmit second data byte
				txCount++; // Increment number of bytes transmitted
				break;
			}
			else if(txCount >= 2)
			{
				//Initialize third byte to transmit
				if (playerDone == 1)
				{ TWDR = (sendToPlayer | (0b10000000)); }
				else
				{ TWDR = sendToPlayer; }
					
				TWCR &= ~(1 << TWEA); //Disable acknowledge. Transmit last data byte
				txCount = 0; //Reset TXcount after last byte has been sent
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
			//Get rxData from data register
			rxData = TWDR;
			
			//Change game status according to rxData
			if (rxData == 1)
			{
				gameStatus = started;
			}
			else if(rxData == 0)
			{
				gameStatus = stopped;
			}
			
			TWCR |= (1 << TWEA); //Enable acknowledge. Go back to listening
			break;
		}
		
		default:
		{
			//If there is an error, this command returns the I2C hardware to a known state and releases the SDA and SCL lines
			TWCR = (1 << TWEA | 1 << TWSTO | 1 << TWEN);
			//Turn all LEDs off
			PORTB &= ~(1 << PB2 | 1 << PB3 | 1 << PB4);
			PORTB |= (1 << PB3);
			break;
		}
		
	}
	
	//Reset interrupt flag
	TWCR |= (1 << TWINT);
}




ISR(TIMER2_OVF_vect)
{
	//Increment timer value
	timerValDouble += (TIME_RESOLUTION/OVERFLOW_FREQ);
}




ISR(TIMER0_OVF_vect)
{
	//Create static int to hold the number of overflows
	static int numberOfOverflows = 0;
	
	//To reduce the blink frequency we only toggle everytime numberOfOverflows exceeds BLINK_TOGGLE_OVERFLOWS
	numberOfOverflows++;
	
	if (numberOfOverflows >= BLINK_TOGGLE_OVERFLOWS)
	{
		//Toggle green LED
		if (PINB & (1 << PB4))
		{
			PORTB &= ~(1 << PB4);
		} 
		else
		{
			PORTB |= (1 << PB4);
		}
		
		//Reset numberOfOverflows
		numberOfOverflows = 0;	
	}
}
