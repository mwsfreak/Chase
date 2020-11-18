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
#include "project.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"


int main(void)
{
    /* Prepare interrupts */
    CyGlobalIntEnable;                          //Enable global interrupts
    
    /* Prepare components */
    UART_Start();
    uint8_t myArray[] = {35,1,44};  // 35 = 0b0010 0011 = penalty -> player 2, avgTime -> player 3 
    UART_PutString("We are ready...\nStart RPI..!\n");
    
    for(;;)
    {
        if (switch_pin_Read() == 0) 
        {
            led_pin_Write(~led_pin_Read());
            UART_PutChar(myArray[0]);
            UART_PutChar(myArray[1]);
            UART_PutChar(myArray[2]);
            CyDelay(1000);
            UART_ClearRxBuffer();
            UART_ClearTxBuffer();
        }
    }
}

/* [] END OF FILE */