#include "project.h"
#include <stdbool.h>

void handleByteReceived(uint8_t byteReceived);
void off(void);
void decrease(void);
void increase (void);
void switchdir(void);
void stop(void);
void direction(void);
void move(void);


volatile uint8 steps;
volatile uint8 speed;

int checkNumbersofSteps(int, int, int);
bool checkStack(int, int, int);