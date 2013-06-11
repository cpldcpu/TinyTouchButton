/*
 * TinyTouchButton.c
 *
 * Created: 31.05.2013 18:13:26
 *  Author: Tim

 Hardware:
	- Attiny 10
	- 4x WS2812 LED on PB2
	- Touchbutton on PB1
 */ 

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include "light_ws2812.h"
#include "TinyTouchLib.h"
	
struct CRGB { uint8_t g; uint8_t r; uint8_t b; };
struct CRGB led[4];

int main(void)
{
	uint8_t ledon=0;
	uint8_t ctr;
	
	CCP=0xD8;		// configuration change protection, write signature
	CLKPSR=0;		// set cpu clock prescaler =1 (8Mhz) (attiny 4/5/9/10)

	DDRB|=_BV(PB2);  // Output pin for WS2812

	tinytouch_init();
	
    while(1)
    {
		if (tinytouch_sense()==tt_push) {ledon^=1;}
			
		ctr++;
			
		if (ledon) {
			led[0].r=255;led[0].g=255;led[0].b=255;  // light on: white on maximum brightness
			led[1].r=255;led[1].g=255;led[1].b=255;
			led[2].r=255;led[2].g=255;led[2].b=255;
			led[3].r=255;led[3].g=255;led[3].b=255;
		} else {
			led[0].r=0;led[0].g=0;led[0].b=0;		// 	light off: Attract mode.
			led[1].r=0;led[1].g=0;led[1].b=0;
			led[2].r=0;led[2].g=0;led[2].b=0;
			led[3].r=0;led[3].g=0;led[3].b=0;
			led[ctr>>6].b=((ctr&0x3f)^0x3f)>>4;  
			led[((ctr>>6)+1)&3].b=(ctr&0x3f)>>4;
		}		
		
		ws2812_sendarray_mask(led,3*4,_BV(PB2));
		_delay_ms(3);
    }
}

