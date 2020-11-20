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

//Create global interrupt flag for I2C
static bool TWIinterrupt = false;

//Create global variable to hold the timer value
static double timerValDouble = 0;

//Create global value to hold the address of the game station. This variable is set when the PSoC object is initialized
static int stationAddress = 0;

//Create enum to hold the status of the program
enum status{stopped, started, awaitingUserHit, userHitConfirmed};
static status gameStatus = stopped;

int main(void)
{
	//Create I2C variables
	uint8_t sendToPlayer = 0; //Player to play next
	uint8_t rxData; //Received I2C data
	uint8_t timerVal_LSB = 0; //Holds the LSB of the timer value to send to the main controller
	uint8_t timerVal_MSB = 0; //Holds the MSB of the timer value to send to the main controller
	bool playerDone = true; //Current player done
	bool newDataReady = false; //New data has been received by I2C and needs to be evaluated
	
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
		//Check if new I2C data is ready to be evaluated
		if(newDataReady == true)
		{
			if (rxData == 1)
			{
				gameStatus = started;
			}
			else if(rxData == 0)
			{
				gameStatus = stopped;
			}
			newDataReady = false;
		}
		
		//Check if I2C HW needs to be handled
		if (TWIinterrupt == true)
		{		
			//Convert double into two uint8_t variables
			gameTimer.convertDoubleToUint8_t(timerValDouble, timerVal_MSB, timerVal_LSB);
			
			//Call handler function to control the I2C interface
			mainController.PSoCHandler(timerVal_MSB, timerVal_LSB, playerDone, sendToPlayer, rxData, newDataReady);
			TWIinterrupt = false;
		}
		
		//Switch on gameStatus 
		switch (gameStatus)
		{
			//The station is stopped
			case stopped:
			{
				//Clear variables
				sendToPlayer = 0;  
				timerVal_LSB = 0; 
				timerVal_MSB = 0; 
				timerValDouble = 0;
				playerDone = false;
				
				//Turn on red RGB
				userInterface.RGBred();
				
				//Stop game timer
				gameTimer.stop();		
				
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
	TWIinterrupt = true;
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
