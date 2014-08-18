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



#include "PlugSettingsScreen.h"
#include <string.h>

//Global definition for whole of the UI
extern UTFT myUTFT;
extern UTouch myTouch;
extern UTFT_Buttons myButtons;

PlugSettingsScreen::PlugSettingsScreen()
{
}

void PlugSettingsScreen::begin()
{
	Serial.begin(115200);
}



void PlugSettingsScreen::refreshScreen()
{

}


void PlugSettingsScreen::drawScreen(  )
{
	//Clear What was there.
	 myUTFT.clrScr();
	 resetButtons();
	 myUTFT.setBackColor (0, 0, 0);
	 myUTFT.print("Plug Override", CENTER, 1);
	 myUTFT.setBackColor(0, 0, 255);

	 //Draw Lights button
	 lightsChange = myButtons.addButton( 10, 54, 145, 53, mainButton);
	 myButtons.drawButton(lightsChange);
	 myUTFT.print("Lights", 35, 71);

	 heaterChange = myButtons.addButton( 10, 110, 145, 53, mainButton);
	 myButtons.drawButton(heaterChange);
	 myUTFT.print("Heater", 35, 135);


	 pumpChange = myButtons.addButton( 160, 54, 145, 53, mainButton);
	 myButtons.drawButton(pumpChange);
	 myUTFT.print("Pump", 190, 71);

	 //Draw back button
	 back2SetupButton = myButtons.addButton( 10, 170, 145, 53, mainButton);
	 myButtons.drawButton(back2SetupButton);
	 myUTFT.print("Back", 55, 195);


}

int PlugSettingsScreen::handleScreen()
{
	//return handleScreenButtons(myButtons.checkButtons());
	int pressed_button = myButtons.checkButtons();

	if (pressed_button==back2SetupButton)
		{
		return PLUGS_BUTTON;
		}
	if (pressed_button==lightsChange)
		{
		bool lightsBool= EEPROM.read(EEPROM_LIGHTS_UNIT_STATE);
		iEngine->SwitchLightsValue(!lightsBool);
		}
	if (pressed_button==heaterChange)
		{
		bool heaterBool= EEPROM.read(EEPROM_LIGHTS_UNIT_STATE);
		iEngine->SwitchHeaterValue(!heaterBool);
		}
	if (pressed_button==pumpChange)
		{
		bool pumpBool= EEPROM.read(EEPROM_PUMP_UNIT_STATE);
		iEngine->SwitchPumpValue(!pumpBool);
		}

	Serial.println("PlugSettingsScreen::handlePlugSettingsScreen end \n");
}



void  PlugSettingsScreen::resetButtons()
{
	myButtons.deleteAllButtons();
	resetButtonIDs();
	lightsChange = -10;
	heaterChange = -10;
	pumpChange = -10;
}


//
// END OF FILE
//
