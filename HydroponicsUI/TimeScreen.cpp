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


#include "TimeScreen.h"



TimeScreen::TimeScreen(){
}

void TimeScreen::drawScreen()
{
	Serial.println("TimeScreen::drawScreen start  \n");
	iEngine->GetTime(max, min);
	drawMinMaxScreen( max, min, units );
	//Setup header last as drawMinMaxScreen will clear screen.
	drawHeader( "Time Control",  "Set Hours & Mins"  );
	Serial.println("TimeScreen::drawScreen end  \n");
}

void TimeScreen::refreshScreen()
{

	if( max > 24 )
		max = 0;

	if( max < 0 )
		max = 24;

	if ( min > 60 )
		min = 0;

	if( min < 0 )
		min = 60;

	refreshMinMaxScreen( max, min, units );
}

void TimeScreen::handleExitScreen()
{
	iEngine->SetTime(max, min);
}

int TimeScreen::handleScreen()
{
	Serial.println("TimeScreen::handleScreen start  \n");


	int ret = handleMinMaxScreen( max, min, units );

	if(ret ==BACK2MAIN_BUTTON)
		ret = BACK2SETUP_BUTTON;

	return ret;
}



//
// END OF FILE
//
