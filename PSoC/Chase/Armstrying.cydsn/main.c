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
    
     
    speed = 1;
    steps = 0;
    arm1 = 1;
    arm2 = 5;
    flyttil = 0;
    move1 = 10;
    move2 = 10;
    char arr1[20] ;
    char arr2[20] ;
    UART_1_PutString("Stepper motor application started\r\n");
    UART_1_PutString("w: Decrease speed\r\n");
    UART_1_PutString("q: Increase speed\r\n");
    UART_1_PutString("0: Stop\r\n");
    UART_1_PutString("i: move\r\n");
    UART_1_PutString("1-8: vaelg plads\r\n");
    
   for(;;)
    {
        
        if (flyttil != 0 && flyttil != arm1)
        {
            PWM_1_Start(); 
            move1 = checkNumbersofSteps(arm1, arm2, flyttil);
            
            if (move1 != 10)
            {
            arm1 = arm1+move1;
             
            
            if (arm1 > 8)
            {
                arm1 = arm1-8;
            }
            else if (arm1 < 0)
            {
                arm1 = arm1+8;
            }
            
            if (move1 > 0)
            {
                direction(1);
            }
            else if (move1 < 0) 
            {
                direction(0);
                move1 = move1*-1;
            }
            
            flyttil = 0;
            steps = 1;
            sprintf(arr1,"Arm1's nye plads: %d\r\n", arm1);
            UART_1_PutString(arr1);
            sprintf(arr2,"Flytter %d pladser\r\n", move1);
            UART_1_PutString(arr2);
            }
        }
    }
}

CY_ISR(Count_Handler)
{ 
    
    if(steps >= 1) 
    {
        if(steps == (25*move1)+1) // 75 Svare til en plads, denne ganes med hvor mange pladser den skal flytte plus 1.
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