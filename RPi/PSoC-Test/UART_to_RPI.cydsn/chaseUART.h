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

#ifndef CHASE_UART_H
#define CHASE_UART_H

#include "project.h"
    
uint8_t gameRunning;
    
void chaseUARTinit();

void sendData(uint8_t penaltyPlayer, uint8_t timePlayer, uint8_t timeMSB, uint8_t timeLSB);

void handleByteReceived2(uint8_t byteReceived1);

CY_ISR_PROTO(ISR_UART_2_rx_handler);

#endif //CHASE_UART_H

/* [] END OF FILE */
