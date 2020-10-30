#include "Armstyring.h"

void increase(void)
{
   UART_1_PutString("Increasing speed\r\n");
    if(speed < 10)
     speed = 1;
    
    else
     {
         speed -= 10;
         PWM_1_WritePeriod(speed);
     }
}
void decrease (void)
{
    UART_1_PutString("Decreasing speed\r\n");
    if(speed > 245)
     speed = 255;
    else
     {
         speed += 10;
         PWM_1_WritePeriod(speed);
     }
}

void direction(uint8 dir)
{
   UART_1_PutString("Direction changed\r\n");
   Control_Reg_1_Write(dir);
}

void pickarm(uint8 choose)
{
    Control_Reg_2_Write(choose);
    armtomove = choose;
    char arr3[20];
    choose = choose+1;
    sprintf(arr3,"Arm %d er valgt\r\n", choose);
    UART_1_PutString(arr3);
}

   void off(void)
{
    UART_1_PutString("Sluk\r\n");
    PWM_1_Stop();
}


void handleByteReceived(uint8_t byteReceived)
{
    switch(byteReceived)
    {
        case 'q' :
         {
            increase();  
         }
        break;
        case 'w' :
         {
            decrease();
         }
        break;
        case 'e' :
         {
            pickarm(0); // vælger arm 1
         }
        break;
        case 'r' :
         {
            pickarm(1); // vælger arm 2
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
        
    }
 }

int checkNumbersofSteps(int8 Arm1, int8 Arm2, int8 FlytTil)
{
	
	if (Arm2 == FlytTil && Arm2 == (Arm1 + 1))
	{
		Arm1 = 1;
	}
	else if (Arm2 == FlytTil && Arm2 != (Arm1 + 1))
	{
		Arm1 = 10;
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

bool checkStack(int8 numberOfSteps, int8 Arm1, int8 Arm2)
{
	if (numberOfSteps == 1 && Arm2 == (Arm1+1))
		return true;
	else
	return false;
}