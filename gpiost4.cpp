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
  Minor modifications to use Pine64 GPIO libraries instead of Arduino

  Code tested on Pine64 2GB model running 64bit Armbian distribution.

  Jason Westervelt - 20170422

**********************************************************************/

#include "gpiost4.h"
#include "gpiost4driver.h"
#include <memory>
#include <cstring>
#include <unistd.h>

// We declare an auto pointer to gpGuide.
std::unique_ptr<GPIOST4> gpGuide(new GPIOST4());

GPIOST4::GPIOST4()
{
    setVersion(1, 0);
    driver = new GPIOST4Driver();
}

GPIOST4::~GPIOST4()
{
    delete (driver);
}

const char *GPIOST4::getDefaultName()
{
    return static_cast<const char *>("GPIO ST4");
}

bool GPIOST4::Connect()
{
    driver->setDebug(isDebug());

    bool rc = driver->Connect();

    if (rc)
        IDMessage(getDeviceName(), "GPIO ST4 is online.");
    else
        IDMessage(getDeviceName(), "Error: cannot find GPIO ST4 device.");

    return rc;

}

bool GPIOST4::Disconnect()
{
    IDMessage(getDeviceName(), "GPIO ST4 is offline.");

    return driver->Disconnect();
}

bool GPIOST4::initProperties()
{
    INDI::DefaultDevice::initProperties();

    initGuiderProperties(getDeviceName(), MAIN_CONTROL_TAB);

    setDriverInterface(AUX_INTERFACE | GUIDER_INTERFACE);

    addDebugControl();

    setDefaultPollingPeriod(250);

    return true;
}

bool GPIOST4::updateProperties()
{

    INDI::DefaultDevice::updateProperties();

    if (isConnected())
    {
        defineProperty(&GuideNSNP);
        defineProperty(&GuideWENP);
    }
    else
    {
        deleteProperty(GuideNSNP.name);
        deleteProperty(GuideWENP.name);
    }

    return true;

}

void GPIOST4::ISGetProperties (const char *dev)
{
    INDI::DefaultDevice::ISGetProperties(dev);
}

bool GPIOST4::ISNewNumber (const char *dev, const char *name, double values[], char *names[], int n)
{
    if(strcmp(dev,getDeviceName())==0)
    {
        if (!strcmp(name,GuideNSNP.name) || !strcmp(name,GuideWENP.name))
        {
            processGuiderProperties(name, values, names, n);
            return true;
        }
    }

    return INDI::DefaultDevice::ISNewNumber(dev, name, values, names, n);
}

bool GPIOST4::ISNewSwitch (const char *dev, const char *name, ISState *states, char *names[], int n)
{
    return INDI::DefaultDevice::ISNewSwitch(dev, name, states, names, n);
}

bool GPIOST4::ISNewText (const char *dev, const char *name, char *texts[], char *names[], int n)
{
    return INDI::DefaultDevice::ISNewText(dev, name, texts, names, n);
}

bool GPIOST4::ISSnoopDevice (XMLEle *root)
{
    return INDI::DefaultDevice::ISSnoopDevice(root);
}

void GPIOST4::debugTriggered(bool enable)
{
    driver->setDebug(enable);
}


 IPState GPIOST4::GuideNorth(uint32_t ms)
 {
     RemoveTimer(NSTimerID);
  
     driver->startPulse(GPIOST4_NORTH);
  
     NSDirection = GPIOST4_NORTH;
  
     LOG_DEBUG("Starting NORTH guide");
  
     NSPulseRequest = ms;
  
     NSGuideTS = std::chrono::system_clock::now();
  
     NSTimerID = IEAddTimer(ms, &GPIOST4::NSTimerHelper, this);
  
     return IPS_BUSY;
 }

IPState GPIOST4::GuideSouth(uint32_t ms)
 {
     RemoveTimer(NSTimerID);

     driver->startPulse(GPIOST4_SOUTH);

     NSDirection = GPIOST4_SOUTH;

     LOG_DEBUG("Starting SOUTH guide");

     NSPulseRequest = ms;

     NSGuideTS = std::chrono::system_clock::now();

     NSTimerID = IEAddTimer(ms, &GPIOST4::NSTimerHelper, this);

     return IPS_BUSY;
 }

 IPState GPIOST4::GuideEast(uint32_t ms)
 {
     RemoveTimer(WETimerID);

     driver->startPulse(GPIOST4_EAST);

     WEDirection = GPIOST4_EAST;

     LOG_DEBUG("Starting EAST guide");

     WEPulseRequest = ms;

     WEGuideTS = std::chrono::system_clock::now();

     WETimerID = IEAddTimer(ms, &GPIOST4::WETimerHelper, this);

     return IPS_BUSY;
 }

 IPState GPIOST4::GuideWest(uint32_t ms)
 {
     RemoveTimer(WETimerID);

     driver->startPulse(GPIOST4_WEST);

     WEDirection = GPIOST4_WEST;

     LOG_DEBUG("Starting WEST guide");

     WEPulseRequest = ms;

     WEGuideTS = std::chrono::system_clock::now();

     WETimerID = IEAddTimer(ms, &GPIOST4::WETimerHelper, this);

     return IPS_BUSY;
 }

 void GPIOST4::NSTimerHelper(void *context)
 {
     static_cast<GPIOST4*>(context)->NSTimerCallback();
 }

 void GPIOST4::WETimerHelper(void *context)
 {
     static_cast<GPIOST4*>(context)->WETimerCallback();
 }

 void GPIOST4::NSTimerCallback()
 {
     driver->stopPulse(NSDirection);
     GuideComplete(AXIS_DE);
 }

 void GPIOST4::WETimerCallback()
 {
     driver->stopPulse(WEDirection);
     GuideComplete(AXIS_RA);
 }
