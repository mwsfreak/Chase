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

void sendData(uint8_t penaltyPlayer, uint8_t timePlayer, uint16_t time);

void handleByteReceived(uint8_t byteReceived);

CY_ISR_PROTO(ISR_UART_1_rx_handler);

#endif //CHASE_UART_H

/* [] END OF FILE */
