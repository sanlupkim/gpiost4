# indi_gpiost4

Fork https://github.com/zenmetsu/gpiost4 for rPi and wiringPi.

This is a dirty hack to use the minimal INDI (http://www.indilib.org/) driver for Kevin Ferrares Arduino 
based USB to ST-4 adapter (https://github.com/kevinferrare/arduino-st4).  Basically, I cut out the middle-man
and used the Pine64 GPIO libraries instead of making USB serial calls to a slave microcontroller.  

The Pine64 and the RaspberryPI have truckloads of GPIO, no need to complicate matters by incorporating 
another device/battery-muncher. 


## License

This is based on the GPUSB driver and as such under the GPL2. 

## Features/Limitations
  * No way^H^H^Hneed to dynamically assign a tty
  * No way to dynamically assign GPIO pins
  * No Goto, just North, West, South, East
  * Only works on Raspberry Pi

## To Do
  * Add support for dynamic GPIO assignment
  * Find out how to allow clients such as KStars to dynamically load this driver as 
    it doesn't show up in the list currently

## Configuring
  * GPIO pin assignments made in gpiost4driver.h
  * Need to specify 4 pins total (RA+/- and DEC+/-)   

## Building

    install wiringPi libs
    cmake .
    make

## Running

    indiserver ./indi_gpiost4
