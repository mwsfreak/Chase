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
    choose, choose1 = 0;
    speed = 1;
    steps = 0;
    armtomove = 0;
    arm1 = 1;
    arm2 = 5;
    flyttil = 0;
    move = 10;
    stack = 0;
    strafpoint1, strafpoint2, strafpoint3, strafpoint4, strafpoint5, strafpoint6, strafpoint7, strafpoint8 = 0;
    UART_1_PutString("Stepper motor application started\r\n");
    UART_1_PutString("w: Decrease speed\r\n");
    UART_1_PutString("q: Increase speed\r\n");
    UART_1_PutString("e: Choose arm 1 \r\n");
    UART_1_PutString("r: Choose arm 2\r\n");
    UART_1_PutString("1-8: vaelg plads\r\n");
    UART_1_PutString("p: Print strafpoints\r\n");
    
   for(;;)
    {
       rykArm(choose);
    }
}

CY_ISR(Count_Handler)
{ 
    
    if(steps >= 1) 
    {
        if(steps == (75*move)+1) // 75 Svare til en plads, denne ganes med hvor mange pladser den skal flytte plus 1.
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