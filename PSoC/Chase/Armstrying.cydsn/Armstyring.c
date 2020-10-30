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

   void direction(void)
{
    UART_1_PutString("Direction changed\r\n");
   Control_Reg_1_Write(~Control_Reg_1_Read());
}

   void off(void)
{
    UART_1_PutString("Sluk\r\n");
    speed = 0;
}

void move(void)
{
    UART_1_PutString("Full Step Mode one round\r\n");
    steps = 1;
}

void handleByteReceived(uint8_t byteReceived)
{
    switch(byteReceived)
    {
        case 's' :
         {
            direction();
         }
        break;
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
        case '0' :
         {
            off();
         }
        break;
        case 'i' :
         {
            move();
         }
    }
 }

int checkNumbersofSteps(int Arm1, int Arm2, int FlytTil)
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

bool checkStack(int numberOfSteps, int Arm1, int Arm2)
{
	if (numberOfSteps == 1 && Arm2 == (Arm1+1))
		return true;
	else
	return false;
}