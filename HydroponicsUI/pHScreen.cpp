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



#include "pHScreen.h"

//Global definition for whole of the UI
extern UTFT myUTFT;
extern UTouch myTouch;
extern UTFT_Buttons myButtons;


PhScreen::PhScreen(){
}

void PhScreen::drawScreen( int aWhichScreen )
{
	iPreviousScreen = aWhichScreen;
	Serial.println("PhScreen::drawScreen start  \n");
	drawMinMaxScreen( floatMax, floatMin, units );
	//Setup header last as drawMinMaxScreen will clear screen.
	drawHeader( "pH Control",  "Max and min pH"  );
	Serial.println("PhScreen::drawScreen end  \n");
	
	drawSettingsButton();
}

int PhScreen::getMax( )
{
	return max;
}

int PhScreen::getMin( )
{
	return min;
}

void PhScreen::refreshScreen()
{
	refreshMinMaxScreen( floatMax, floatMin, units );
}

void PhScreen::handleExitScreen()
{
	//Save the values for when device switched off.
	EEPROMEx.writeFloat(EEPROM_PHMAXTEMP_ADDRESS, floatMax);
	EEPROMEx.writeFloat(EEPROM_PHMINTEMP_ADDRESS, floatMin);
}

int PhScreen::handleScreen()
{
	Serial.println("PhScreen::handleScreen start  \n");
	int pressed_button = myButtons.checkButtons();
		if(settingsButton == pressed_button)
		{
			return PH_CALIBRATION_BUTTON;
		}
		if (pressed_button==back2MainButton)
				{
				handleExitScreen();

				if (iPreviousScreen == 3/*SETUP_SCREEN*/ || iPreviousScreen == /*CALLIBRATE_PH_SCREEN */10 )
						return BACK2SETUP_BUTTON;

				return BACK2MAIN_BUTTON;
				}
			if (pressed_button==plusMaxButton)
				{
				floatMax++;
				}
			if (pressed_button==minusMaxButton)
				{
				floatMax--;
				}
			if (pressed_button==plusMinButton)
				{
				floatMin++;
				}
			if (pressed_button==minusMinButton)
				{
				floatMin--;
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

	//return handleMinMaxScreen( floatMax, floatMin, units );
}

void PhScreen::drawSettingsButton()
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
