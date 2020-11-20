/* 
* Sensor.cpp
*
* Created: 13-11-2020 12:30:03
* Author: HansK
*/


#include "Sensor.h"

// default constructor
Sensor::Sensor()
{
	//Initialize sensor
	init();
	
} //Sensor

bool Sensor::getSensorVal()
{
	//Check sensor value on PC3 and return true or false
	if (PINC & (1 << PC3))
	{
		return true;
	}
	else
	{
		return false;
	} 
}

void Sensor::init()
{
	//Set ball sensor pin to input
	DDRC &= ~(1 << PC3);
}
