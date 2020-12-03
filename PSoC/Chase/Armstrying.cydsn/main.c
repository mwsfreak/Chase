#include "Armstyring.h"
#include "Plads.h"
#include "chaseUART.h"



int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    uint8_t firstRun = 1;
    
    //Initialize armstyring 
    chaseArmstyringinit();
    
    //Initialize I2C
    initPlads();
    
    //Initialize UART to RPi
    chaseUARTinit();
    
    
    CyDelay(1000);
    
    for(uint8_t i = 1; i <= 8; i++)
        {
            stopPlads(i);   
        }
     
   for(;;)
    {
        if (gameRunning) {
            
            if (firstRun == 1) {
                arm1 = 5;           //Should be 1
                arm2 = 6;           //Should be 5
                startPlads(arm1);
                startPlads(arm2);
                firstRun = 0;
            }
            
//            led_pin_Write(1);   //Turn on status led
            
            getPladsData(arm1, &timerValMSB1, &timerValLSB1, &timerVal1, &playerDone1, &sendToPlayer1);
            if (playerDone1 == true)
            {
                playerDone1 = false;  
                choose = 0;
                flyttil = sendToPlayer1;
                
                move = checkNumbersofSteps(arm1, arm2, flyttil);
                if (move != 10 && move != 0)
                {
                    stopPlads(arm1);
                    uint8_t arm1_temp = arm1;
                    rykArm(choose);
                    if (stack == 1) {
                        sendData(arm2-1, arm1_temp, timerValMSB1, timerValLSB1);
                        startPlads(arm2);
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
                choose = 1;
                flyttil = sendToPlayer2;
                
                move = checkNumbersofSteps(arm2, arm1, flyttil);
                if (move != 10 && move != 0)
                {
                    stopPlads(arm2);
                    
                    uint8_t arm2_temp = arm2;
                    rykArm(choose);
                    if (stack == 1) {
                        sendData(arm1-1, arm2_temp, timerValMSB2, timerValLSB2);
                        startPlads(arm1);
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
                off();
                firstRun = 1;
            }
            
            for(uint8_t i = 1; i <= 8; i++)
                {
                    stopPlads(i);   
                }
        }
    }
}

