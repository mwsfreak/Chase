/* 
* Sensor.h
*
* Created: 13-11-2020 12:30:03
* Author: HansK
*/


#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <stdbool.h>
#include <avr/io.h>

class Sensor
{
//functions
public:
	Sensor();
	bool getSensorVal();
private:
	void init();

}; //Sensor

#endif //__SENSOR_H__
