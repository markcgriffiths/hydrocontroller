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

#ifndef PLUGSCREEN_H
#define PLUGSCREEN_H

#include "UI_Shared.h"
#include "Screen.h"
#include <SimpleTimer.h>

class HydroponicsEngine;

struct whatDevice
{
	boolean lights;
	boolean heater;
	boolean pump;
};

class PlugScreen : public Screen
{
public:
	//Functions
	PlugScreen();
	void begin();
	void drawScreen();
	void refreshScreen();
	static void waitForReceiver(NewRemoteCode receivedCode);
	int handleScreen();
	void dataReceived();
	static void setAddresstoEEPROM(unsigned long ctr);
	void drawPrepareScreen();
	static void EEPROM_writeint(int address, int value);



private:
	void resetButtons();

	private:
    bool drawUnits;
    boolean codeLearned;
    int lightID, address, iTimerNumber, settingsButton;
    
};

#endif
//
// END OF FILE
//
