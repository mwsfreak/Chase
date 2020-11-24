#include "project.h"
#include <stdio.h>
#include <stdint.h>
#include "Plads.h"
#include <stdbool.h>

#define ADDRESS 5

CY_ISR_PROTO(UART_RX_HANDLER);
volatile char UART_rxByte = 0;

int main(void)
{
    UART_Start();
    UART_PutString("Runnig\n\r");
    CyGlobalIntEnable;
    rx_int_StartEx(UART_RX_HANDLER);
    
    //Initialize I2C
    initPlads();
    
    //Create variables to receive
    float timerVal = 0;
    uint8_t timerValMSB;
    uint8_t timerValLSB;
    uint8_t sendToPlayer = 0;
    bool playerDone = false;
    
    //Create buffer to print rx values via UART
    char timerValBuffer[256];

    uint16_t testcnt = 0;
    
    
    startPlads(ADDRESS - 4);
    UART_PutString("Sendt byte: '1'\n\r"); //Udskriver byte
    startPlads(ADDRESS - 3);
    UART_PutString("Sendt byte: '1'\n\r"); //Udskriver byte
    startPlads(ADDRESS - 2);
    UART_PutString("Sendt byte: '1'\n\r"); //Udskriver byte
    startPlads(ADDRESS - 1);
    UART_PutString("Sendt byte: '1'\n\r"); //Udskriver byte
    startPlads(ADDRESS);
    UART_PutString("Sendt byte: '1'\n\r"); //Udskriver byte
    
    while(testcnt < 80)
    {
//        if(UART_rxByte != 0)
//        {
//            switch(UART_rxByte)
//            {
//                case 'r':
//                {
//                    //get data from ATmega8
//                    getPladsData(ADDRESS, &timerValMSB, &timerValLSB, &timerVal, &playerDone, &sendToPlayer);
//                    
//                    //Print result
//                    snprintf(timerValBuffer, sizeof(timerValBuffer), "Timer value: %f sekunder\n\rTimer val MSB: %d\n\rTimer val LSB: %d\n\rSend to player: %d\n\r", timerVal, timerValMSB, timerValLSB, sendToPlayer);
//                    UART_PutString(timerValBuffer); //Udskriver timer og sendToPlayer værdi
//                    if(playerDone == true)
//                        UART_PutString("playerDone: TRUE\n\r"); //Udskriver playerDone TRUE
//                    else
//                        UART_PutString("playerDone: FALSE\n\r"); //Udskriver playerDone FALSE
//                   
//                    break;
//                }
//                case '1':
//                {
//                    startPlads(ADDRESS);
//                    UART_PutString("Sendt byte: '1'\n\r"); //Udskriver byte
//                    break;
//                }
//                case '0':
//                {
//                    stopPlads(ADDRESS);
//                    UART_PutString("Sendt byte: '0'\n\r"); //Udskriver byte
//                    break;
//                }
//            }
//            UART_rxByte = 0; //Reset UART_rxByte
        

                    getPladsData(ADDRESS, &timerValMSB, &timerValLSB, &timerVal, &playerDone, &sendToPlayer);
                    
                    //Print result
                    snprintf(timerValBuffer, sizeof(timerValBuffer), "Timer value: %f sekunder\n\rTimer val MSB: %d\n\rTimer val LSB: %d\n\rSend to player: %d\n\r", timerVal, timerValMSB, timerValLSB, sendToPlayer);
                    UART_PutString(timerValBuffer); //Udskriver timer og sendToPlayer værdi
                    if(playerDone == true)
                        UART_PutString("playerDone: TRUE\n\r"); //Udskriver playerDone TRUE
                    else
                        UART_PutString("playerDone: FALSE\n\r"); //Udskriver playerDone FALSE
                        
                    getPladsData(ADDRESS + 2, &timerValMSB, &timerValLSB, &timerVal, &playerDone, &sendToPlayer);
                    getPladsData(ADDRESS + 3, &timerValMSB, &timerValLSB, &timerVal, &playerDone, &sendToPlayer);
                    getPladsData(ADDRESS + 2, &timerValMSB, &timerValLSB, &timerVal, &playerDone, &sendToPlayer);
                    getPladsData(ADDRESS + 1, &timerValMSB, &timerValLSB, &timerVal, &playerDone, &sendToPlayer);
                    
                    testcnt++;
        
        //}
    }
    
    stopPlads(ADDRESS+1);
    UART_PutString("Sendt byte: '0'\n\r"); //Udskriver byte
    stopPlads(ADDRESS+2);
    UART_PutString("Sendt byte: '0'\n\r"); //Udskriver byte
    stopPlads(ADDRESS+3);
    UART_PutString("Sendt byte: '0'\n\r"); //Udskriver byte
    stopPlads(ADDRESS-1);
    UART_PutString("Sendt byte: '0'\n\r"); //Udskriver byte
    stopPlads(ADDRESS);
    UART_PutString("Sendt byte: '0'\n\r"); //Udskriver byte
    
     getPladsData(ADDRESS, &timerValMSB, &timerValLSB, &timerVal, &playerDone, &sendToPlayer);
                    
                    //Print result
                    snprintf(timerValBuffer, sizeof(timerValBuffer), "Timer value: %f sekunder\n\rTimer val MSB: %d\n\rTimer val LSB: %d\n\rSend to player: %d\n\r", timerVal, timerValMSB, timerValLSB, sendToPlayer);
                    UART_PutString(timerValBuffer); //Udskriver timer og sendToPlayer værdi
                    if(playerDone == true)
                        UART_PutString("playerDone: TRUE\n\r"); //Udskriver playerDone TRUE
                    else
                        UART_PutString("playerDone: FALSE\n\r"); //Udskriver playerDone FALSE
    
    while(1)
    {}
}

CY_ISR(UART_RX_HANDLER)
{
    UART_rxByte = UART_GetChar();    
}
