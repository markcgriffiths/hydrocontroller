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



#include "MainScreen.h"
//myUTFT myUTFT(ITDB32S, 38,39,40,41);
//UmyTouch mymyTouch(6,5,4,3,2);
//myUTFT_myButtons  myButtons(&myUTFT, &myTouch);

//Global definition for whole of the UI
extern UTFT myUTFT;
extern UTouch myTouch;
extern UTFT_Buttons myButtons;


MainScreen::MainScreen(){
}

void MainScreen::begin() {
	// Initial setup
	myUTFT.InitLCD();
	myUTFT.clrScr();

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	myUTFT.setFont(BigFont);
	myUTFT.setBackColor(0, 0, 255);
	myButtons.setTextFont(BigFont);

	resetButtons();
}

void MainScreen::drawScreen()
{
	Serial.println("MainScreen::drawScreen start  \n");

	 myUTFT.clrScr();
	 myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLACK);
	 resetButtons();

	 //Indicate that we have changed screens.
	 //whichScreen = MAIN_SCREEN;

	 //Draw a black background
	 myUTFT.setBackColor (VGA_BLACK);
	 myUTFT.setColor(VGA_WHITE);
	  //Draw  Ph button

	 if( ( limitsInfo.maxPHLevel > sensorUIInfo.phLevel ) &&
	 	  	      ( limitsInfo.minPHLevel < sensorUIInfo.phLevel ) )
	 {
		 pHButton = myButtons.addButton( 10, 5, 64, 64, green_light);
	 }
	 else
	 {
	 	pHButton = myButtons.addButton( 10, 5, 64, 64, red_light);
	 }



	  myUTFT.print("pH", 20, 80);

	  //Draw pH value
	  myUTFT.printNumF(sensorUIInfo.phLevel, 1, 20, 120);

	  //Draw  EC button

	  if( ( limitsInfo.maxECLevel > sensorUIInfo.eCLevel ) &&
	  	      ( limitsInfo.minECLevel < sensorUIInfo.eCLevel ) )
	  {
		  ECButton = myButtons.addButton( 80, 5, 64, 64, green_light);
	  }
	  else
	 {
	 	 ECButton = myButtons.addButton( 80, 5, 64, 64, red_light);
	 }

	  myUTFT.print("EC", 90, 80);

	  //Draw EC value
	  //myUTFT.printNumF(sensorUIInfo.eCLevel, 1, 90, 120);
	  myUTFT.printNumI(sensorUIInfo.eCLevel, 90, 120,2);

	  //Draw  Air Temp button
	  if( ( limitsInfo.maxAirTemp > sensorUIInfo.airTemp ) &&
	      ( limitsInfo.minAirTemp < sensorUIInfo.airTemp ) )
	  {
	  	  airButton = myButtons.addButton( 155, 5, 64, 64, green_light);
	  }
	  else
	  {
	  	airButton = myButtons.addButton( 155, 5, 64, 64, red_light);
	  }

	 myUTFT.print("Air", 150, 80);

	  //Draw Air value
	  myUTFT.printNumI(sensorUIInfo.airTemp, 170, 120, 2);

	  //Draw  Water Temp button
	  if( ( limitsInfo.maxWaterTemp > sensorUIInfo.waterTemp ) &&
	  	      ( limitsInfo.minWaterTemp < sensorUIInfo.waterTemp ) )
	    {
	    	  waterButton = myButtons.addButton( 230, 5, 64, 64, green_light);
	    }
	    else
	    {
	    	 waterButton = myButtons.addButton( 230, 5, 64, 64, red_light);
	    }

	  myUTFT.print("Water", 220, 80);
	  //Draw water value
	  myUTFT.printNumI(sensorUIInfo.waterTemp, 240, 120, 2);



	  //Draw Setup button
	  back2SetupButton = myButtons.addButton( 10, 168, 145, 53, mainButton);


	  myUTFT.setBackColor (VGA_BLACK);
	  myUTFT.drawRoundRect (160, 170, 300, 220);
	  myUTFT.printNumI(sensorUIInfo.hour, 190, 187,2,'0');
	  myUTFT.print(":", 220, 187);
	  myUTFT.printNumI(sensorUIInfo.minute, 235, 187,2,'0');

	  myButtons.drawButtons();
	  //Draw Button Text
	  myUTFT.setBackColor (161,190,237);
	  myUTFT.print("Setup", 40, 185);

	Serial.println("MainScreen::drawScreen end  \n");
}

void MainScreen::refreshScreen()
{
	myUTFT.setBackColor (VGA_BLACK);
    myUTFT.setColor(VGA_WHITE);
	//Draw pH value
	myUTFT.printNumF(sensorUIInfo.phLevel, 1, 20, 120);
	//Draw EC value
	myUTFT.printNumI(sensorUIInfo.eCLevel, 90, 120,2);
	//Draw Air value
	myUTFT.printNumI(sensorUIInfo.airTemp, 170, 120, 2);
	//Draw water value
	myUTFT.printNumI(sensorUIInfo.waterTemp, 240, 120, 2);
	//Draw Clock
	myUTFT.printNumI(sensorUIInfo.hour, 190, 187,2,'0');
	myUTFT.printNumI(sensorUIInfo.minute, 235, 187,2,'0');
}

void MainScreen::handleExitScreen()
{
	//Save the values for when device switched off.
	//EEPROM.write(EEPROM_AIRMAXTEMP_ADDRESS, maxAirTemp);
	//EEPROM.write(EEPROM_AIRMINTEMP_ADDRESS, minAirTemp);
	//EEPROM.write(EEPROM_AIRTEMPUNITS_ADDRESS, airTempUnits);

}

int MainScreen::handleScreen()
{
	 return handleScreenButtons(myButtons.checkButtons());
}

//This stops two myButtons having the same number.
void MainScreen::resetButtons() {

	myButtons.deleteAllButtons();
	resetButtonIDs();


}

void MainScreen::updateInfoFromSensors( SENSOR& sensorInformation )
{
 sensorUIInfo.phLevel = sensorInformation.phLevel;
 sensorUIInfo.eCLevel = sensorInformation.eCLevel;
 sensorUIInfo.airTemp = sensorInformation.airTemp;
 sensorUIInfo.waterTemp =  sensorInformation.waterTemp;
 sensorUIInfo.hour = sensorInformation.hour;
 sensorUIInfo.minute = sensorInformation.minute;
 sensorUIInfo.lightLevel = sensorInformation.lightLevel;
}

void MainScreen::setLimitValues( LIMITS& aLimitInfo ){
	limitsInfo.maxAirTemp = aLimitInfo.maxAirTemp;
	limitsInfo.minAirTemp = aLimitInfo.minAirTemp;
	limitsInfo.maxWaterTemp = aLimitInfo.maxWaterTemp;
	limitsInfo.minWaterTemp = aLimitInfo.minWaterTemp;
	limitsInfo.maxECLevel = aLimitInfo.maxECLevel;
	limitsInfo.minECLevel = aLimitInfo.minECLevel;
	limitsInfo.maxPHLevel = aLimitInfo.maxPHLevel;
	limitsInfo.minPHLevel = aLimitInfo.minPHLevel;
}



//
// END OF FILE
//
