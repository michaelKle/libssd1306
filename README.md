libssd1306
==========

A pure C implementation to interface the Adafruit OLEDs based on SSD1306 using the Rapsberry PI.

The library is heavily
 * 	Based upon Python SSD1306 code from:
  	http://guy.carpenter.id.au/gaugette/about/
  	by Guy Guy Carpenter
 
 * 	Based upon Adafruit's Arduino library:
 	https://github.com/adafruit/Adafruit_SSD1306
  	by Limor Fried/Ladyada for Adafruit Industries.

 *	Based upon the wiringPi code from:
  	https://projects.drogon.net/raspberry-pi/wiringpi/
  	by Gordon Henderson

This is a CMake project which allows out of source builds. Simply run:

    git clone https://github.com/michaelKle/libssd1306
    cd libssd1306
    mkdir build && cd build && cmake ../../libssd1306/ && make

The compiled library and test application can be found under build/src then. Optionally you can set an install path and run

    sudo make install
