/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "Plads.h"
#include "project.h"
#include <stdio.h>
#include <stdint.h>

#define I2C_DELAY 50

void initPlads()
{
I2C_Start(); //Start I2C communication

   //Create variables to receive
timerValMSB1;
timerValLSB1;
sendToPlayer1 = 0;
playerDone1 = false;

timerValMSB2;
timerValLSB2;
sendToPlayer2 = 0;
playerDone2 = false;
}

uint8_t startPlads(uint8_t Address)
{
    I2C_MasterClearStatus();
    
    uint8_t status = I2C_MasterSendStart(Address, 0); //Send start command with write
    if (status == I2C_MSTR_NO_ERROR) //No error
    {
        I2C_MasterWriteByte(0x01); //Sending 0b00000001
        I2C_MasterSendStop(); //Stop sending
    }
    else //Error in communication
    {
        I2C_MasterSendStop(); //Stop sending
    }      
    
    CyDelay(I2C_DELAY);
    
    return status;
}

uint8_t stopPlads(uint8_t Address)
{
    I2C_MasterClearStatus();
    
    uint8_t status = I2C_MasterSendStart(Address, 0); //Send start command with write
    if (status == I2C_MSTR_NO_ERROR) //No error
    {
        I2C_MasterWriteByte(0x00); //Sending 0b00000000
        I2C_MasterSendStop(); //Stop sending
    }
    else //Error in communication
    {
        I2C_MasterSendStop(); //Stop sending
    } 
    
    CyDelay(I2C_DELAY);
    
    return status;
}

uint8_t getPladsData(uint8_t Address, uint8_t* timerValMSB, uint8_t* timerValLSB, bool* playerDone, uint8_t* sendToPlayer)
{
    I2C_MasterClearStatus();
    
    uint8_t rxData[3]; //Create array to contain rxData
    
    //Receive data
    uint8_t status = I2C_MasterSendStart(Address, 1); //Send start command with read
    if (status == I2C_MSTR_NO_ERROR) //No error
    {
        //Receive 3 bytes
        rxData[0] = I2C_MasterReadByte(I2C_ACK_DATA); //Receive byte med ACK
        rxData[1] = I2C_MasterReadByte(I2C_ACK_DATA); //Receive byte med ACK
        rxData[2] = I2C_MasterReadByte(I2C_NAK_DATA); //Receive byte med NACK
        I2C_MasterSendStop(); //Stop receiving
        
        //Return raw values of rxData[0] & rxData[1] to timerVal MSB & LSB
        *timerValMSB = rxData[0];
        *timerValLSB = rxData[1];
        

        //Mask out playerDone bit
        *playerDone = ((rxData[2] & 0b10000000) >> 7);
        
        //Mask out sendToPlayer value
        *sendToPlayer = (rxData[2] & 0b00001111);
    }
    else //Error in communication
    {
        I2C_MasterSendStop(); //Stop receiving
    }
    
    CyDelay(I2C_DELAY);
    
    return status;
}

/* [] END OF FILE */
