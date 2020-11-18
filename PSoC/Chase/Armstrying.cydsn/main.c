#include "Armstyring.h"

CY_ISR_PROTO(ISR_UART_rx_handler);
CY_ISR_PROTO(Count_Handler1);
CY_ISR_PROTO(Count_Handler2);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    
    isr_1_StartEx(Count_Handler1);
    isr_2_StartEx(Count_Handler2);
    UART_1_Start();
    
    //speed = 1;
    steps1, steps2 = 0;
    arm1 = 1;
    arm2 = 5;
    flyttil = 0;
    move = 10;
    stack = 0;
    choose = 0;
    strafpoint1, strafpoint2, strafpoint3, strafpoint4, strafpoint5, strafpoint6, strafpoint7, strafpoint8 = 0;
   
    UART_1_PutString("Stepper motor application started\r\n");
    UART_1_PutString("e: Choose arm 1 \r\n");
    UART_1_PutString("r: Choose arm 2\r\n");
    UART_1_PutString("1-8: vaelg plads\r\n");
    UART_1_PutString("p: Print strafpoints\r\n");
    
   for(;;)
    {
     //spørg på adresse arm1
        //rykArm(0);
        rykArm(choose);
        
    //spørg på adresse arm2
        //rykArm(1);
    }
}

CY_ISR(Count_Handler1)
{ 
    if(steps1 >= 1) 
    {
        if(steps1 == (75*move)+1) // 75 Svare til en plads, denne ganes med hvor mange pladser den skal flytte plus 1.
        {
            off();
            steps1 = 0;   
        }  
        else
        {
            UART_1_PutString("fejl3");
            steps1=steps1+1;
        }   
    }

    PWM_1_ReadStatusRegister(); 
}

CY_ISR(Count_Handler2)
{ 
    if(steps2 >= 1) 
    {
        if(steps2 == (75*move)+1) // 75 Svare til en plads, denne ganes med hvor mange pladser den skal flytte plus 1.
        {
            off();
            steps2 = 0;   
        }  
        else
        { 
            UART_1_PutString("fejl4");
            steps2=steps2+1;
        }   
    }
    PWM_2_ReadStatusRegister(); 
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