/*
 * testGPIO.c:
 *	Copyright (c) 2013 Michael Kleiber
 *
 *  application to test the GPIO functions employed by libssd1306
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

#include <stdio.h>


#include "rpiIO.h"

#include "ssd1306/ssd1306.h"

void
usage(char * argv0)
{
	printf("Call %s with a PIN number between 0 and 25 as argument to use\n", argv0);
}

int
main(int argc, char * argv[])
{
	if (argc != 2) {
		usage(argv[0]);
		return 0;
	}



	int resetPin = 17;
	if (argc == 2) {
		sscanf(argv[1], "%d", &resetPin);
	}

	if (resetPin < 0 || resetPin > 25) {
		usage(argv[0]);
		return 0;
	}


	printf("Starting GPIO Test on GPIO PIN %d\n", resetPin);



	rpiIO_initGPIO();
	rpiIO_pinMode(resetPin, PINMODE_OUTPUT);

	int i;
	for (i = 0; i < 10; ++i) {
		printf("Setting GPIO PIN %d to %d\n", resetPin, 0);
		rpiIO_digitalWrite(resetPin, 0);
		ssd1306_delay(1000);
		printf("Setting GPIO PIN %d to %d\n", resetPin, 1);
		rpiIO_digitalWrite(resetPin, 1);
		ssd1306_delay(1000);
	}


	return 0;
}
