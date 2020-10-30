#include "project.h"
#include "stdio.h"
#include <stdbool.h>

void handleByteReceived(uint8_t byteReceived);
void off(void);
void decrease(void);
void increase (void);
void switchdir(void);
void stop(void);
void direction(uint8 dir);
void move(void);


volatile uint16 steps;
volatile uint8 speed;
volatile int8 arm1;
volatile int8 arm2;
volatile uint8 flyttil;
volatile int8 move1;
volatile int8 move2;


int checkNumbersofSteps(int, int, int);
bool checkStack(int, int, int);