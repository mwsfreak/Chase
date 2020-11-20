/* 
* Servo.h
*
* Created: 28-10-2020 09:10:26
* Author: HansK
*/


#ifndef __SERVO_H__
#define __SERVO_H__


class Servo
{
public:
	Servo();
	void releaseBall();
private:
	void open();
	void close();
	void init();
}; //Servo

#endif //__SERVO_H__
