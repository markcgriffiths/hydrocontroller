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



#include "AboutScreen.h"

//Global definition for whole of the UI
extern UTFT myUTFT;
extern UTouch myTouch;
extern UTFT_Buttons myButtons;

AboutScreen::AboutScreen(){
}

void AboutScreen::begin() {


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

void AboutScreen::drawScreen()
{
	//Clear What was there.
	 myUTFT.clrScr();

	 resetButtons();
	 myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLACK);
	 //Indicate that we have changed screens.


	  myUTFT.setBackColor (VGA_BLACK);
	  myUTFT.print("Hydroponics Control", CENTER, 1);

	  myUTFT.print("SW Version 0.01", CENTER, 30);
	  myUTFT.print("Developed by", CENTER, 54);
	  myUTFT.print("Mark Griffiths", CENTER, 78);
	  myUTFT.print("Open Sourced", CENTER, 102);
	  myUTFT.print("humielectronics.com", CENTER, 126);

	  //Draw back button
	  back2SetupButton = myButtons.addButton( 10, 168, 145, 53, mainButton);
	  myButtons.drawButton(back2SetupButton);
	  myUTFT.setBackColor (161,190,237);
	  myUTFT.print("Back", 45, 185);
}

void AboutScreen::handleExitScreen()
{
	//Save the values for when device switched off.
	//EEPROM.write(EEPROM_AIRMAXTEMP_ADDRESS, maxAirTemp);
	//EEPROM.write(EEPROM_AIRMINTEMP_ADDRESS, minAirTemp);
	//EEPROM.write(EEPROM_AIRTEMPUNITS_ADDRESS, airTempUnits);

}

int AboutScreen::handleScreen()
{
	 return handleScreenButtons(myButtons.checkButtons());
}

//This stops two myButtons having the same number.
void AboutScreen::resetButtons() {

	myButtons.deleteAllButtons();
	resetButtonIDs();

}



//
// END OF FILE
//
