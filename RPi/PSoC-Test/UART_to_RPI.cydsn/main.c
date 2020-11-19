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
#include "chaseUART.h"

//uint8_t gameRunning = 0;

int main(void)
{
    /* Prepare interrupts */
    CyGlobalIntEnable;                          //Enable global interrupts
    
    /* Prepare components */
    chaseUARTinit();
        
    for(;;)
    {
        if (switch_pin_Read() == 0) 
        {
            sendData(2, 3, 300); // Penalty --> Player 2, time --> player 3, time = 300 seconds/100
            CyDelay(50);
            //UART_1_ClearRxBuffer();
            //UART_1_ClearTxBuffer();
        }
    }
}

/* [] END OF FILE */