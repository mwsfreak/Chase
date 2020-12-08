#define F_CPU 8000000
#define RELEASE_TIME_MS 500
#define DUTY_CYCLE_OPEN 9200
#define DUTY_CYCLE_CLOSED 9000

#include <util/delay.h>
#include <avr/io.h>
#include "Servo.h"

// default constructor
Servo::Servo()
{
	init();
} 

void Servo::open()
{
	OCR1A = DUTY_CYCLE_OPEN; //Set duty cycle to 10%
}

void Servo::close()
{
	OCR1A = DUTY_CYCLE_CLOSED; //Set duty cycle to 5%
}

void Servo::init()
{
	DDRB |= (1 << PB1); // Set PB1 (pin 15) as output
	TCCR1A |= (1 << COM1A0 | 1 << COM1A1); // Set OC1A on compare match when upcounting and clear when downcounting
	TCCR1B |= (1 << CS11 | 1 << WGM13); //Set prescaler to 8 and set mode phase and frequency correct
	ICR1 = 10000; //Set Top of pmw timer 1
	
	close();
}

void Servo::releaseBall()
{
	open(); //Release the ball
	
	_delay_ms(RELEASE_TIME_MS); //Wait for ball to roll out
	
	close(); //Close again
}
