/*
 * ssd1306.h:
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

#ifndef _SSD1306_H
#define _SSD1306_H

/*
 * opaque handle
 * details defined in implementation file
 */
typedef struct _SSD1306 SSD1306;

/*
 * library works for 32 and 64 row displays
 */
typedef enum _SSD1306_ROWS {
	SSD1306_ROWS_NONE = 0,
	SSD1306_ROWS_32 = 32,
	SSD1306_ROWS_64 = 64
} SSD1306_ROWS;

typedef enum _SSD1306_VCCSTATE {
	SSD1306_VCCSATE_NONE = 0,
	SSD1306_VCCSATE_EXTERNAL = 1,
	SSD1306_VCCSATE_SWITCHCAP = 2
} SSD1306_VCCSTATE;


/*
 * allocates new handle on heap
 * must be destroyed by ssd1306_destroy
 * configures SPI connection
 * configures reset and dc Pin
 * the numbering follows the GPIO numbering of the BCM2835
 * http://elinux.org/RPi_Low-level_peripherals#General_Purpose_Input.2FOutput_.28GPIO.29
 * device must be available when calling this function
 */
SSD1306* ssd1306_create(int spiChannel, int resetPin, int dcPin, int maxClockSpeed);

/*
 * destroy object and close SPI connection
 */
void ssd1306_destroy(SSD1306 * p);


/*
 * setup connection for display
 */
void ssd1306_init(SSD1306 * p, SSD1306_ROWS rowtype, SSD1306_VCCSTATE vccstate);


/*
 * reset display
 */
void ssd1306_reset(SSD1306 * p);


/*
 * start scrolling to the left
 */
void ssd1306_startScrollLeft(SSD1306 * p, int start, int stop);


/*
 * start scrolling to the right
 */
void ssd1306_startScrollRight(SSD1306 * p, int start, int stop);

/*
 * stop scrolling
 */

void ssd1306_stopScroll(SSD1306 * p);

/*
 * set (a part of) the memory of the display
 * by using the internal memory
 */
void ssd1306_displayBlock(SSD1306 * p, int row, int col, int col_count, int col_offset /* = 0 */);

/*
 * aquire the pointer to the memory that is
 * used for drawing and when displaying
 * this memory block is (width * height / 8) Bytes
 * the data is stored in column major order
 */
unsigned char * ssd1306_getDisplayMemory(SSD1306 * p);

/*
 * send command to display
 * attributes of setup commands are also commands
 * only the actual display data is sent as data
 */
void ssd1306_command(SSD1306 * p, unsigned char c);

/*
 * send data to display
 */
void ssd1306_data(SSD1306 * p, unsigned char * c, int len);






/*
 * Convenience functions
 * Drawing should actually be handled by another library
 */


/*
 * set whole display memory to either black or white
 */
void ssd1306_clear(SSD1306 * p, int color);

/*
 * set one pixel of the display
 * this is just a test function
 * the actual drawing should be handled
 * by a different library
 * col = 0 = black, col != 0 = white
 */
void ssd1306_setPixel(SSD1306 * p, int x, int y, int col);


/*
 * draw a line
 * col = 0 = black, col != 0 = white
 */
void ssd1306_drawLine(SSD1306 * p, int x0, int y0, int x1, int y1, int color);

/*
 * update the display to reflect the libraries internal buffer
 */
void ssd1306_display(SSD1306 * p);


/*
 * forward the delay function of wiringPi
 * Convenience
 */
void ssd1306_delay(unsigned int ms);





#endif

