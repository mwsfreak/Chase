#include "project.h"
#include "stdio.h"
#include <stdbool.h>

void handleByteReceived(uint8_t byteReceived);
void off(void);
void decrease(void); // kun til test
void increase (void); // kun til test
void direction(uint8 dir);
void pickarm(uint8 choose); // pin1-4 = arm1 og pin 5-8 = arm2


volatile uint16 steps;
volatile uint8 speed;
volatile int8 arm1;
volatile int8 arm2;
volatile uint8 flyttil;
volatile int8 move;
volatile uint8 armtomove;

int checkNumbersofSteps(int8, int8, int8);
bool checkStack(int8, int8, int8);