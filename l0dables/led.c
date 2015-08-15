#include <r0ketlib/config.h>
#include <r0ketlib/display.h>
#include <r0ketlib/fonts.h>
#include <r0ketlib/render.h>
#include <r0ketlib/print.h>
#include <r0ketlib/keyin.h>

#include <r0ketlib/itoa.h>
#include <rad1olib/pins.h>

#include <rad1olib/light_ws2812_cortex.h>
#include <rad1olib/setup.h>

#include "usetable.h"

static volatile int r = 200;
static volatile int g = 0;
static volatile int b = 0;

//int i;

uint8_t rgb[24];

int counter = 0;


uint8_t rgb[] = {
                50, 50, 0,
                50, 50, 0, 
		150,   0,  0,
                100,   100, 0,
                50,   150,   0,
                0,   150,  0 ,
                0,  100,   100,
                0,  0,   150		
                };

void ram(void)
{
//////////////////////////////////////////////////////////////
	SETUPgout(RGB_LED);
	cpu_clock_set(51);
/////////////////////////////////////////////////////////////
	while(1){

		counter++;

			if ( r%5 != 0 || g%5 !=0||b%5!=0){
				r=200;
				g=0;
				b=0;
			}

			if(r == 200 && b > 0){
				b -= 5;
			}
			else if(b == 200 && g == 0){
				r += 5;
			}
			else if(b == 200 && g > 0 ){
				g -= 5;
			}
			else if(g == 200 && r == 0){
				b += 5;
			}
			else if(g == 200 && r > 0){
				r -= 5;
			}
			else if(r == 200 && b == 0){
				g += 5;
			}
			else {
				r=200;
				g=0;
				b=0;
			}

if( counter <= 5000 ){

rgb[0] = r;
rgb[1] = g;
rgb[2] = b;
rgb[3] = b;
rgb[4] = r;
rgb[5] = g;

rgb[6] = 150;
rgb[7] = 0;
rgb[8] = 0;
rgb[9] = 100;
rgb[10] = 100;
rgb[11] = 0;
rgb[12] = 50;
rgb[13] = 150;
rgb[14] = 0;
rgb[15] = 0;
rgb[16] = 150;
rgb[17] = 0;
rgb[18] = 0;
rgb[19] = 100;
rgb[20] = 100;
rgb[21] = 0;
rgb[22] = 0;
rgb[23] = 150;

}
else
{

		rgb[21] = rgb[18];
		rgb[22] = rgb[19];
		rgb[23] = rgb[20];

		rgb[18] = rgb[15];
		rgb[19] = rgb[16];
		rgb[20] = rgb[17];

		rgb[15] = rgb[12];
		rgb[16] = rgb[13];
		rgb[17] = rgb[14];

		rgb[12] = rgb[9];
		rgb[13] = rgb[10];
		rgb[14] = rgb[11];

		rgb[9] = rgb[6];
		rgb[10] = rgb[7];
		rgb[11] = rgb[8];

		rgb[6] = rgb[3];
		rgb[7] = rgb[4];
		rgb[8] = rgb[5];

		rgb[3] = rgb[0];
		rgb[4] = rgb[1];
		rgb[5] = rgb[2];

		rgb[0] = r;
		rgb[1] = g;
		rgb[2] = b;
}
if ( counter >= 10000 ){
		counter = 0;
}

		ws2812_sendarray(rgb, sizeof(rgb));
		getInputWaitRelease();


	if(getInputRaw()!=0){

			uint8_t off[] = {
        	        	0, 0, 0,
        	        	0, 0, 0, 
        	        	0, 0, 0,
        	        	0, 0, 0,
       		        	0, 0, 0 ,
               	 		0, 0, 0,
               			0, 0, 0,
				0, 0, 0
                	};

			

			ws2812_sendarray(off, sizeof(off));
			getInputWaitRelease();
			return;
		}
	}
}
