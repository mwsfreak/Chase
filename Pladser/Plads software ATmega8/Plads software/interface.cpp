/*
 * buttons.c
 *
 * Created: 13/11/2020 11.23.02
 *  Author: emil-
 */ 

#include "interface.h"
#include <avr/io.h>


	interface::interface()
	{
		init();
	}
	
	void interface::init()
	{
		//Set buttonpins to inputs
		DDRB &= ~(1 << PB0 | 1 << PB7 | 1 << PB6);
		DDRD &= ~(1 << PD7 | 1 << PD6 | 1 << PD5 | 1 << PD4);
		//Set RGB-pins to outputs
		DDRB |= (1 << PB2 | 1 << PB3 | 1 << PB4);
		
		//Enable timer 0
		TIMSK |= (1 << TOIE0); //Enable overflow interrupt

		/*
		knap 1 = PB0
		knap 2 = PD7
		knap 3 = PD6
		knap 4 = PD5
		knap 5 = PB7
		knap 6 = PB6
		knap 7 = PD4
		*/
	}
	
	int interface::getKnapstatus()
	{
		if (PINB & (1 << PB0)) //Button 1
		{
			return 1;
		}
		
		if (PIND & (1 << PD7)) //Button 2
		{
			return 2;
		}
		
		if (PIND & (1 << PD6)) //Button 3
		{
			return 3;
		}
		
		if (PIND & (1 << PD5)) //Button 4
		{
			return 4;
		}
		
		if (PINB & (1 << PB7)) //Button 5
		{
			return 5;
		}
		
		if (PINB & (1 << PB6)) //Button 6
		{
			return 6;
		}
		
		if (PIND & (1 << PD4)) //Button 7
		{
			return 7;
		}
		return 0;
	}
	
	void interface::RGBgreen()
	{
		LEDsOff();
		PORTB |= (1 << PB4);
	}

	void interface::RGBgreenBlink()
	{
		LEDsOff();
		TCCR0 = (1 << CS00 | 1 << CS02);		//Set prescaler to 1024
		TCNT0 = 0;								//reset countervalue
	}
	
	void interface::RGBred()
	{
		LEDsOff();
		PORTB |= (1 << PB2);		//Turn red LED on
	}
	
	void interface::RGBblue()
	{
		LEDsOff();
		PORTB |= (1 << PB3);		//Turn blue LED on
	}


	void interface::LEDsOff()
	{
		//Stop blink timer
		TCCR0 = 0;
		
		//Turn all LEDs off
		PORTB &= ~(1 << PB2 | 1 << PB3 | 1 << PB4);
	}
	