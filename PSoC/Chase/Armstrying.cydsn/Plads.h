
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

/* [] END OF FILE */
