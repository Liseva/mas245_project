/*
 * oppgave6.cpp
 *
 * Created: 10/19/2022 9:56:31 AM
 * Author : lisev
 */ 

#define F_CPU 1000000 //1MHz funnet i datablad.
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	//DDRC = (1 << PC5);
	
	DDRD  = 0b01100000; //detclarering av aktiv bit.
	
	TCCR0A = _BV(COM0A1) |  _BV(WGM00); //A1 er connected til timer 0 - WGM er mode - fast pwm mode
	TCCR0B = _BV(CS00); // gjør at den ikke preskalerer
	


	
	while(1){  //Infinite loop
		
		for(int i = 1; i<255; ++i) //kjører fra 0->255 bit (svak til sterk)
		{
				OCR0A = i; 
				_delay_ms(10);
		}
		
		
		for(int i = 255; i>1; --i)//kjører fra 255->0 bit(sterk til svak)
		{
			OCR0A = i;
			_delay_ms(10);
		}
	
	}
}

