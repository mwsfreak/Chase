
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
#include <stdint.h>
#include <stdbool.h>

void initPlads();
uint8_t startPlads(uint8_t Address);
uint8_t stopPlads(uint8_t Address);
float timeConvert(uint8_t MSB, uint8_t LSB);
uint8_t getPladsData(uint8_t Address, uint8_t* timerValMSBfloat, uint8_t* timerValLSB, float* timerVal, bool* playerDone, uint8_t* sendToPlayer);


   //Create variables to receive
volatile    float timerVal1;
volatile    uint8_t timerValMSB1;
volatile    uint8_t timerValLSB1;
volatile    uint8_t sendToPlayer1;
volatile    bool playerDone1;
    
volatile    float timerVal2;
volatile    uint8_t timerValMSB2;
volatile    uint8_t timerValLSB2;
volatile    uint8_t sendToPlayer2;
volatile    bool playerDone2;

/* [] END OF FILE */
