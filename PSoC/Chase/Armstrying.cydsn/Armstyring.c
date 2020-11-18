#include "Armstyring.h"


void direction(uint8 dir)
{
   Control_Reg_1_Write(dir); // sætter retning
   if (dir == 0)
   UART_1_PutString("Direction counterclockwise\r\n");
   else if (dir == 1)
   UART_1_PutString("Direction clockwise\r\n");
}

void startPWM(uint8 choose)
{
     if(choose == 0) // starter arm 1 
    {
      PWM_1_Start();  
        
    }
    else if (choose == 1) // starter arm 2 
    {
        PWM_2_Start();
    }
}
void pickarm(uint8 choose) // skal slettes når I2C implementeres
{
    if(choose == 0) // starter arm 1 
    {
      PWM_1_Start();  
        
    }
    else if (choose == 1) // starter arm 2 
    {
      PWM_2_Start();
    }
    char arr3[20];
    choose = choose+1;
    sprintf(arr3,"Arm %d er valgt\r\n", choose);
    UART_1_PutString(arr3);
} 

   void off(void)
{
    UART_1_PutString("Sluk\r\n");
    PWM_1_Stop();
    PWM_2_Stop();
}


void handleByteReceived(uint8_t byteReceived)
{
    switch(byteReceived)
    {
//        case 'q' :
//         {
//            increase();  
//         }
//        break;
//        case 'w' :
//         {
//            decrease();
//         }
//        break;
        case 'e' :
         {
            choose = 0;
            pickarm(choose);
         }
        break;
        case 'r' :
         {
            choose = 1;
            pickarm(choose);
         }
        break;
        case '1' :
         {
            flyttil = 1;
         }
        break;
        case '2' :
         {
            flyttil = 2;
         }
        break;
        case '3' :
         {
            flyttil = 3;
         }
        break;
        case '4' :
         {
            flyttil = 4;
         }
        break;
        case '5' :
         {
            flyttil = 5;
         }
        break;
        case '6' :
         {
            flyttil = 6;
         }
        break;
        case '7' :
         {
            flyttil = 7;
         }
        break;
        case '8' :
        {
            flyttil = 8;
        }
        break;
        case 'p' :
         {
            printstrafpoint();
         }        
    }
 }

int8 checkNumbersofSteps(int8 Arm1, int8 Arm2, int8 FlytTil)
{
	
	if (Arm2 == FlytTil && (Arm2 == (Arm1 +1) || (Arm1 == 8 && Arm2 == 1)))
	{
        Arm1 = 1; //retuner ack/nack, rigtig plads
	}
	else if (Arm2 == FlytTil)
	{
		Arm1 = 10; //spørg igen på i2c, forkert plads
  	}
	else
	{
        //retuner ack/nack, rigtig plads
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
	return Arm1; // Retunere arm(1/2) nye plads
}

bool checkStack(int8 numberOfSteps, int8 Arm1, int8 Arm2)
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


void printstrafpoint(void)
{
    sprintf(arrr1,"Daniel Craig har %d strafpoints\r\nArne ildsted har %d strafpoints\r\nLunar aka misterlight har %d strafpoints\r\nArthurPistol har %d strafpoints\r\nAllan'balalan har %d strafpoints\r\nHansi har %d strafpoints\r\nMagnum har %d strafpoints\r\nBananatan har %d strafpoints\r\n", strafpoint1, strafpoint2, strafpoint3, strafpoint4, strafpoint5, strafpoint6, strafpoint7, strafpoint8);
    UART_1_PutString(arrr1);
   
}

void rykArm(uint8 choose) //skal også få flyt til fra ic2 bussen. 
{
    
   
if(choose == 0)
        {
            
            if (flyttil != 0 && flyttil != arm1) //flyt til kan være over 8?
            {
                move = checkNumbersofSteps(arm1, arm2, flyttil);
                stack = checkStack(move,arm1,arm2);
                
                if (move != 10 && move != 0 && stack == 0)
                {
                    arm1 = arm1+move;
                    startPWM(choose);
                    
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
                    steps1 = 1;
                    sprintf(arr1,"Arm1's nye plads: %d\r\n", arm1);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Flytter %d pladser\r\n", move);
                    UART_1_PutString(arr2);
                }
                
             if (move != 10 && move != 0 && stack == 1)
                {
                    addstrafpoint(arm2);
                    direction(1);
                                     
                    arm2 = arm2+move;
                    choose = 1;
                    startPWM(choose); 
                    
                    if (arm2 > 8)
                    {
                        arm2 = arm2-8;
                    }
                    else if (arm2 <= 0)
                    {
                        arm2 = arm2+8;
                    }
                    steps2 =1;
                    
                   // CyDelay(1000);
                    
                   
                    choose = 0;
                    
                    arm1 = arm1+move;
                    startPWM(choose);
                    
                    if (arm1 > 8)
                    {
                        arm1 = arm1-8;
                    }
                    else if (arm1 <= 0)
                    {
                        arm1 = arm1+8;
                    }
                    flyttil = 0;
                    
                    steps1 = 1;   
                    
                    UART_1_PutString("Der er stack");
                    sprintf(arr1,"Arm1's nye plads: %d\r\n", arm1);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Arm2's nye plads: %d\r\n", arm2);
                    UART_1_PutString(arr2);
                }
            }
            //else send fejl meddelse til plads(spørg igen) ikke en valid plads
        }

else if (choose == 1)
        {
            
            if(flyttil != 0 && flyttil != arm2)
            {       
                move = checkNumbersofSteps(arm2, arm1, flyttil);
                stack = checkStack(move,arm2,arm1);
                
                if (move != 10 && move != 0 && stack == 0)
                {
                    arm2 = arm2+move;
                    startPWM(choose); 
                    
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
                    steps2 = 1;
                    sprintf(arr1,"Arm2's nye plads: %d\r\n", arm2);
                    UART_1_PutString(arr1);
                    sprintf(arr2,"Flytter %d pladser\r\n", move);
                    UART_1_PutString(arr2);
                }
               if (move != 10 && move != 0 && stack == 1)
                {
                   addstrafpoint(arm1);
                   direction(1);
                    
                   choose = 0;
                    
                    arm1 = arm1+move;
                    startPWM(choose);
                    
                    if (arm1 > 8)
                    {
                        arm1 = arm1-8;
                    }
                    else if (arm1 <= 0)
                    {
                        arm1 = arm1+8;
                    }
                   
                    steps1 = 1; 
                    
                    //CyDelay(1000);
                                        
                    choose = 1;
                    startPWM(choose);
                    
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
                    steps2 =1;
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
    

void addstrafpoint(uint8 plads)
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