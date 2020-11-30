#include "Armstyring.h"
#include "Plads.h"

void direction(uint8 dir) //sætter direction af armene ved at ændre kontrolregister.
{
   UART_1_PutString("Direction changed\r\n");
   Control_Reg_1_Write(dir);
}

void pickarm(uint8 choose) //en test funktion hvor man vælger arm ved UART.
{
    Control_Reg_2_Write(choose); //control register der styrer hvilken arm der er valgt
    armtomove = choose; //har vi ikke slette den?
    char arr3[20];
    choose = choose+1;
    sprintf(arr3,"Arm %d er valgt\r\n", choose);
    UART_1_PutString(arr3);
}

   void off(void)
{
    UART_1_PutString("Sluk\r\n");
    PWM_1_Stop(); //slukker pwm der styrer motoren.
}


void handleByteReceived(uint8_t byteReceived) //Uart funktioner til test program
{
    switch(byteReceived)
    {
        case 'e' :
         {
            pickarm(0); // vælger arm 1
            choose = 0;
         }
        break;
        case 'r' :
         {
            pickarm(1); // vælger arm 2
            choose = 1;
         }
        break;
        case '1' :
         {
            startPlads(1);
            arm2 = 1;
         }
        break;
        case '2' :
         {
            startPlads(2);
            arm2 = 2;
         }
        break;
        case '3' :
         {
            startPlads(3);
            arm2 = 3;
         }
        break;
        case '4' :
         {
            startPlads(4);
            arm2 = 4;
         }
        break;
        case '5' :
         {
            startPlads(5);
            arm2 = 5;
         }
        break;
        case '6' :
         {
            startPlads(6);
            arm2 = 6;
         }
        break;
        case '7' :
         {
            startPlads(7);
            arm2 = 7;
         }
        break;
        case '8' :
        {
            startPlads(8);
            arm2 = 8;
        }
        break;
        case 'p' :
         {
            printstrafpoint();
         }        
    }
 }

int8 checkNumbersofSteps(int8 Arm1, int8 Arm2, int8 FlytTil) //checker hvor mange steps der skal tages 
//iforhold til de to arme og flyttil
{
	
	if (Arm2 == FlytTil && (Arm2 == (Arm1 +1) || (Arm1 == 8 && Arm2 == 1))) //checker om der er stack
	{
        Arm1 = 1; //hvis der er stack skal der kun flyttes en plads
	}
	else if (Arm2 == FlytTil) //hvis man vil sende til den anden arms plads, men ikke sikre der er stack
	{
		Arm1 = 10; //fejlkode
  	}
	else
	{
		if (Arm1 > Arm2 && Arm2 > FlytTil)
		{
			Arm1 = (FlytTil + 8) - Arm1;
		}
		else if (Arm1 > Arm2 && Arm2 < FlytTil)
		{
			Arm1 = -(Arm1 - FlytTil);
		}
		else if (Arm1 < Arm2 && Arm2 > FlytTil)
		{
			Arm1 = -(Arm1 - FlytTil);
		}
		else if (Arm1 < Arm2 && Arm2 < FlytTil)
		{
			Arm1 = -(Arm1 + 8) + FlytTil;
		}
	}
	return Arm1;
}

bool checkStack(int8 numberOfSteps, int8 Arm1, int8 Arm2) //checker om der er stack og retunerer en bool
{
	if (numberOfSteps == 1)
    {
		if(Arm2 == (Arm1+1))
            return true;
        
        else if(Arm1 == 8 && Arm2 == 1)
        return true;
        
    }       

    	return false;
}


void printstrafpoint(void) //test funktion
{
    sprintf(arrr1,"Daniel Craig har %d strafpoints\r\nArne ildsted har %d strafpoints\r\nLunar aka misterlight har %d strafpoints\r\nArthurPistol har %d strafpoints\r\nAllan'balalan har %d strafpoints\r\nHansi har %d strafpoints\r\nMagnum har %d strafpoints\r\nBananatan har %d strafpoints\r\n", strafpoint1, strafpoint2, strafpoint3, strafpoint4, strafpoint5, strafpoint6, strafpoint7, strafpoint8);
    UART_1_PutString(arrr1);
   
}

void addstrafpoint(uint8 plads) //tilføjer stafpoint, skal ændres med RPI funktion.
{
    if (plads == 1)
    {
        strafpoint1++;
    }
    else if (plads == 2)
    {
        strafpoint2++;
    }
    else if (plads == 3)
    {
        strafpoint3++;
    }
    else if (plads == 4)
    {
        strafpoint4++;
    }
    else if (plads == 5)
    {
        strafpoint5++;
    }
    else if (plads == 6)
    {
        strafpoint6++;
    }
    else if (plads == 7)
    {
        strafpoint7++;
    }
    else if (plads == 8)
    {
        strafpoint8++;
    }
}

void rykArm(uint8 choose) //skal også få flyt til fra ic2 bussen. 
{  
 if(choose == 0)
        {
            if (flyttil != 0 && flyttil != arm1) //flyt til kan være over 8?
            {
                //move = checkNumbersofSteps(arm1, arm2, flyttil);
                stack = checkStack(move,arm1,arm2);
                
                
                if (move != 10 && move != 0 && stack == 0)
                {
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
                    sprintf(arr1,"Arm1's nye plads: %d\r\n", arm1);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Flytter %d pladser\r\n", move);
                    UART_1_PutString(arr2);
                }
                
             if (move != 10 && move != 0 && stack == 1)
                {
                    addstrafpoint(arm2);
                    direction(0);
                    
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
                    
                    UART_1_PutString("Der er stack\n");
                    sprintf(arr1,"Arm1's nye plads: %d\r\n", arm1);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Arm2's nye plads: %d\r\n", arm2);
                    UART_1_PutString(arr2);
                }
            }
        }
                
        else if (choose == 1)
        {
            if(flyttil != 0 && flyttil != arm2)
            {       
                //move = checkNumbersofSteps(arm2, arm1, flyttil);
                stack = checkStack(move,arm2,arm1);

                if (move != 10 && move != 0 && stack == 0)
                {
                    Control_Reg_2_Write(1);
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
                        direction(0);
                    }
                    else if (move < 0) 
                    {
                        direction(1);
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
                   direction(0);
                    
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
                    
                    UART_1_PutString("Der er stack\n");
                    sprintf(arr1,"Arm1's nye plads: %d\r\n", arm1);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Arm2's nye plads: %d\r\n", arm2);
                    UART_1_PutString(arr2);
                } 
               
            }
        }
        
}
        
