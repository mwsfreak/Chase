/*
 * knappanel.h
 *
 * Created: 13/11/2020 11.22.39
 *  Author: emil-
 */ 


#ifndef KNAPPANEL_H_
#define KNAPPANEL_H_


#pragma once
class knappanel
{
public:
	knappanel();
	int getKnapstatus();
	void RGBgreen();
	void RGBgreenBlink();
	void RGBblue();
	void RGBred();
private:
	void init();
	void LEDsOff();
};


#endif /* KNAPPANEL_H_ */