#include "Armstyring.h"
#include "Plads.h"


void chaseArmstyringInit()
{
    UART_1_Start();
    choose, choose1 = 0;
    speed = 1;
    steps = 0;
    armtomove = 0;
    flyttil = 0;
    move = 10;
    stack = 0;
//    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    isr_1_StartEx(Count_Handler); 
}
void direction(uint8 dir) //sætter direction af armene ved at ændre kontrolregister.
{
   UART_1_PutString("Direction changed\r\n");
   Control_Reg_1_Write(dir); // Ændre retning alt efter om dir er 1 eller 0 
}

void off(void)
{
    UART_1_PutString("Sluk\r\n");
    PWM_1_Stop(); //slukker pwm der styrer motoren.
}

void on(void)
{
    PWM_1_Start();   
}

void chooseArm(uint choose)
{
   Control_Reg_2_Write(choose);
}

//void handleByteReceived(uint8_t byteReceived) //Uart funktioner til test program
//{
//    switch(byteReceived)
//    {
//        case 'e' :
//         {
//            pickarm(0); // vælger arm 1
//            choose = 0;
//         }
//        break;
//        case 'r' :
//         {
//            pickarm(1); // vælger arm 2
//            choose = 1;
//         }
//        break;
//        case '1' :
//         {
//            startPlads(1);
//            arm2 = 1;
//         }
//        break;
//        case '2' :
//         {
//            startPlads(2);
//            arm2 = 2;
//         }
//        break;
//        case '3' :
//         {
//            startPlads(3);
//            arm2 = 3;
//         }
//        break;
//        case '4' :
//         {
//            startPlads(4);
//            arm2 = 4;
//         }
//        break;
//        case '5' :
//         {
//            startPlads(5);
//            arm2 = 5;
//         }
//        break;
//        case '6' :
//         {
//            startPlads(6);
//            arm2 = 6;
//         }
//        break;
//        case '7' :
//         {
//            startPlads(7);
//            arm2 = 7;
//         }
//        break;
//        case '8' :
//        {
//            startPlads(8);
//            arm2 = 8;
//        }
//        break;
//           
//    }
// }


// Ikke nødvendigt at tjekke efter om FlytTil at mindre ned 1 eller større end 8, da det kun er tal her imellem
// der kan sendes af plads
int8 checkNumbersofSteps(int8 Arm1, int8 Arm2, int8 FlytTil) //checker hvor mange steps der skal tages  
{                                                            //iforhold til de to arme og flyttil                
	uint8 numberOfSteps = 0;
	if (Arm2 == FlytTil && (Arm2 == (Arm1 +1) || (Arm1 == 8 && Arm2 == 1))) //checker om der er stack
	{
        numberOfSteps = 1; //hvis der er stack skal der kun flyttes en plads
//        stack = 1; Skal testes
	}
	else if (Arm2 == FlytTil) //hvis man vil sende til den anden arms plads, men der ikke er stack
	{
		numberOfSteps = 10; //fejlkode
  	}
	else
	{
        // Logik der sikre at de 2 arme ikke støder ind i hinanden. Tager ud gangspunkt i at der er 4 mulig
        // kombinationer når en arm skal flytte, når der tages forhold til hvor den anden arm er, og hvor 
        // der flyttes til
		if (Arm1 > Arm2 && Arm2 > FlytTil) // Tilfælde 1 
		{
			numberOfSteps = (FlytTil + 8) - Arm1;
		}
		else if (Arm1 > Arm2 && Arm2 < FlytTil) // Tilfælde 2 
		{
			numberOfSteps = -(Arm1 - FlytTil);
		}
		else if (Arm1 < Arm2 && Arm2 > FlytTil) // Tilfælde 3 
		{
			numberOfSteps = -(Arm1 - FlytTil);
		}
		else if (Arm1 < Arm2 && Arm2 < FlytTil) // Tilfælde 4
		{
			numberOfSteps = -(Arm1 + 8) + FlytTil;
		}
	}
	return numberOfSteps;
}

bool checkStack(int8 numberOfSteps, int8 Arm1, int8 Arm2) //checker om der er stack og retunerer en bool
{
	if (numberOfSteps == 1) // Da det kun er tilfælde hvor numberOfSteps er 1 der kan være stack tjekkes dette først
    {
		if(Arm2 == (Arm1+1)){ // Hvis NumberOfSteps er 1, skal pladsen til venstre være 
            return true;     // den anden plads for at der kommer stack, dette tjekeks her
        }
        else if(Arm1 == 8 && Arm2 == 1){ // Denne tjekker særtilfældet hvor den krydser fra plads 8 til plads 1
        return true;
        }
    }       
    	return false; // Hvis ingen af overstående tilfælder er sande, returneres falsk altså ingen stack. 
}

void rykArm(uint8 choose) //skal også få flyt til fra ic2 bussen. 
{  
 if(choose == 0) // Da choose bestemmer hvilke arm der arbejdes med, tjekkes dette først
        {
            

                if (move != 10 && move != 0 && stack == 0) // Tjekker om inputtet er vaild og om der er stack
                {
                    chooseArm(0); // Vælger arm 1
                    arm1 = arm1+move; // Finder armens nye "plads"
                   
                    
                    // Logik der sikre at den flytter til den rigtige plads, så hvis den kommer over 8 eller under 0
                    // tages der forhold til det, plus den får den til at køre den rigtige vej
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
                        direction(0);
                    }
                    else if (move < 0) 
                    {
                        direction(1);
                        move = move*-1;
                    }
            
                    flyttil = 0; // nulstiller flyttil, så den ikke dobbelt flytter
                      
                    
                    steps = 1; // Gør det muligt at tælle steps
                    on(); // Tænder PWM
//                    while(steps >= 1) //hvis motoren er igang gør intet
//                    {}
                    
                    
                    sprintf(arr1,"Arm1's nye plads: %d\r\n", arm1);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Flytter %d pladser\r\n", move);
                    UART_1_PutString(arr2);
                }
                
             if (move != 10 && move != 0 && stack == 1) // Dette er hvis stack er sat
                {
                    
                    direction(0); // Skal altid rykke mod venstre 
                    
                    chooseArm(1); // Vælger først armen der bliver stacket på, så den rykker først og sikre de ikke kolidere
                    
                    arm2 = arm2+move;  
                    
                    
                    if (arm2 > 8) 
                    {
                        arm2 = arm2-8;
                    }
                    else if (arm2 <= 0)
                    {
                        arm2 = arm2+8;
                    }
                    
                    steps =1;
                    on(); // Tænder PWM 
//                    while(steps >= 1) //hvis motoren er igang gør intet
//                    {}
                    //CyDelay(1000); // Venter 1 sek på at armen er færdig med at rykke, og starter så med at rykke den anden arm
                                       
                    chooseArm(0); // Vælger så den anden arm, og gør det samme
                    
                    arm1 = arm1+move;
                    
                    
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
                    on();
//                    while(steps >= 1) //hvis motoren er igang gør intet
//                    {}
                    
                      
                    
                    UART_1_PutString("Der er stack\n");
                    sprintf(arr1,"Arm1's nye plads: %d\r\n", arm1);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Arm2's nye plads: %d\r\n", arm2);
                    UART_1_PutString(arr2);
                }
            
        }
                
        else if (choose == 1) // Samme som overnævnte bare med den anden arm
        {

                if (move != 10 && move != 0 && stack == 0)
                {
                    chooseArm(1);
                    arm2 = arm2+move;
                    
                    
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
                        direction(0);
                    }
                    else if (move < 0) 
                    {
                        direction(1);
                        move = move*-1;
                    }
            
                    flyttil = 0;
                   
                    steps = 1;
                    on();
//                    while(steps >= 1) //hvis motoren er igang gør intet
//                    {}
                    
                    sprintf(arr1,"Arm2's nye plads: %d\r\n", arm2);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Flytter %d pladser\r\n", move);
                    UART_1_PutString(arr2);
                }
               if (move != 10 && move != 0 && stack == 1)
                {
                   
                   direction(0);
                    
                   chooseArm(0);
                    
                    arm1 = arm1+move;
                    
                    
                    if (arm1 > 8)
                    {
                        arm1 = arm1-8;
                    }
                    else if (arm1 <= 0)
                    {
                        arm1 = arm1+8;
                    }
                  
                    
                    steps = 1; 
                   on();
//                    while(steps >= 1) //hvis motoren er igang gør intet
//                    {}
                    
                    
                    //CyDelay(1000); Delay ikke nødvendigt når man venter på motor
                                        
                    chooseArm(1); //nødt til at have 2 seperate PWM signaler?
                    
                    arm2 = arm2+move;
                    
                    
                    if (arm2 > 8)
                    {
                        arm2 = arm2-8;
                    }
                    else if (arm2 <= 0)
                    {
                        arm2 = arm2+8;
                    }
                    
                    steps =1;
                    on();
//                    while(steps >= 1) //hvis motoren er igang gør intet
//                    {}
                    
                                        
                    flyttil = 0;
                    
                    UART_1_PutString("Der er stack\n");
                    sprintf(arr1,"Arm1's nye plads: %d\r\n", arm1);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Arm2's nye plads: %d\r\n", arm2);
                    UART_1_PutString(arr2);
                } 
               
            
        }
        
}
    
CY_ISR(Count_Handler) // Den funktion der holder styr på hvor mange steps der er taget
{ 
    
    if(steps >= 1) // Startes hvis steps bliver 1 eller der over
    {
        if(steps >= (75*move)-2) // 75 Svare til en plads, denne ganes med hvor mange pladser den skal flytte minus 2,
        {                        // da dette er en systematisk fejl
            
            off();               // Slukekr PWM 
            steps = 0;           // Og sikre at den ikke tæller mere
        }  
        else 
        {
            steps=steps+1;       // Tæller steps op 
        }   
    }
    PWM_1_ReadStatusRegister(); // Læser på interrupt og clear det
}

//CY_ISR(ISR_UART_rx_handler)
//{
//    uint8_t bytesToRead = UART_1_GetRxBufferSize();
//    while (bytesToRead > 0)
//    {
//        uint8_t byteReceived = UART_1_ReadRxData();
//        UART_1_WriteTxData(byteReceived); // echo back
//        
//        handleByteReceived(byteReceived);
//        bytesToRead--;
//    }
//}
