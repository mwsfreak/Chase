/* 
* Timer.cpp
*
* Created: 04-11-2020 09:44:19
* Author: HansK
*/

#include "Timer.h"
#include <math.h>

// default constructor
Timer::Timer()
{
	init();
} 

void Timer::init()
{
	TCCR2 &= ~((1 << WGM20) | (1 << WGM21)); //Set timer 2 to normal mode
	TIMSK |= (1 << TOIE2); //Enable overflow interrupt
	stop();
}

void Timer::start()
{
	TCCR2 |= ((1 << CS20) | (1 << CS21) | (1 << CS22)); // Set prescaler to 1024
}
void Timer::stop()
{
	TCCR2 &= ~((1 << CS20) | (1 << CS21) | (1 << CS22)); // Set prescaler to no clock source
	TCNT2 = 0; // Reset timer value
}

