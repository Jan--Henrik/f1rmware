/*
 * This l0dable by Benedikt Roth and Stefan Tomanek serves as your main
 * viewscreen, displaying your nickname the way you want it, and lets the
 * stars zoom by. You can accelerate your vessel by pushing the joystick
 * upwards or bring it to a halt by pressing it down - leaving your ship
 * drifting in the endless space. Attach two LEDs to the modulbus
 * connectors (SS2->GND, SS5->GND), so your r0ket can light up its nacelles
 * when breaking the warp barrier.
 *
 * commit 33fe346942176a0e988818980d04d1a8f746f894 1 parent 0eaf74fa87
 * wertarbyte authored August 13, 2011
 */
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

#define NUM_STARS 100
#define SPEED_MAX 10
#define SPEED_DEFAULT 4
#define SPEED_STOP 0
#define SPEED_WARP 6


typedef struct {
	short x, y, z;
} s_star;

typedef struct {
	short speed;
} s_ship;

static s_ship ship = {SPEED_DEFAULT};

static s_star stars[NUM_STARS];

void init_star(s_star *star, int z);
void set_warp_lights(uint8_t enabled);
void drift_ship(void);

static volatile int r = 200;
static volatile int g = 0;
static volatile int b = 0;

uint8_t rgb[6];

void ram(void)
{
//////////////////////////////////////////////////////////////
uint8_t pattern[] = {
                50, 50, 0,
                50, 50, 0, 
		150,   0,  0,
                100,   100, 0,
                50,   150,   0,
                0,   150,  0 ,
                0,  100,   100,
                0,  0,   150		
                };
SETUPgout(RGB_LED);
cpu_clock_set(51);
ws2812_sendarray(pattern, sizeof(pattern));
getInputWaitRelease();
///////////////////////////////////////////////////////////////
	short centerx = RESX >> 1;
	short centery = RESY >> 1;
	short i;
	uint8_t key = 0;

	for (i = 0; i < NUM_STARS; i++) {
		init_star(stars + i, i + 1);	
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////

}

	static uint8_t count = 0;
	while(1) {
		count++;
		count%=256;
		key = getInputRaw();
		if (key == BTN_ENTER) {
			break;
		} else if ( count%4 == 0 ) {
			if (key == BTN_UP && ship.speed < SPEED_MAX) {
				ship.speed++;
			} else if (key == BTN_DOWN && ship.speed > SPEED_STOP) {
				ship.speed--;
			} else if (key ==BTN_NONE) {
				/* converge towards default speed */
				if (ship.speed < SPEED_DEFAULT) {
					ship.speed++;
				} else if (ship.speed > SPEED_DEFAULT) {
					ship.speed--;
				}
			}
		}

///////////////////////////////////////////////////////////////
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


rgb[0] = r;
rgb[1] = g;
rgb[2] = b;
rgb[3] = g;
rgb[4] = r;
rgb[5] = b;

ws2812_sendarray(rgb, sizeof(rgb));
///////////////////////////////////////////////////////////////////////
		if (ship.speed == 0 && count%6==0) drift_ship();

		int dx=0;
		int dy=0;
		setExtFont(GLOBAL(nickfont));
		setTextColor(0x00,0xFF);
		dx=DoString(0,0,GLOBAL(nickname));
		dx=(RESX-dx)/2;
		if(dx<0) dx=0;
		dy=(RESY-getFontHeight())/2;

		lcdFill(0x00);
		DoString(dx,dy,GLOBAL(nickname));

		for (i = 0; i < NUM_STARS; i++) {
			stars[i].z -= ship.speed;

			if (ship.speed > 0 && stars[i].z <= 0)
				init_star(stars + i, i + 1);

			short tempx = ((stars[i].x * 30) / stars[i].z) + centerx;
			short tempy = ((stars[i].y * 30) / stars[i].z) + centery;

			if (tempx < 0 || tempx > RESX - 1 || tempy < 0 || tempy > RESY - 1) {
				if (ship.speed > 0) { /* if we are flying, generate new stars in front */
					init_star(stars + i, i + 1);
				} else { /* if we are drifting, simply move those stars to the other end */
					stars[i].x = (((tempx%RESX)-centerx)*stars[i].z)/30;
					stars[i].y = (((tempy%RESY)-centery)*stars[i].z)/30;
				}
				continue;
			}

			lcdSetPixel(tempx, tempy, 0xFF);
			if (stars[i].z < 50) {
				lcdSetPixel(tempx + 1, tempy, 0xFF);
			}
			if (stars[i].z < 20) {
				lcdSetPixel(tempx, tempy + 1, 0xFF);
				lcdSetPixel(tempx + 1, tempy + 1, 0xFF);
			}
		}

		lcdDisplay();

		delayms_queue_plus(50,0);
	}
}


void drift_ship(void) {
	uint8_t d_x = 1;
	uint8_t d_y = 1;
	for (uint8_t i = 0; i < NUM_STARS; i++) {
		stars[i].x += d_x;
		stars[i].y += d_y;
	}
}

void init_star(s_star *star, int z)
{
	star->x = (getRandom() % RESX) - (RESX >> 1);
	star->y = (getRandom() % RESY) - (RESY >> 1);
	star->z = z;

	return;
}

