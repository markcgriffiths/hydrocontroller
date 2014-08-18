/* Hydrocontroller - Used for controlling a hydroponics machine.
    Copyright (C) 2014  Mark Griffiths

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// HISTORY:
// Mark Griffiths- Original version (15/02/2014)

#ifndef HydroponicsUI_h
#define HydroponicsUI_h

#include "UI_Shared.h"
#include <HydroponicsEngine.h> //The Engine of the system.
#include "MinMaxScreen.h"
#include "pHScreen.h"
#include "ECScreen.h"
#include "AirScreen.h"
#include "WaterScreen.h"
#include "TouchCalibration.h"
#include "TimeScreen.h"
#include "PlugScreen.h"
#include "MainScreen.h"
#include "SetupScreen.h"
#include "AboutScreen.h"
#include "ECCalibrateScreen.h"
#include "PhCalibrateScreen.h"
#include "PlugSettingsScreen.h"
#include "PumpScreen.h"
#include "PumpDurationScreen.h"

//For keeping track of the screen.
const int MAIN_SCREEN = 1;
const int ABOUT_SCREEN = 2;
const int SETUP_SCREEN = 3;
const int PH_SCREEN = 4;
const int EC_SCREEN = 5;
const int AIR_SCREEN = 6;
const int WATER_SCREEN = 7;
const int PLUGS_SCREEN = 8;
const int CLOCK_SCREEN = 9;
const int CALLIBRATE_PH_SCREEN = 10;
const int CALLIBRATE_EC_SCREEN = 11;
const int CALLIBRATE_AIR_SCREEN = 12;
const int CALLIBRATE_WATER_SCREEN = 13;
const int PLUGS_SETTINGS_SCREEN = 14;
const int PUMP_SCREEN = 15;
const int PUMP_DURATION_SCREEN = 16;

//All the data from the HW sensors.


class HydroponicsUI
{
public:
	HydroponicsUI();
	void begin(HydroponicsEngine* aHydroEng);
	void refreshScreen();
	void CheckForScreenTouch();
	void updateInfoFromSensors( SENSOR& sensorInformation );
	void drawMainScreen();

private:
    void handleButtons( int aHandleWhichButton );
	
	private:
    PhScreen* ipHScreen;
    ECScreen* iECScreen;
    AirScreen* iAirScreen;
    WaterScreen* iWaterScreen;
    TouchCalibration* iCalibrateTouch;
    TimeScreen* iTimeScreen;
    PlugScreen* iPlugScreen;
    PlugSettingsScreen* iPlugSettingsScreen;
    MainScreen* iMainScreen;
    SetupScreen* iSetupScreen;
    AboutScreen* iAboutScreen;
    HydroponicsEngine* iHydroEng;
    ECCalibrateScreen* iECCalibrateScreen;
    PhCalibrateScreen* iPhCalibrateScreen;
    PumpScreen* iPumpScreen;
    PumpDurationScreen* iPumpDurationScreen;

	//Member Variables
    int whichScreen; //We always start on main.
    struct LIMITS limitInfo;
};

#endif
//
// END OF FILE
//
