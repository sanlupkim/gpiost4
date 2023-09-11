/*******************************************************************************
  Copyright(c) 2012 Jasem Mutlaq. All rights reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.

  The full GNU General Public License is included in this distribution in the
  file called LICENSE.
*******************************************************************************/

/**********************************************************************
  Crude kludge to use Pine64 GPIO libraries instead of Arduino

  Code tested on Pine64 2GB model running 64bit Armbian distribution.

  Jason Westervelt - 20170422

**********************************************************************/
/*********************************************************************
 * changes made for raspberry pi and wiringPi gpio driver
 * tested with rPi Zero W, Arducam IMX327 and eq3-2
 *
gpio readall
-Pi ZeroW-+---+------+---------+-----+-----+
 Physical | V | Mode | Name    | wPi | BCM |
----++----+---+------+---------+-----+-----+
  1 || 2  |   |      | 5v      |     |     |
  3 || 4  |   |      | 5v      |     |     |
  5 || 6  |   |      | 0v      |     |     |
  7 || 8  | 1 | ALT5 | TxD     | 15  | 14  |
  9 || 10 | 1 | ALT5 | RxD     | 16  | 15  |
 11 || 12 | 0 | OUT  | GPIO. 1 | 1   | 18  |   	<-- RA_plus // defined in gpiost4driver.h
 13 || 14 |   |      | 0v      |     |     |
 15 || 16 | 0 | OUT  | GPIO. 4 | 4   | 23  |	<-- DEC_plus
 17 || 18 | 0 | OUT  | GPIO. 5 | 5   | 24  | 	<-- DEC_minus
 19 || 20 |   |      | 0v      |     |     |	<-- RA_minus
 21 || 22 | 0 | OUT  | GPIO. 6 | 6   | 25  |
 23 || 24 | 1 | OUT  | CE0     | 10  | 8   |
 25 || 26 | 1 | OUT  | CE1     | 11  | 7   |

  e2rd - 20230911
**********************************************************************/

#include "gpiost4driver.h"
#include <indidevapi.h>
#include <wiringPi.h>

GPIOST4Driver::GPIOST4Driver()
{
    guideCMD[0]=0;
    debug=false;
}

GPIOST4Driver::~GPIOST4Driver()
{
    //dtor
    // usb_close(usb_handle);
}

bool GPIOST4Driver::Connect()
{
  if (wiringPiSetup () ) {
    std::cout << "Failed to configure GPIO\n" << std::endl;
    return 0;
  }
  std::cout << " Setting GPIO pin directions\n" << std::endl;
  pinMode(RA_plus, OUTPUT);
  pinMode(RA_minus, OUTPUT);
  pinMode(DEC_plus, OUTPUT);
  pinMode(DEC_minus, OUTPUT);
  std::cout << " GPIO ready\n" << std::endl;
  return 1;
}

bool GPIOST4Driver::Disconnect()
{
  //write_blocking("DISCONNECT#");

  //  close(this->fd);
    return true;
}

bool GPIOST4Driver::startPulse(int direction)
{
    switch (direction)
    {
      case GPIOST4_NORTH:
        if (debug) IDLog("Start North\n");
          digitalWrite(DEC_plus, HIGH);
        break;

        case GPIOST4_WEST:
        if (debug) IDLog("Start West\n");
          digitalWrite(RA_minus, HIGH);
        break;

        case GPIOST4_SOUTH:
        if (debug) IDLog("Start South\n");
          digitalWrite(DEC_minus, HIGH);
        break;

        case GPIOST4_EAST:
        if (debug) IDLog("Start East\n");
          digitalWrite(RA_plus, HIGH);
        break;
    }
    return 1;
   
}

bool GPIOST4Driver::stopPulse(int direction)
{
    switch (direction)
    {
        case GPIOST4_NORTH:
        if (debug) IDLog("Stop North\n");
          digitalWrite(DEC_plus, LOW);
        break;

        case GPIOST4_WEST:
        if (debug) IDLog("Stop West\n");
          digitalWrite(RA_minus, LOW);
        break;

        case GPIOST4_SOUTH:
        if (debug) IDLog("Stop South\n");
          digitalWrite(DEC_minus, LOW);
        break;

        case GPIOST4_EAST:
        if (debug) IDLog("Stop East\n");
          digitalWrite(RA_plus, LOW);
        break;
    }
    return 1;
}

