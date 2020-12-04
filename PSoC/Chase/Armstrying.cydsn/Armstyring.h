#include "project.h"
#include "stdio.h"
#include <stdbool.h>

//void handleByteReceived(uint8_t byteReceived);
void off(void);             // Slukker for PMW-signal
void on(void);              // Tænder for PWM-signal
void direction(uint8 dir);  // Bestemmer om der skal køres med eller mod uret
int8 checkNumbersofSteps(int8, int8, int8); // Tjekker om en sendte plads er gyldig, return hvordan den skal flytte
bool checkStack(int8, int8, int8); // Tjekker om der stack (Om der rykkes til pladsen ved siden af, mens der er en)
void rykArm(uint8 choose);  // Flytter armen til den ønskede plads
void chaseArmstyringInit(); // Initiere de forskellige variabler, komponenter og interruptrutiner
void chooseArm(uint choose); // Vælger arm
//CY_ISR_PROTO(ISR_UART_rx_handler);
CY_ISR_PROTO(Count_Handler);    // Holder styr på, hvor mange steps der er blevet taget, og søger for
                                // at den tager det rigtige antal steps


// Alle variabler til flytning af armene
volatile uint8 choose, choose1, speed, arm1, arm2, flyttil, move, armtomove, stack; 
volatile uint16 steps; 
char arrr1[255];
char arr1[20] ;
char arr2[20] ;
//volatile uint8 choose1;
//volatile uint8 speed;
//volatile int8 arm1,arm2;
//volatile uint8 flyttil;
//volatile int8 move;
//volatile uint8 armtomove;
//volatile int8 stack;


