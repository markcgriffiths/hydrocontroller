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

#ifndef SETUPSCREEN_H
#define SETUPSCREEN_H

#include <string.h>
#include <UI_Shared.h>
#include "Screen.h"

class SetupScreen : public Screen
{
public:
	//Functions
	SetupScreen();
	void begin();
	void drawScreen();
	int handleScreen();
	void refreshScreen();
	void handleExitScreen();
	void resetButtons();


};
#endif
//
// END OF FILE
//
