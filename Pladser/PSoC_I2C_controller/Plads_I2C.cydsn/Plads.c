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

void initPlads()
{
    I2C_Start(); //Start I2C communication
}

uint8_t startPlads(uint8_t Address)
{
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
    return status;
}

uint8_t stopPlads(uint8_t Address)
{
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
    return status;
}

float timeConvert(uint8_t MSB, uint8_t LSB)
{
    uint16_t timerVal16bit = (uint16_t)LSB;
    timerVal16bit += (uint16_t)(MSB << 8);
    return ((float)timerVal16bit)/100;
}

uint8_t getPladsData(uint8_t Address, uint8_t* timerValMSB, uint8_t* timerValLSB, float* timerVal, bool* playerDone, uint8_t* sendToPlayer)
{
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
        
        //convert rxData[0] and rxData[1] to timerVal
        *timerVal = timeConvert(rxData[0], rxData[1]);

        //Mask out playerDone bit
        *playerDone = ((rxData[2] & 0b10000000) >> 7);
        
        //Mask out sendToPlayer value
        *sendToPlayer = (rxData[2] & 0b00001111);
    }
    else //Error in communication
    {
        I2C_MasterSendStop(); //Stop receiving
    }
    
    return status;
}

/* [] END OF FILE */
