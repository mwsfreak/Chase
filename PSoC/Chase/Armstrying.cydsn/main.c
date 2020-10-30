#include "Armstyring.h"

CY_ISR_PROTO(ISR_UART_rx_handler);
CY_ISR_PROTO(Count_Handler);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    isr_1_StartEx(Count_Handler);
    UART_1_Start();
    PWM_1_Start();
  
    UART_1_PutString("Stepper motor application started\r\n");
    UART_1_PutString("s: Direction\r\n");
    UART_1_PutString("w: Decrease speed\r\n");
    UART_1_PutString("q: Increase speed\r\n");
    UART_1_PutString("0: Stop\r\n");
    UART_1_PutString("i: move\r\n");

    speed = 0;
    steps = 0;
   for(;;)
    {}
}

CY_ISR(Count_Handler)
{ 
    if(steps >= 1) 
    {
        if(steps == 25)
        {
            off();
            steps = 0;   
        }  
        else
        {
        steps=steps+1;
        }
    }
    PWM_1_ReadStatusRegister(); 
}

CY_ISR(ISR_UART_rx_handler)
{
    uint8_t bytesToRead = UART_1_GetRxBufferSize();
    while (bytesToRead > 0)
    {
        uint8_t byteReceived = UART_1_ReadRxData();
        UART_1_WriteTxData(byteReceived); // echo back
        
        handleByteReceived(byteReceived);
        bytesToRead--;
    }
}