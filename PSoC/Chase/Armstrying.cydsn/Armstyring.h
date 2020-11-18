#include "project.h"
#include "stdio.h"
#include <stdbool.h>

void handleByteReceived(uint8_t byteReceived);
void off(void);
void direction(uint8 dir);
void pickarm(uint8 choose); // pin1-4 = arm1 og pin 5-8 = arm2 // skal slettes når I2C implementeres
int8 checkNumbersofSteps(int8, int8, int8);
bool checkStack(int8, int8, int8);
void addstrafpoint(uint8 plads);
void printstrafpoint(void);
void startPWM(uint8 choose);
void rykArm(uint8);



volatile uint16 steps1, steps2;
//volatile uint8 speed;
volatile int8 arm1,arm2;
volatile uint8 flyttil;
volatile int8 move;
volatile int8 stack;
volatile uint8 strafpoint1,strafpoint2,strafpoint3,strafpoint4,strafpoint5,strafpoint6,strafpoint7,strafpoint8;
volatile uint8 choose;
char arrr1[255];
char arr1[20] ;
char arr2[20] ;