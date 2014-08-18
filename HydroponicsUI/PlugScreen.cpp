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



#include "PlugScreen.h"
#include <string.h>

//Global definition for whole of the UI
extern UTFT myUTFT;
extern UTouch myTouch;
extern UTFT_Buttons myButtons;

extern boolean isDataReceived;
extern SimpleTimer simpleTimer;
int timerNumber =0;
int exitButton = -10;
boolean isScreenActive = false;
whatDevice plugDevice = {false,false,false};




PlugScreen::PlugScreen()
{
}

void PlugScreen::begin()
{
	Serial.begin(115200);
}



void PlugScreen::refreshScreen()
{

}


void PlugScreen::drawScreen(  )
{
	//Clear What was there.
	 myUTFT.clrScr();

	 resetButtons();
	 myUTFT.setBackColor (0, 0, 0);
	 myUTFT.print("Plug Setup", CENTER, 1);
	 myUTFT.setBackColor(0, 0, 255);

	 //Draw Lights button
	 lightsButton = myButtons.addButton( 10, 54, 145, 53, mainButton);
	 myButtons.drawButton(lightsButton);
	 myUTFT.print("Lights", 35, 71);

	 heaterButton = myButtons.addButton( 10, 110, 145, 53, mainButton);
	 myButtons.drawButton(heaterButton);
	 myUTFT.print("Heater", 35, 135);


	 pumpButton = myButtons.addButton( 160, 54, 145, 53, mainButton);
	 myButtons.drawButton(pumpButton);
	 myUTFT.print("Pump", 190, 71);

	 settingsButton = myButtons.addButton( 160, 110, 145, 53, mainButton);
	 myButtons.drawButton(settingsButton);
	 myUTFT.print("Override", 175, 135);

	 //Draw back button
	 back2SetupButton = myButtons.addButton( 10, 170, 145, 53, mainButton);
	 myButtons.drawButton(back2SetupButton);
	 myUTFT.print("Back", 55, 195);

}

void PlugScreen::waitForReceiver(NewRemoteCode receivedCode)
{
	//Clear What was there.
	myUTFT.setFont(SmallFont);
	myUTFT.clrScr();
	myUTFT.setColor(255, 0, 0);
	myUTFT.fillRect(0, 0, 220, 13);
	myUTFT.setColor(255, 255, 255);
	myUTFT.setBackColor(255, 0, 0);
	myUTFT.drawLine(0, 14, 320, 14);
	myUTFT.print("ID Set", CENTER, 1);
	myUTFT.setBackColor(0, 0, 0);

	setAddresstoEEPROM(receivedCode.address);
	if(plugDevice.lights)
	{
		EEPROM.write(EEPROM_LEARNED_LIGHTS_UNIT,receivedCode.unit);
		plugDevice.lights = false;
	}
	if(plugDevice.pump)
	{
		EEPROM.write(EEPROM_LEARNED_PUMP_UNIT,receivedCode.unit);
		plugDevice.pump = false;
	}
	if(plugDevice.heater)
	{
		EEPROM.write(EEPROM_LEARNED_HEATER_UNIT,receivedCode.unit);
		plugDevice.heater = false;
	}

	//unsigned long add = getAddress();
	String sAddress(receivedCode.address);
	String slightID(receivedCode.unit);

	myUTFT.print("The unit has been set to unit ", LEFT, 30);
	myUTFT.print(slightID, LEFT, 42);
	myUTFT.print("The Transmitting Address is", LEFT, 54);
	myUTFT.print(sAddress, LEFT, 66);
	myUTFT.print("You can now use the plug", LEFT, 78);
	myUTFT.print("**********************************", LEFT, 90);

	myButtons.deleteAllButtons();
	exitButton = -1;

	exitButton = myButtons.addButton( 10, 170, 145, 53, mainButton);
	myButtons.drawButton(exitButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Back", 55, 195);
	myUTFT.setFont(BigFont);

}

void PlugScreen::setAddresstoEEPROM(unsigned long ctr)
{
	//truncate upper part and write lower part into EEPROM
	EEPROM_writeint(EEPROM_LEARNED_PLUGS_ADDRESS_A+2, word(ctr));
	//shift upper part down
	ctr = ctr >> 16;
	//truncate and write
	EEPROM_writeint(EEPROM_LEARNED_PLUGS_ADDRESS_A, word(ctr));
}

//write word to EEPROM
  void PlugScreen::EEPROM_writeint(int address, int value)
  {
  EEPROM.write(address,highByte(value));
  EEPROM.write(address+1 ,lowByte(value));
}


void PlugScreen::dataReceived()
{

}

int PlugScreen::handleScreen()
{
	//return handleScreenButtons(myButtons.checkButtons());
	int pressed_button = myButtons.checkButtons();

	if (pressed_button==back2SetupButton)
		{
		NewRemoteReceiver::deinit();
		return BACK2SETUP_BUTTON;
		}
	if (pressed_button==lightsButton)
		{
		resetButtons();
		drawPrepareScreen();
		plugDevice.lights = true;
		NewRemoteReceiver::init(RECEIVER_PIN, 1, waitForReceiver);
		Serial.println("PlugScreen::handlePlugScreen SET TIMER \n");
		}
	if (pressed_button==heaterButton)
		{
		resetButtons();
		drawPrepareScreen();
		plugDevice.heater = true;
		NewRemoteReceiver::init(RECEIVER_PIN, 1, waitForReceiver);
		}
	if (pressed_button==pumpButton)
		{
		resetButtons();
		drawPrepareScreen();
		plugDevice.pump = true;
		NewRemoteReceiver::init(RECEIVER_PIN, 1, waitForReceiver);
		}
	if (pressed_button==settingsButton)
		{
			NewRemoteReceiver::deinit();
			return PLUGS_SETTINGS_BUTTON;
		}
	if (pressed_button==exitButton)
		{
			NewRemoteReceiver::deinit();
			drawScreen();
		}

	Serial.println("PlugScreen::handlePlugScreen end \n");
}

void  PlugScreen::drawPrepareScreen()
{
	Serial.println("PlugScreen::handlePlugScreen SET LIGHTS ID  \n");
	myUTFT.setFont(SmallFont);
	myUTFT.clrScr();
	myUTFT.setColor(255, 0, 0);
	myUTFT.fillRect(0, 0, 220, 13);
	myUTFT.setColor(255, 255, 255);
	myUTFT.setBackColor(255, 0, 0);
	myUTFT.drawLine(0, 14, 320, 14);
	myUTFT.print("Plug Setting", CENTER, 1);
	myUTFT.setBackColor(0, 0, 0);

	myUTFT.print("Use the remote that came with your", LEFT, 30);
	myUTFT.print("plugs to set which unit and address it", LEFT, 42);
	myUTFT.print("uses.Just point the remote at the screen", LEFT, 54);
	myUTFT.print("and press a button for the unit you want", LEFT, 66);
	myUTFT.print("to use for the lights", LEFT, 78);
	myUTFT.print("", LEFT, 90);
	myUTFT.print("Please press a unit for the lights", LEFT, 102);
	myUTFT.print("", LEFT, 114);

	exitButton = myButtons.addButton( 10, 170, 145, 53, mainButton);
	myButtons.drawButton(exitButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Cancel", 55, 195);
	myUTFT.setFont(BigFont);
}



void  PlugScreen::resetButtons() {

	myButtons.deleteAllButtons();
	resetButtonIDs();
}


//
// END OF FILE
//
