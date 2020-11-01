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
    armtomove = 0;
    arm1 = 1;
    arm2 = 5;
    flyttil = 0;
    move = 10;
    stack = 0;
    strafpoint1, strafpoint2, strafpoint3, strafpoint4, strafpoint5, strafpoint6, strafpoint7, strafpoint8 = 0;
    char arr1[20] ;
    char arr2[20] ;
    UART_1_PutString("Stepper motor application started\r\n");
    UART_1_PutString("w: Decrease speed\r\n");
    UART_1_PutString("q: Increase speed\r\n");
    UART_1_PutString("e: Choose arm 1 \r\n");
    UART_1_PutString("r: Choose arm 2\r\n");
    UART_1_PutString("1-8: vaelg plads\r\n");
    UART_1_PutString("p: Print strafpoints\r\n");
    
   for(;;)
    {
        
        if(Control_Reg_2_Read() == 0)
        {
            if (flyttil != 0 && flyttil != arm1) //flyt til kan være over 8?
            {
                move = checkNumbersofSteps(arm1, arm2, flyttil);
                stack = checkStack(move,arm1,arm2);
                
                if (move != 10 && move != 0 && stack == 0)
                {
                    arm1 = arm1+move;
                    PWM_1_Start(); 
                    
                    if (arm1 > 8)
                    {
                        arm1 = arm1-8;
                    }
                    else if (arm1 <= 0)
                    {
                        arm1 = arm1+8;
                    }
                    
                    if (move > 0)
                    {
                        direction(1);
                    }
                    else if (move < 0) 
                    {
                        direction(0);
                        move = move*-1;
                    }
            
                    flyttil = 0;
                    steps = 1;
                    sprintf(arr1,"Arm1's nye plads: %d\r\n", arm1);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Flytter %d pladser\r\n", move);
                    UART_1_PutString(arr2);
                }
                
             if (move != 10 && move != 0 && stack == 1)
                {
                    addstrafpoint(arm2);
                    direction(1);
                    
                    Control_Reg_2_Write(1); //nødt til at have 2 seperate PWM signaler?
                    
                    arm2 = arm2+move;
                    PWM_1_Start(); 
                    
                    if (arm2 > 8)
                    {
                        arm2 = arm2-8;
                    }
                    else if (arm2 <= 0)
                    {
                        arm2 = arm2+8;
                    }
                    steps =1;
                    
                    CyDelay(1000);
                    
                   
                    Control_Reg_2_Write(0);
                    
                    arm1 = arm1+move;
                    PWM_1_Start(); 
                    
                    if (arm1 > 8)
                    {
                        arm1 = arm1-8;
                    }
                    else if (arm1 <= 0)
                    {
                        arm1 = arm1+8;
                    }
                    flyttil = 0;
                    steps = 1;   
                    
                    UART_1_PutString("Der er stack");
                    sprintf(arr1,"Arm1's nye plads: %d\r\n", arm1);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Arm2's nye plads: %d\r\n", arm2);
                    UART_1_PutString(arr2);
                }
            }
        }
                
        else if (Control_Reg_2_Read() == 1)
        {
            if(flyttil != 0 && flyttil != arm2)
            {       
                move = checkNumbersofSteps(arm2, arm1, flyttil);
                stack = checkStack(move,arm2,arm1);
                
                if (move != 10 && move != 0 && stack == 0)
                {
                    arm2 = arm2+move;
                    PWM_1_Start(); 
                    
                    if (arm2 > 8)
                    {
                        arm2 = arm2-8;
                    }
                    else if (arm2 <= 0)
                    {
                        arm2 = arm2+8;
                    }
                    
                    if (move > 0)
                    {
                        direction(1);
                    }
                    else if (move < 0) 
                    {
                        direction(0);
                        move = move*-1;
                    }
            
                    flyttil = 0;
                    steps = 1;
                    sprintf(arr1,"Arm2's nye plads: %d\r\n", arm2);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Flytter %d pladser\r\n", move);
                    UART_1_PutString(arr2);
                }
               if (move != 10 && move != 0 && stack == 1)
                {
                   addstrafpoint(arm1);
                   direction(1);
                    
                   Control_Reg_2_Write(0);
                    
                    arm1 = arm1+move;
                    PWM_1_Start(); 
                    
                    if (arm1 > 8)
                    {
                        arm1 = arm1-8;
                    }
                    else if (arm1 <= 0)
                    {
                        arm1 = arm1+8;
                    }
                   
                    steps = 1; 
                    
                    CyDelay(1000);
                                        
                    Control_Reg_2_Write(1); //nødt til at have 2 seperate PWM signaler?
                    
                    arm2 = arm2+move;
                    PWM_1_Start(); 
                    
                    if (arm2 > 8)
                    {
                        arm2 = arm2-8;
                    }
                    else if (arm2 <= 0)
                    {
                        arm2 = arm2+8;
                    }
                    steps =1;
                    flyttil = 0;
                    
                    UART_1_PutString("Der er stack");
                    sprintf(arr1,"Arm1's nye plads: %d\r\n", arm1);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Arm2's nye plads: %d\r\n", arm2);
                    UART_1_PutString(arr2);
                } 
               
            }
        }
    }
}

CY_ISR(Count_Handler)
{ 
    
    if(steps >= 1) 
    {
        if(steps == (25*move)+1) // 75 Svare til en plads, denne ganes med hvor mange pladser den skal flytte plus 1.
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