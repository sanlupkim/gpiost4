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

#ifndef GPIOST4DRIVER_H
#define GPIOST4DRIVER_H


/* Standard headers */
#include <iostream>

/* #define RA_plus 12
#define DEC_plus 16
#define DEC_minus 18
#define RA_minus 22 */

//wPi style
#define RA_plus 1
#define DEC_plus 4
#define DEC_minus 5
#define RA_minus 6

enum
{
    GPIOST4_NORTH,
    GPIOST4_SOUTH,
    GPIOST4_EAST,
    GPIOST4_WEST,
};

class GPIOST4Driver
{
    public:
    GPIOST4Driver();
    virtual ~GPIOST4Driver();

    //  Generic indi device entries
    bool Connect();
    bool Disconnect();

    bool startPulse(int direction);
    bool stopPulse(int direction);

    void setDebug(bool enable) { debug = enable; }

private:
    char guideCMD[1];
    bool debug;
};

#endif // GPIOST4Driver_H
