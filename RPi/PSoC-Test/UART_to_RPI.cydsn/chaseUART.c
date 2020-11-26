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

#include "chaseUART.h"

void chaseUARTinit() {
    UART_2_Start();
    isr_uart_2_rx_StartEx(ISR_UART_2_rx_handler);
}

void sendData(uint8_t penaltyPlayer, uint8_t timePlayer, uint16_t time) {
    uint8_t byteA = (penaltyPlayer << 4) + timePlayer; 
    uint8_t byteB = (time >> 8);
    uint8_t byteC = (time & 0x00FF);
    
    UART_2_PutChar(byteA);
    UART_2_PutChar(byteB);
    UART_2_PutChar(byteC);
}

void handleByteReceived2(uint8_t byteReceived) {
    switch(byteReceived)
    {
        case 0x01:
            //Start game
            gameRunning = 1;
            //led_pin_Write(1);
            break;
        
        case 0x02:
            //Stop game
            gameRunning = 0;
            //led_pin_Write(0);
            break;
        
        default:
            break;
    }
    
}

CY_ISR(ISR_UART_2_rx_handler)
{
    uint8_t bytesToRead = UART_2_GetRxBufferSize();
    while (bytesToRead > 0)
    {
        uint8_t byteReceived = UART_2_ReadRxData();
        //UART_1_WriteTxData(byteReceived); // echo back
        
        handleByteReceived2(byteReceived);
        
        bytesToRead--;
    }
}

/* [] END OF FILE */
