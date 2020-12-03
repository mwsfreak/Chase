#include "project.h"
#include "stdio.h"
#include <stdbool.h>

//void handleByteReceived(uint8_t byteReceived);
void off(void);
void direction(uint8 dir);
void pickarm(uint8 choose); // pin1-4 = arm1 og pin 5-8 = arm2
int8 checkNumbersofSteps(int8, int8, int8);
bool checkStack(int8, int8, int8);
void rykArm(uint8 choose); 
void chaseArmstyringinit();

//CY_ISR_PROTO(ISR_UART_rx_handler);
CY_ISR_PROTO(Count_Handler);

volatile uint8 choose;
volatile uint8 choose1;
volatile uint16 steps;
volatile uint8 speed;
volatile int8 arm1,arm2;
volatile uint8 flyttil;
volatile int8 move;
volatile uint8 armtomove;
volatile int8 stack;
char arrr1[255];
char arr1[20] ;
char arr2[20] ;