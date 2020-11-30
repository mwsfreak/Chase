#include "Armstyring.h"
#include "Plads.h"
#include "chaseUART.h"

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
    flyttil = 0;
    move = 10;
    stack = 0;
    gameRunning = 0;
    
    uint8_t firstRun = 1;
        
    //Initialize I2C
    initPlads();
    
    //Initialize UART to RPi
    chaseUARTinit();
    
    //Create variables to receive
    float timerVal1 = 0;
    uint8_t timerValMSB1;
    uint8_t timerValLSB1;
    uint8_t sendToPlayer1 = 0;
    bool playerDone1 = false;
    
    float timerVal2 = 0;
    uint8_t timerValMSB2;
    uint8_t timerValLSB2;
    uint8_t sendToPlayer2 = 0;
    bool playerDone2 = false;
    
    strafpoint1, strafpoint2, strafpoint3, strafpoint4, strafpoint5, strafpoint6, strafpoint7, strafpoint8 = 0;
    UART_1_PutString("Stepper motor application started\r\n");
    UART_1_PutString("e: Choose arm 1 \r\n");
    UART_1_PutString("r: Choose arm 2\r\n");
    UART_1_PutString("1-8: vaelg plads\r\n");
    UART_1_PutString("p: Print strafpoints\r\n");
    
    CyDelay(1000);
   
   for(;;)
    {
        if (gameRunning) {
            
            if (firstRun == 1) {
                arm1 = 7;           //Should be 1
                arm2 = 5;           //Should be 5
                startPlads(arm1);
                startPlads(arm2);
                firstRun = 0;
            }
            
            led_pin_Write(1);   //Turn on status led
            
            getPladsData(arm1, &timerValMSB1, &timerValLSB1, &timerVal1, &playerDone1, &sendToPlayer1);
            if (playerDone1 == true)
            {
                playerDone1 = false;  
                UART_1_PutString("\nArm1: Kommer ind i if");
                choose = 0;
                flyttil = sendToPlayer1;
                
                move = checkNumbersofSteps(arm1, arm2, flyttil);
                if (move != 10 && move != 0)
                {
                    UART_1_PutString("\nArm1: stopper plads");
                    stopPlads(arm1);
                    uint8_t arm1_temp = arm1;
                    rykArm(choose);
                    if (stack == 1) {
                        sendData(arm2-1, arm1_temp, timerValMSB1, timerValLSB1);
                        startPlads(arm2);
                        UART_1_PutString("Arm 1: Ude af startplads. Saetter stack = 0");
                        stack = 0;
                    } else {
                        sendData(0, arm1_temp, timerValMSB1, timerValLSB1);
                        startPlads(arm1);
                    }
                }
                
            }
           
            getPladsData(arm2, &timerValMSB2, &timerValLSB2, &timerVal2, &playerDone2, &sendToPlayer2);
            if (playerDone2 == true)
            {
                playerDone2 = false;
                UART_1_PutString("\nArm2: Kommer ind i if");
                choose = 1;
                flyttil = sendToPlayer2;
                
                move = checkNumbersofSteps(arm2, arm1, flyttil);
                if (move != 10 && move != 0)
                {
                    UART_1_PutString("\nArm2: stopper plads");
                    stopPlads(arm2);
                    
                    uint8_t arm2_temp = arm2;
                    rykArm(choose);
                    if (stack == 1) {
                        sendData(arm1-1, arm2_temp, timerValMSB2, timerValLSB2);
                        startPlads(arm1);
                        UART_1_PutString("Arm 2: Ude af startplads. Saetter stack = 0");
                        stack = 0;
                    } else {
                        sendData(0, arm2_temp, timerValMSB2, timerValLSB2);
                        startPlads(arm2);
                    }
                }
            }
        } else {
            if (firstRun == 0) {
                led_pin_Write(0);   //Turn off status 
                stopPlads(arm1);
                stopPlads(arm2);
                off();
                
                firstRun = 1;
            }
        }
    }
}

CY_ISR(Count_Handler)
{ 
    
    if(steps >= 1) 
    {
        if(steps == (75*move)-2) // 75 Svare til en plads, denne ganes med hvor mange pladser den skal flytte plus 1.
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