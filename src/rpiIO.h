/*
 * rpiIO.h:
 *	Copyright (c) 2013 Michael Kleiber
 *
 * 	Based upon the wiringPi code from:
 * 	https://projects.drogon.net/raspberry-pi/wiringpi/
 * 	by Gordon Henderson
 *
 * 	Based upon the sample code of the elinux wiki:
 * 	http://elinux.org/RPi_Low-level_peripherals
 *
 *  This file is part of libssd1306.
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

#ifndef _RPI_IO_H
#define _RPI_IO_H


typedef struct _RPIIO_SPI RPIIO_SPI;

typedef enum _PINMODE {
	PINMODE_NONE = 0,
	PINMODE_OUTPUT = 1,
	PINMODE_INPUT = 2
} PINMODE;


/*
 * GPIO needs to be initialized before assigning
 * values
 */
int rpiIO_initGPIO();

/*
 * set GPIO pin according to
 * http://elinux.org/RPi_Low-level_peripherals
 * as input or output
 */
void rpiIO_pinMode(int gpioPin, PINMODE mode);


/*
 * set or clear GPIO pin according to
 * http://elinux.org/RPi_Low-level_peripherals
 */
void rpiIO_digitalWrite(int gpioPin, int value);


/*
 * create a SPI object which hides
 * the internals of the SPI
 * connection
 * pointer must be release by rpiIO_spiDestroy
 */
RPIIO_SPI * rpiIO_spiCreate(const char * devName, int maxClock);

/*
 * free the memory allocated by rpiIO_spiCreate
 */
void rpiIO_spiDestroy(RPIIO_SPI *);

/*
 * transmit data over the SPI bus
 * receive and transmit buffer have to be
 * of the same length
 * they can also be the same buffer
 */

int rpiIO_spiDataRW(RPIIO_SPI * p, unsigned char * tx, unsigned char * rx, int len);

#endif
