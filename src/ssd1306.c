/*
 * ssd1306.c:
 *	Copyright (c) 2013 Michael Kleiber
 *
 * 	Based upon Python SSD1306 code from:
 * 	http://guy.carpenter.id.au/gaugette/about/
 * 	by Guy Guy Carpenter
 *
 * 	Based upon Adafruit's Arduino library:
 * 	https://github.com/adafruit/Adafruit_SSD1306
 * 	by Limor Fried/Ladyada for Adafruit Industries.
 *
 *
 *    libssd1306 is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as
 *    published by the Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    libssd1306 is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with libssd1306.
 *    If not, see <http://www.gnu.org/licenses/>.
 */

#include "ssd1306/ssd1306.h"


#include "rpiIO.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLADDRESS 0x21
#define SSD1306_PAGEADDRESS 0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

#define SSD1306_MEMORY_MODE_HORIZ 0x00
#define SSD1306_MEMORY_MODE_VERT  0x01
#define SSD1306_MEMORY_MODE_PAGE  0x02


/* Scrolling #defines */
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A


typedef struct _SSD1306 {
	int spiChannel;
	int maxClockSpeed;
	int resetPin;
	int dcPin;
	SSD1306_ROWS rowtype;
	SSD1306_VCCSTATE vccstate;

	unsigned char * bitmap;
	int width;
	int height;

	RPIIO_SPI * spi;
} SSD1306;


SSD1306 *
ssd1306_create(int spiChannel, int resetPin, int dcPin, int maxClockSpeed)
{
	SSD1306 * p = malloc(sizeof(SSD1306));

	p->spiChannel = spiChannel;
	p->resetPin = resetPin;
	p->dcPin = dcPin;
	p->maxClockSpeed = maxClockSpeed;
	p->rowtype = SSD1306_ROWS_NONE;
	p->vccstate = SSD1306_VCCSATE_NONE;

	p->bitmap = NULL;
	p->width = 0;
	p->height = 0;

	p->spi = NULL;

	return p;
}

void
ssd1306_destroy(SSD1306 * p)
{
	if (!p) return;

	rpiIO_spiDestroy(p->spi);
	free(p);
}


static void
begin(SSD1306 * p)
{
	ssd1306_delay(1);

	ssd1306_reset(p);

	ssd1306_command(p, SSD1306_DISPLAYOFF);
	ssd1306_command(p, SSD1306_SETDISPLAYCLOCKDIV);
	ssd1306_command(p, 0x80);
	ssd1306_command(p, SSD1306_SETMULTIPLEX);
	if (p->rowtype == SSD1306_ROWS_32) {
		ssd1306_command(p, 0x1F);
	}
	else if (p->rowtype == SSD1306_ROWS_64) {
		ssd1306_command(p, 0x3F);
	}
	ssd1306_command(p, SSD1306_SETDISPLAYOFFSET);
	ssd1306_command(p, 0x00);
	ssd1306_command(p, SSD1306_SETSTARTLINE | 0x00);

	ssd1306_command(p, SSD1306_CHARGEPUMP);
	if (p->vccstate == SSD1306_VCCSATE_EXTERNAL) {
		ssd1306_command(p, 0x10);
	}
	else {
		ssd1306_command(p, 0x14);
	}

	ssd1306_command(p, SSD1306_MEMORYMODE);
	ssd1306_command(p, 0x00);
	ssd1306_command(p, SSD1306_SEGREMAP | 0x01);
	ssd1306_command(p, SSD1306_COMSCANDEC);

	if (p->rowtype == SSD1306_ROWS_32) {
		ssd1306_command(p, SSD1306_SETCOMPINS);
		ssd1306_command(p, 0x02);
		ssd1306_command(p, SSD1306_SETCONTRAST);
		ssd1306_command(p, 0x8F);
	}
	else if (p->rowtype == SSD1306_ROWS_64) {
		ssd1306_command(p, SSD1306_SETCOMPINS);
		ssd1306_command(p, 0x12);
		ssd1306_command(p, SSD1306_SETCONTRAST);
		if (p->vccstate == SSD1306_VCCSATE_EXTERNAL) {
			ssd1306_command(p, 0x9F);
		}
		else {
			ssd1306_command(p, 0xCF);
		}
	}

	ssd1306_command(p, SSD1306_SETPRECHARGE);
	if (p->vccstate == SSD1306_VCCSATE_EXTERNAL) {
		ssd1306_command(p, 0x22);
	}
	else {
		ssd1306_command(p, 0xF1);
	}

	ssd1306_command(p, SSD1306_SETVCOMDETECT);
	ssd1306_command(p, 0x40);
	ssd1306_command(p, SSD1306_DISPLAYALLON_RESUME);
	ssd1306_command(p, SSD1306_NORMALDISPLAY);

	ssd1306_command(p, SSD1306_DISPLAYON);
}

void
ssd1306_init(SSD1306 * p, SSD1306_ROWS rowtype, SSD1306_VCCSTATE vccstate)
{
	if (!p) return;

	p->rowtype = rowtype;
	p->vccstate = vccstate;

	if (rowtype == SSD1306_ROWS_32) {
		p->width = 128;
		p->height = 32;
	}
	if (rowtype == SSD1306_ROWS_64) {
		p->width = 128;
		p->height = 64;
	}

	p->bitmap = (unsigned char *)(calloc(p->width * p->height / 8, sizeof(unsigned char)));

	const char * devName = (p->spiChannel==0?"/dev/spidev0.0":"/dev/spidev0.1");
	p->spi = rpiIO_spiCreate(devName, p->maxClockSpeed);

	rpiIO_initGPIO();
	rpiIO_pinMode(p->resetPin, PINMODE_OUTPUT);
	rpiIO_digitalWrite(p->resetPin, 1);
	rpiIO_pinMode(p->dcPin, PINMODE_OUTPUT);
	rpiIO_digitalWrite(p->dcPin, 0);

	begin(p);
}

void
ssd1306_reset(SSD1306 * p)
{
	if (!p) return;

	rpiIO_digitalWrite(p->resetPin, 0);
	ssd1306_delay(10);
	rpiIO_digitalWrite(p->resetPin, 1);
}

void
ssd1306_startScrollLeft(SSD1306 * p, int start, int stop)
{
	if (!p) return;

	ssd1306_command(p, SSD1306_LEFT_HORIZONTAL_SCROLL);
	ssd1306_command(p, 0X00);
	ssd1306_command(p, (unsigned char)start);
	ssd1306_command(p, 0X00);
	ssd1306_command(p, (unsigned char)stop);
	ssd1306_command(p, 0X01);
	ssd1306_command(p, 0XFF);
	ssd1306_command(p, SSD1306_ACTIVATE_SCROLL);
}

void
ssd1306_startScrollRight(SSD1306 * p, int start, int stop)
{
	if (!p) return;

	ssd1306_command(p, SSD1306_RIGHT_HORIZONTAL_SCROLL);
	ssd1306_command(p, 0X00);
	ssd1306_command(p, (unsigned char)start);
	ssd1306_command(p, 0X00);
	ssd1306_command(p, (unsigned char)stop);
	ssd1306_command(p, 0X01);
	ssd1306_command(p, 0XFF);
	ssd1306_command(p, SSD1306_ACTIVATE_SCROLL);
}


void ssd1306_stopScroll(SSD1306 * p)
{
	if (!p) return;

	ssd1306_command(p, SSD1306_DEACTIVATE_SCROLL);
}


void
ssd1306_displayBlock(SSD1306 * p, int row, int col, int col_count, int col_offset /* = 0 */)
{
	if (!p) return;

	/* for now always transmit whole image */
	uint8_t rows = p->height;
	uint8_t cols = p->width;

	/* devide by 8 -- black/white display 8 Bits = 1 Byte = 8 Pixels */
	uint8_t pagecount = rows >> 3;
	uint8_t pagestart = row >> 3;
	uint8_t pageend = pagestart + pagecount -1;
	uint8_t colstart = col;
	uint8_t colend = col + col_count -1;

	ssd1306_command(p, SSD1306_MEMORYMODE);
	ssd1306_command(p, SSD1306_MEMORY_MODE_VERT);

	ssd1306_command(p, SSD1306_PAGEADDRESS);
	ssd1306_command(p, pagestart);
	ssd1306_command(p, pageend);

	ssd1306_command(p, SSD1306_COLADDRESS);
	ssd1306_command(p, colstart);
	ssd1306_command(p, colend);

	int length = col_count * pagecount;

	ssd1306_data(p, p->bitmap, length);
}

unsigned char *
ssd1306_getDisplayMemory(SSD1306 * p)
{
	if (!p) return NULL;

	return p->bitmap;
}


void
ssd1306_command(SSD1306 * p, unsigned char c)
{
	if (!p) return;

	rpiIO_digitalWrite(p->dcPin, 0);
	rpiIO_spiDataRW(p->spi, &c, NULL, 1);
}

void
ssd1306_data(SSD1306 * p, unsigned char * c, int len)
{
	if (!p) return;

	rpiIO_digitalWrite(p->dcPin, 1);
	rpiIO_spiDataRW(p->spi, c, NULL, len);
	rpiIO_digitalWrite(p->dcPin, 0);
}




void
ssd1306_clear(SSD1306 * p, int color)
{
	unsigned char c = (color==0?0x00:0xFF);

	memset(p->bitmap, c, p->width * p->height / 8);
}

void
ssd1306_setPixel(SSD1306 * p, int x, int y, int color)
{
	if (!p) return;

	int col = x;
	int row = y >> 3;
	int mask = 1 << (y % 8);
	int ix = row + (p->height >> 3) * col;


	if (color == 0) {
		p->bitmap[ix] &= ~mask;
	}
	else {
		p->bitmap[ix] |= mask;
	}
}

static void swap(int * x0, int * y0)
{
	int tmp = *x0;
	*x0 = *y0;
	*y0 = tmp;
}

void
ssd1306_drawLine(SSD1306 * p, int x0, int y0, int x1, int y1, int color)
{
	/*
	 * bresenham's algorithm - thx wikpedia
	 * taken from Adafruit gfx library
	 */

	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(&x0, &y0);
		swap(&x1, &y1);
	}

	if (x0 > x1) {
		swap(&x0, &x1);
		swap(&y0, &y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0<=x1; x0++) {
		if (steep) {
			ssd1306_setPixel(p, y0, x0, color);
		} else {
			ssd1306_setPixel(p, x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void
ssd1306_display(SSD1306 * p)
{
	if (!p) return;

	ssd1306_displayBlock(p, 0, 0, 128, 0);
}

void
ssd1306_delay(unsigned int ms)
{
	struct timespec sleeper, dummy ;

	sleeper.tv_sec  = (time_t)(ms / 1000) ;
	sleeper.tv_nsec = (long)(ms % 1000) * 1000000 ;

	nanosleep (&sleeper, &dummy) ;
}



