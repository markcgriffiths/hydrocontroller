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



#include "ECScreen.h"
#include "ECCalibrateScreen.h"

//Global definition for whole of the UI
extern UTFT myUTFT;
extern UTouch myTouch;
extern UTFT_Buttons myButtons;


ECScreen::ECScreen( )
{
}

void ECScreen::drawScreen(int aWhichScreen )
{
	iPreviousScreen = aWhichScreen;
	drawMinMaxScreen( max, min, units );
	//Setup header last as drawMinMaxScreen will clear screen.
	drawHeader( "EC Control",  "Max and min EC"  );

	drawSettingsButton();

}

int ECScreen::getMax( )
{
	return max;
}

int ECScreen::getMin( )
{
	return min;
}

void ECScreen::refreshScreen()
{
	refreshMinMaxScreen( max, min, units );
}

void ECScreen::handleExitScreen()
{
	//Save the values for when device switched off.
	EEPROM.write(EEPROM_ECMAXTEMP_ADDRESS, max);
	EEPROM.write(EEPROM_ECMINTEMP_ADDRESS, min);
}

int ECScreen::handleScreen()
{
	int pressed_button = myButtons.checkButtons();
	if(settingsButton == pressed_button)
	{
		return EC_CALIBRATION_BUTTON;
	}
	if (pressed_button==back2MainButton)
			{
			handleExitScreen();

			if (iPreviousScreen == 3/*SETUP_SCREEN*/ || iPreviousScreen == /*CALLIBRATE_EC_SCREEN */11 )
					return BACK2SETUP_BUTTON;

			return BACK2MAIN_BUTTON;
			}
		if (pressed_button==plusMaxButton)
			{
			max++;
			}
		if (pressed_button==minusMaxButton)
			{
			max--;
			}
		if (pressed_button==plusMinButton)
			{
			min++;
			}
		if (pressed_button==minusMinButton)
			{
			min--;
			}
		if (pressed_button==unitsButton)
			{
			//Go to the setup screen
			if( units >= 2)
				units = 0;
			else
				units++;
			}

		refreshScreen();
		return NONE;

}

void ECScreen::drawSettingsButton()
{
	settingsButton = myButtons.addButton( 160, 170, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	myButtons.drawButton(settingsButton);

	//Draw Button Text
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Settings", 160, 185 );

}



//
// END OF FILE
//
