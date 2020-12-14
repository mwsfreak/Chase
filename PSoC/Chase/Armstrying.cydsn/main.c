#include "Armstyring.h"
#include "Plads.h"
#include "RPi.h"



int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    uint8_t firstRun = 1; // Sættes som flag, og sikre at start pladserne kun startes en gang
    
    //Initialize armstyring 
    chaseArmstyringInit();
    
    //Initialize I2C
    initPlads();
    
    //Initialize UART to RPi
    chaseUARTinit();
    
    
    CyDelay(1000); // Giver tid til at I2C kan starte op
    
    for(uint8_t i = 1; i <= 8; i++) // Slukker alle pladser
        {
            stopPlads(i);   
        }
     
   for(;;)
    {
        if (gameRunning) {
            
            if (firstRun == 1) {
                arm1 = 1;           //Should be 1
                arm2 = 5;           //Should be 5
                startPlads(arm1);
                startPlads(arm2);
                firstRun = 0;
            }
            
           led_pin_Write(1);   //Turn on status led
            
            // Kommunikation med arm1 og den plads den er på, smat vidresending af information til RPI
        
            // I2C kommunikation med plads fra arm 1, modtager 5 parameter
            getPladsData(arm1, &timerValMSB1, &timerValLSB1, &playerDone1, &sendToPlayer1);
            if (playerDone1 == true)     // Først tjekkes om playerDone1 er sat til 1, da dette betyder spilleren er færdig
            {                            // hvis dette ikke er tilfældet gøres intet
               
                playerDone1 = false;     // Denne sættes lav, så den ikke sender 2 gange, og stopper forkerte pladser
                choose = 0;              // Valg af arm
                flyttil = sendToPlayer1; // Giver den valgt plads videre til flyttil
                
                              
                move = checkNumbersofSteps(arm1, arm2, flyttil); // Giver move antalet af pladser der skal rykkes
                
                if (flyttil != 0 && flyttil != arm1) //Sikre at det input der er givet er nyt
                {
                    stack = checkStack(move,arm1,arm2); // Tjekker for stack, og sætter stack til 1 eller 0
                }
                
                if (move != 10 && move != 0) // Ekstra sikring, så det næste kun gøres hvis den valgte plads er gyldig
                {
                    stopPlads(arm1); // Stopper en nuværende plads
                    uint8_t arm1_temp = arm1; // Laver en variable der gennem pladsen der hvor den var på (bruges i stack)
                    uint8_t arm2_temp = arm2;
                    rykArm(choose); // Rykker armen
                    if (stack == 1) { // Hvis der er stack sendes dette til RPI
                        sendData(arm2_temp, arm1_temp, timerValMSB1, timerValLSB1); // Sender tid og strafpoint til RPI
                        startPlads(arm2); // starter den nye plads
                        stack = 0; // Sætter stack lav
                    } else {
                        sendData(0, arm1_temp, timerValMSB1, timerValLSB1); // Hvis der ikke er stack, sendes der bare tider og hvem der skal have tiderne
                        startPlads(arm1); // Starter nye palds
                    }
                }
                
            }
           
            // Kommunikation med arm2 og den plads den er på, smat vidresending af information til RPI
            getPladsData(arm2, &timerValMSB2, &timerValLSB2, &playerDone2, &sendToPlayer2);
            if (playerDone2 == true)
            {
                playerDone2 = false;
                choose = 1;
                flyttil = sendToPlayer2;
                
                             
                move = checkNumbersofSteps(arm2, arm1, flyttil);
             
                if(flyttil != 0 && flyttil != arm2)
                {       
                    //move = checkNumbersofSteps(arm2, arm1, flyttil);
                    stack = checkStack(move,arm2,arm1);
                }
                if(flyttil != 0 && flyttil != arm2)
                {       
                    //move = checkNumbersofSteps(arm2, arm1, flyttil);
                    stack = checkStack(move,arm2,arm1);
                }
                
                if (move != 10 && move != 0)
                {
                    stopPlads(arm2);
                    
                    uint8_t arm1_temp = arm1;
                    uint8_t arm2_temp = arm2;
                    rykArm(choose);
                    if (stack == 1) {
                        sendData(arm1_temp, arm2_temp, timerValMSB2, timerValLSB2);
                        startPlads(arm1);
                        stack = 0;
                    } else {
                        sendData(0, arm2_temp, timerValMSB2, timerValLSB2);
                        startPlads(arm2);
                    }
                }
            }
        } else { // Slukker for spillet når gamerunning er false
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

