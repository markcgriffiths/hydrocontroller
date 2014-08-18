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



#include "SetupScreen.h"

//Global definition for whole of the UI
extern UTFT myUTFT;
extern UTouch myTouch;
extern UTFT_Buttons myButtons;


SetupScreen::SetupScreen(){
}

void SetupScreen::begin() {


	// Initial setup
	myUTFT.InitLCD();
	myUTFT.clrScr();

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	myUTFT.setFont(BigFont);
	myUTFT.setBackColor(0, 0, 255);
	myButtons.setTextFont(BigFont);

	resetButtons();

	Serial.begin(115200);


}

void SetupScreen::drawScreen()
{
	//Clear What was there.
	myUTFT.clrScr();
	myUTFT.setBackColor(0, 0, 255);
	 //Indicate that we have changed screens.
	 //whichScreen = SETUP_SCREEN;
	 resetButtons();
	 myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLUE);
	 //myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_RED, VGA_BLUE);

	  //Draw pH button
	  pHButton = myButtons.addButton( 10, 10, 145, 53, mainButton);

	  //Draw  EC Button
	  ECButton = myButtons.addButton( 160, 10, 145, 53, mainButton);


	   //Draw Air button
	  airButton = myButtons.addButton( 10, 64, 145, 53, mainButton);

	  //Draw  Water Button
	  waterButton = myButtons.addButton( 160, 64, 145, 53, mainButton);


	  //Draw Plugs button
	  plugsButton = myButtons.addButton( 10, 116, 145, 53, mainButton);

	  //Draw  About Button
	  pumpTimerButton = myButtons.addButton( 160, 116, 145, 53, mainButton);


	  //Draw Back button
	  back2MainButton = myButtons.addButton( 10, 170, 145, 53, mainButton);

	  //Draw  Clock Button
	  clockButton = myButtons.addButton( 160, 170, 145, 53, mainButton);

	  myButtons.drawButtons();

	  //Add text to buttons.
	  myUTFT.setBackColor (161,190,237);
	  myUTFT.print("pH", 55, 30);
	  myUTFT.print("EC", 200, 30);
	  myUTFT.print("Air", 55, 90);
	  myUTFT.print("Water", 200, 90);
	  myUTFT.print("Plugs", 55, 146);
	  myUTFT.print("Pump", 200, 146);
	  myUTFT.print("Back", 55, 200);
	  myUTFT.print("Clock", 200, 200);
}

void SetupScreen::handleExitScreen()
{
	//Save the values for when device switched off.
	//EEPROM.write(EEPROM_AIRMAXTEMP_ADDRESS, maxAirTemp);
	//EEPROM.write(EEPROM_AIRMINTEMP_ADDRESS, minAirTemp);
	//EEPROM.write(EEPROM_AIRTEMPUNITS_ADDRESS, airTempUnits);

}

int SetupScreen::handleScreen()
{
	 return handleScreenButtons(myButtons.checkButtons());
}

//This stops two myButtons having the same number.
void SetupScreen::resetButtons() {

	myButtons.deleteAllButtons();
	resetButtonIDs();


}



//
// END OF FILE
//
