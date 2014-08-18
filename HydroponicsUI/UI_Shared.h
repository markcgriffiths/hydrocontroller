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

#ifndef UI_SHARED_H
#define UI_SHARED_H
#include <Arduino.h>
#include <EEPROM.h>

#include <UTFT.h> // Graphics Library
#include <UTouch.h> //Touch Screen Library

#include <HydroponicsEngine.h> //The Engine of the system.

// Declare which fonts we will be using
extern uint8_t BigFont[];
extern uint8_t SmallFont[];

#define HYDROPONICSUILIB_VERSION "0.0.1"

#include "../UTFT/UTFT.h" // Graphics Library
#include "../UTouch/UTouch.h" //Touch Screen Library
#include "../UTFT_Buttons/UTFT_Buttons.h"

#define imagedatatype  unsigned int

// Declare which bitmaps we will be using
extern imagedatatype minusButton[]; //48x48
extern imagedatatype plusButton[]; //48x48
extern imagedatatype green_light[];//64x64
extern imagedatatype red_light[];//64x64
extern imagedatatype mainButton[];//145x53

//Define Button sizes
const int MAIN_BUTTON_X = 145;
const int MAIN_BUTTON_Y = 53;
const int PLUS_BUTTON_X = 48;
const int PLUS_BUTTON_Y = 48;

//For keeping track of the buttons.
const int NONE						= 0;
const int AIR_BUTTON 				= 1;
const int PH_BUTTON 				= 2;
const int EC_BUTTON 				= 3;
const int WATER_BUTTON 				= 4;
const int PUMP_BUTTON 				= 5;
const int ABOUT_BUTTON 				= 6;
const int BACK2SETUP_BUTTON 		= 7;
const int CLOCK_BUTTON 				= 8;
const int BACK2MAIN_BUTTON 			= 9;
const int LAMPS_BUTTON 				= 10;
const int PLUGS_BUTTON 				= 11;
const int LIGHTSWITCH_BUTTON 		= 12;
const int EC_CALIBRATION_BUTTON 	= 13;
const int PH_CALIBRATION_BUTTON 	= 14;
const int PLUGS_SETTINGS_BUTTON 	= 15;
const int PUMP_TIMER_BUTTON 		= 16;
const int PUMP_DURATION_BUTTON 		= 17;

//EEPROM address, 4kb so 4*1024 spaces to save data.
const int EEPROM_EMPTY_ADDRESS = 0;
const int EEPROM_AIRMAXTEMP_ADDRESS = 1;
const int EEPROM_AIRMINTEMP_ADDRESS = 2;
const int EEPROM_AIRTEMPUNITS_ADDRESS = 3;  //0 = C, 1= F, 2= K.

const int EEPROM_WATERMAXTEMP_ADDRESS = 4;
const int EEPROM_WATERMINTEMP_ADDRESS = 5;
const int EEPROM_WATERTEMPUNITS_ADDRESS = 6;  //0 = C, 1= F, 2= K.

const int EEPROM_PHMAXTEMP_ADDRESS = 7;
const int EEPROM_PHMINTEMP_ADDRESS = 8;

const int EEPROM_ECMAXTEMP_ADDRESS = 9;
const int EEPROM_ECMINTEMP_ADDRESS = 10;

const unsigned int EEPROM_LEARNED_PLUGS_ADDRESS_A = 11;  //LSB
const unsigned int EEPROM_LEARNED_PLUGS_ADDRESS_B = 12;
const unsigned int EEPROM_LEARNED_PLUGS_ADDRESS_C = 13;
const unsigned int EEPROM_LEARNED_PLUGS_ADDRESS_D = 14; //MSB
const unsigned int EEPROM_LEARNED_PLUGS_ADDRESS_E = 15; //Counter

const unsigned short EEPROM_LEARNED_LIGHTS_UNIT = 16;
const unsigned short EEPROM_LEARNED_PUMP_UNIT = 17;
const unsigned short EEPROM_LEARNED_HEATER_UNIT = 18;

const unsigned short EEPROM_LIGHTS_UNIT_STATE = 19;
const unsigned short EEPROM_PUMP_UNIT_STATE = 20;
const unsigned short EEPROM_HEATER_UNIT_STATE = 21;

const unsigned short EEPROM_PUMP_TIMER_HOUR = 22;
const unsigned short EEPROM_PUMP_TIMER_MIN = 23;

const unsigned short EEPROM_PUMP_DURATION_TIMER_HOUR = 24;
const unsigned short EEPROM_PUMP_DURATION_TIMER_MIN = 25;

struct LIMITS{
	int maxAirTemp;
	int minAirTemp;
	int airUnits;
	int maxWaterTemp;
	int minWaterTemp;
	int waterUnits;
	int maxECLevel;
	int minECLevel;
	float maxPHLevel;
	float minPHLevel;
};






#endif //UI_SHARED_H

//
// END OF FILE
//
