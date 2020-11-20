/*
 * interface.h
 *
 * Created: 13/11/2020 11.22.39
 *  Author: emil-
 */ 


#ifndef INTERFACE_H_
#define INTERFACE_H_


#pragma once
class interface
{
public:
	interface();
	int getKnapstatus();
	void RGBgreen();
	void RGBgreenBlink();
	void RGBblue();
	void RGBred();
	void LEDsOff();
private:
	void init();
};


#endif /* INTERFACE_H_ */