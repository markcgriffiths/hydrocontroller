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


PlugScreen::PlugScreen(){
}

void PlugScreen::begin() {

	Serial.println("PlugScreen::begin() start  \n");
	// Initial setup
	myUTFT.InitLCD();
	myUTFT.clrScr();

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	myUTFT.setFont(BigFont);
	myUTFT.setBackColor(0, 0, 255);
	myButtons.setTextFont(BigFont);

	Serial.begin(115200);

    Serial.println("PlugScreen::begin() end  \n");

 	//Read values stored on EEPROM.
 	//maxAirTemp = EEPROM.read(EEPROM_AIRTEMP_ADDRESS);


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
	 myUTFT.print("Plug Control", CENTER, 1);
	 myUTFT.print("Connect plug After", CENTER, 30);
	 myUTFT.setBackColor (161,190,237);

	  //Draw Lights button
	  lightsButton = myButtons.addButton( 10, 64, 145, 53, mainButton);
	  myButtons.drawButton(lightsButton);
	  myUTFT.print("Lights", 35, 81);

	  //lightSwitchButton = myButtons.addButton( 10, 100, 145, 53, mainButton);
	  //myButtons.drawButton(lightSwitchButton);
	  //myUTFT.print("On", 35, 120);

	  //Draw  Pump Button
	  pumpButton = myButtons.addButton( 160, 64, 145, 53, mainButton);
	  myButtons.drawButton(pumpButton);
	  myUTFT.print("Pump", 190, 81);
	  
	  heaterButton = myButtons.addButton( 160, 120, 145, 53, mainButton);
	  myButtons.drawButton(heaterButton);
	  myUTFT.print("Heater", 185, 130);

	  
	  //Draw back button
	  back2SetupButton = myButtons.addButton( 10, 170, 145, 53, mainButton);
	  myButtons.drawButton(back2SetupButton);
	  myUTFT.print("Back", 55, 200);
}

int PlugScreen::handleScreen(){

	Serial.println("PlugScreen::handlePlugScreen start  \n");
	return handleScreenButtons(myButtons.checkButtons());
	Serial.println("PlugScreen::handlePlugScreen end \n");
}

void  PlugScreen::resetButtons() {

	myButtons.deleteAllButtons();
	resetButtonIDs();
}

//
// END OF FILE
//
