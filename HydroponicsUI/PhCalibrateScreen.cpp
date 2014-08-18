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


#include "PhCalibrateScreen.h"

extern UTFT myUTFT;
extern UTouch myTouch;
extern UTFT_Buttons myButtons;

boolean waiting_for_PH_info;
String PHsensorstring;
String PHinputstring;

boolean waiting_for_PH_single_reading;
boolean PHsensor_stringcomplete;
String PHLevelString;

extern String waterTempString;

extern SimpleTimer simpleTimer;
static int iCounter = 0;
static int max_seconds_for_PH_calibration = 12;
static int iSeconds = max_seconds_for_PH_calibration;
static int iTimesToUpdatePHScreen = (60);

PhCalibrateScreen::PhCalibrateScreen(){
}

void PhCalibrateScreen::waitForTouch()
{
  while (myTouch.dataAvailable() == true)
  {
	  myTouch.read();
  }
  while (myTouch.dataAvailable() == false) {}
  while (myTouch.dataAvailable() == true)
  {
	  myTouch.read();
  }
}

void PhCalibrateScreen::SwaitForTouch()
{
  while (myTouch.dataAvailable() == true)
  {
	  myTouch.read();
  }
  while (myTouch.dataAvailable() == false) {}
  while (myTouch.dataAvailable() == true)
  {
	  myTouch.read();
  }
}

void PhCalibrateScreen::updateWaitScreen()
{
	iSeconds = iSeconds - 3;
	String thisString = String(iSeconds);
	myUTFT.clrScr();
	myUTFT.print("Calibration in Progress", CENTER, 30);
	myUTFT.print("Seconds left until ready " + thisString, CENTER, 42);


	switch(iCounter){

	case 0:
		Serial.println("PRINT 0");
		myUTFT.print("************************************", CENTER, 114);
		iCounter = 1;
		break;
	case 1:
		Serial.println("PRINT 1");
		myUTFT.print("******************", CENTER, 114);
		iCounter = 2;
		break;
	case 2:
		Serial.println("PRINT 2");
		myUTFT.print("*****", CENTER, 114);
		iCounter = 0;
		break;

	}

   if(iSeconds == 0){
	   iSeconds = max_seconds_for_PH_calibration;
	   //Move to next stage.
	   pHFourCalibration();
   }
}

void PhCalibrateScreen::updatePH7WaitScreen()
{
	iSeconds = iSeconds - 3;
	String thisString = String(iSeconds);
	myUTFT.clrScr();
	myUTFT.print("Calibration in Progress", CENTER, 30);
	myUTFT.print("Seconds left until ready " + thisString, CENTER, 42);


	switch(iCounter){

	case 0:
		Serial.println("PRINT 0");
		myUTFT.print("************************************", CENTER, 114);
		iCounter = 1;
		break;
	case 1:
		Serial.println("PRINT 1");
		myUTFT.print("******************", CENTER, 114);
		iCounter = 2;
		break;
	case 2:
		Serial.println("PRINT 2");
		myUTFT.print("*****", CENTER, 114);
		iCounter = 0;
		break;

	}

   if(iSeconds == 0){
	   iSeconds = max_seconds_for_PH_calibration;
	   //Move to next stage.
	   pHTenCalibration();
   }

}

void PhCalibrateScreen::updatePH10WaitScreen()
{
	iSeconds = iSeconds - 3;
	String thisString = String(iSeconds);
	myUTFT.clrScr();
	myUTFT.print("Calibration in Progress", CENTER, 30);
	myUTFT.print("Seconds left until ready " + thisString, CENTER, 42);


	switch(iCounter){

	case 0:
		Serial.println("PRINT 0");
		myUTFT.print("************************************", CENTER, 114);
		iCounter = 1;
		break;
	case 1:
		Serial.println("PRINT 1");
		myUTFT.print("******************", CENTER, 114);
		iCounter = 2;
		break;
	case 2:
		Serial.println("PRINT 2");
		myUTFT.print("*****", CENTER, 114);
		iCounter = 0;
		break;

	}

   if(iSeconds == 0){
	   iSeconds = max_seconds_for_PH_calibration;
	   //Move to next stage.
	   endCalibration();
   }

}



void PhCalibrateScreen::begin()
{
	// Initial setup
	myUTFT.InitLCD();
	myUTFT.clrScr();

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	myUTFT.setFont(BigFont);
	myUTFT.setBackColor(0, 0, 255);
	myButtons.setTextFont(BigFont);
	
	waiting_for_PH_info = false;

	resetButtons();
	
	PHsensorstring = "";
	PHsensorstring.reserve(30); //set aside some bytes for receiving data
	PHinputstring = "";
	
	Serial2.begin(38400); //To communicate with the sensor.

	Serial.begin(115200);

	//Start Read straight away.
	PHinputstring = ( waterTempString + ",C\r");  //Start based on temp.
	Serial2.print(PHinputstring); //send command to sensor.
	PHinputstring = "";
}

void PhCalibrateScreen::drawScreen()
{
	//Clear What was there.
	myUTFT.clrScr();
	myUTFT.setBackColor(0, 0, 255);
	resetButtons();
	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLUE);


	resetButton = myButtons.addButton( 10, 10, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	infoButton = myButtons.addButton( 160, 10, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	readButton = myButtons.addButton( 10, 64, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	tempReadButton = myButtons.addButton( 160, 64, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	stopButton = myButtons.addButton( 10, 116, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	startButton = myButtons.addButton( 160, 116, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	backButton = myButtons.addButton( 10, 170, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	calibrateButton = myButtons.addButton( 160, 170, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);

	myButtons.drawButtons();

	//Add text to buttons.
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Reset", 55, 30);
	myUTFT.print("Info", 190, 30);
	myUTFT.print("Read", 55, 90);
	myUTFT.print("TRead", 190, 90);
	myUTFT.print("Stop", 55, 146);
	myUTFT.print("Start", 190, 146);
	myUTFT.print("Back", 55, 195);
	myUTFT.print("Calibrate", 185, 195);
}

void PhCalibrateScreen::SdrawScreen()
{
	myUTFT.clrScr();
	myUTFT.setBackColor(0, 0, 255);

	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLUE);

	myButtons.addButton( 10, 10, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	myButtons.addButton( 160, 10, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	myButtons.addButton( 10, 64, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	myButtons.addButton( 160, 64, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	myButtons.addButton( 10, 116, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	myButtons.addButton( 160, 116, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	myButtons.addButton( 10, 170, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	myButtons.addButton( 160, 170, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);

	myButtons.drawButtons();

	//Add text to buttons.
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Reset", 55, 30);
	myUTFT.print("Info", 190, 30);
	myUTFT.print("Read", 55, 90);
	myUTFT.print("TRead", 190, 90);
	myUTFT.print("Stop", 55, 146);
	myUTFT.print("Start", 190, 146);
	myUTFT.print("Back", 55, 195);
	myUTFT.print("Calibrate", 185, 195);
}


void PhCalibrateScreen::handleExitScreen()
{

}

int PhCalibrateScreen::handleScreen()
{
	int pressed_button = myButtons.checkButtons();

	 if (pressed_button==backButton)
		{
		handleExitScreen();
		return PH_BUTTON;
		}
	if (pressed_button==resetButton)
		{
		factoryReset();
		}
	if (pressed_button==infoButton)
		{
		getInfo();
		}
	if (pressed_button==readButton)
		{
		getSingleReading();
		}
	if (pressed_button==tempReadButton)
		{
		getTempSingleReading();
		}
	if (pressed_button==startButton)
		{
		setStartMode();
		}
	if (pressed_button==stopButton)
		{
		setStopMode();
		}
	if (pressed_button==calibrateButton)
		{
		startCalibration();
		}
	 if (pressed_button==subBackButton && !factReset)
		 {
		 drawScreen();
		 }

	 factReset = false;

	return NONE;
}

void PhCalibrateScreen::startCalibration()
{

	myButtons.disableButton( resetButton );
	myButtons.disableButton( infoButton );
	myButtons.disableButton( readButton );
	myButtons.disableButton( tempReadButton );
	myButtons.disableButton( stopButton );
	myButtons.disableButton( startButton );
	myButtons.disableButton( backButton);
	myButtons.disableButton( calibrateButton);

	myUTFT.setFont(SmallFont);
	pHSevenCalibration();
}

void PhCalibrateScreen::pHSevenCalibration()
{

	myUTFT.clrScr();
	myUTFT.setColor(255, 0, 0);
	myUTFT.fillRect(0, 0, 220, 13);
	myUTFT.setColor(255, 255, 255);
	myUTFT.setBackColor(255, 0, 0);
	myUTFT.drawLine(0, 14, 320, 14);
	myUTFT.print("pH Calibration", CENTER, 1);
	myUTFT.setBackColor(0, 0, 0);

	myUTFT.print("Calibrate now with the pH 7 liquid", LEFT, 30);
	myUTFT.print("Place the sensor in the pH 7 ", LEFT, 42);
	myUTFT.print("calibration liquid. This process", LEFT, 54);
	myUTFT.print("will take 5 minutes to complete", LEFT, 66);
	myUTFT.print("Wait for process to finish before ", LEFT, 78);
	myUTFT.print("removing the sensor from the liquid", LEFT, 90);
	myUTFT.print("", LEFT, 102);
	myUTFT.print("************************************", LEFT, 114);
	myUTFT.print("Touch screen to continue", CENTER, 162);

	waitForTouch();
	myUTFT.clrScr();

	PHinputstring = "C\r";  //set in continious mode
	delay(2000);  //Wait 2 seconds 
	Serial2.print(PHinputstring); //send command to sensor.
	PHinputstring = "S\r";  //set the sensor to calibrate for pH7.
	Serial2.print(PHinputstring);

	myUTFT.clrScr();
	String thisSt = String(iSeconds);
	myUTFT.print("Calibration in Progress", CENTER, 30);
	myUTFT.print("Seconds left until ready " + thisSt, CENTER, 42);
	simpleTimer.setTimer(3000, updateWaitScreen, /*100*/ 4); //Update screen every 3 seconds for 2 minues.
}

void PhCalibrateScreen::pHFourCalibration()
{

	myUTFT.clrScr();
	myUTFT.setColor(255, 0, 0);
	myUTFT.fillRect(0, 0, 220, 13);
	myUTFT.setColor(255, 255, 255);
	myUTFT.setBackColor(255, 0, 0);
	myUTFT.drawLine(0, 14, 320, 14);
	myUTFT.print("pH Calibration", CENTER, 1);
	myUTFT.setBackColor(0, 0, 0);

	myUTFT.print("Calibrate now with the pH 4 liquid", LEFT, 30);
	myUTFT.print("Place the sensor in the pH 4 ", LEFT, 42);
	myUTFT.print("calibration liquid. This process", LEFT, 54);
	myUTFT.print("will take 5 minutes to complete", LEFT, 66);
	myUTFT.print("Wait for process to finish before ", LEFT, 78);
	myUTFT.print("removing the sensor from the liquid", LEFT, 90);
	myUTFT.print("", LEFT, 102);
	myUTFT.print("************************************", LEFT, 114);
	myUTFT.print("Touch screen to continue", CENTER, 162);

	SwaitForTouch();
	myUTFT.clrScr();

	PHinputstring = "F\r";  //set the sensor to calibrate for pH4.
	Serial2.print(PHinputstring);

	myUTFT.clrScr();
	String thisSt = String(iSeconds);
	myUTFT.print("Calibration in Progress", CENTER, 30);
	myUTFT.print("Seconds left until ready " + thisSt, CENTER, 42);
	simpleTimer.setTimer(3000, updatePH7WaitScreen, /*100*/ 4); //Update screen every 3 seconds for 2 minues.
}

void PhCalibrateScreen::pHTenCalibration()
{

	myUTFT.clrScr();
	myUTFT.setColor(255, 0, 0);
	myUTFT.fillRect(0, 0, 220, 13);
	myUTFT.setColor(255, 255, 255);
	myUTFT.setBackColor(255, 0, 0);
	myUTFT.drawLine(0, 14, 320, 14);
	myUTFT.print("pH Calibration", CENTER, 1);
	myUTFT.setBackColor(0, 0, 0);

	myUTFT.print("Calibrate now with the pH 10 liquid", LEFT, 30);
	myUTFT.print("Place the sensor in the pH 10 ", LEFT, 42);
	myUTFT.print("calibration liquid. This process", LEFT, 54);
	myUTFT.print("will take 5 minutes to complete", LEFT, 66);
	myUTFT.print("Wait for process to finish before ", LEFT, 78);
	myUTFT.print("removing the sensor from the liquid", LEFT, 90);
	myUTFT.print("", LEFT, 102);
	myUTFT.print("************************************", LEFT, 114);
	myUTFT.print("Touch screen to continue", CENTER, 162);

	SwaitForTouch();
	myUTFT.clrScr();

	PHinputstring = "T\r";  //set the sensor to calibrate for pH10.
	Serial2.print(PHinputstring);

	myUTFT.clrScr();
	String thisSt = String(iSeconds);
	myUTFT.print("Calibration in Progress", CENTER, 30);
	myUTFT.print("Seconds left until ready " + thisSt, CENTER, 42);
	simpleTimer.setTimer(3000, updatePH10WaitScreen, /*100*/ 4); //Update screen every 3 seconds for 2 minues.
}

void PhCalibrateScreen::endCalibration()
{
	PHinputstring = "E\r";  //end continious mode
	//Serial2.print(PHinputstring); //send command to sensor.
	myUTFT.fillScr(VGA_BLUE);
	myUTFT.setColor(VGA_RED);
	myUTFT.fillRoundRect(80, 70, 239, 169);

	myUTFT.setColor(VGA_WHITE);
	myUTFT.setBackColor(VGA_RED);
	myUTFT.print("! All Done !", CENTER, 93);
	myUTFT.print("Touch Screen", CENTER, 119);
	myUTFT.print("To Exit", CENTER, 132);

	SwaitForTouch();
	myUTFT.clrScr();

	myUTFT.setFont(BigFont);

	myButtons.enableButton( /*resetButton*/ 0);
	myButtons.enableButton( /*infoButton*/ 1);
	myButtons.enableButton( /*readButton*/ 2);
	myButtons.enableButton( /*tempReadButton*/ 3);
	myButtons.enableButton( /*stopButton*/ 4);
	myButtons.enableButton( /*startButton*/ 5);
	myButtons.enableButton( /*backButton*/6);
	myButtons.enableButton( /*calibrateButton*/7);

	SdrawScreen();
}


//This stops two myButtons having the same number.
void PhCalibrateScreen::resetButtons()
{
	Serial.print("RESET ALL BUTTONS");
	myButtons.deleteAllButtons();

	resetButton = -10;
	infoButton = -10;
	readButton = -10;
	tempReadButton = -10;
	startButton = -10;
	stopButton = -10;
	backButton = -10;
	calibrateButton = -10;
	subBackButton = -10;
}

/*
 * 1. The type of device:
2. The firmware version number:
3. The firmware version date:

Full proper syntax: i<cr> or I<CR>
The pH Circuit will respond: P,V5.0,5/13<CR>

P (for pH)
V5.0
5/13 (May / 2013)

 */
void PhCalibrateScreen::getInfo()
{
	String inputstring = "I\r";  //Command to get info
	Serial2.print(inputstring); //send command to sensor.

	//Draw part of the screen here and the info when we have it.
	//info comes after the loop has iterated.
	myUTFT.clrScr();
	resetButtons();
	//Draw back button
	subBackButton = myButtons.addButton( 10, 168, 145, 53, mainButton);
	myButtons.drawButton(subBackButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Back", 45, 185);

	waiting_for_PH_info = true;
}

//Static so can be used with the callback.
void drawPHInfoScreen()
{
	String version = PHsensorstring.substring(3,6);
	String date = PHsensorstring.substring(7,11);
	PHsensorstring="";

	//resetButtons();
	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLACK);
	//Indicate that we have changed screens.


	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.print("PH HW Info", CENTER, 1);

	myUTFT.print("Sensor Version", CENTER, 30);
	myUTFT.print(version, CENTER, 54);
	myUTFT.print("Firmware Date", CENTER, 78);
	myUTFT.print(date, CENTER, 102);
}

//Static so can be used with the callback.
void drawPHReadingScreen()
{
	String PH = PHsensorstring;
	PHsensorstring="";

	//resetButtons();
	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLACK);
	//Indicate that we have changed screens.


	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.print("PH Get Reading", CENTER, 1);

	myUTFT.print("PH Level", CENTER, 30);
	myUTFT.print(PH, CENTER, 54);
}

void serialEvent2()
{
Serial.print("IN SERIAL EVENT 2\n");
PHsensorstring="";
while(Serial2.available())
	   {
	   char inchar = (char)Serial2.read();                              //get the char we just received
	   PHsensorstring += inchar;   //add it to the inputString
	   if(inchar == '\r') {PHsensor_stringcomplete = true;}
	   }

Serial.print("LENGTH OF STRING \n");
Serial.print( PHsensorstring.length());
Serial.print("\n END LENGTH OF STRING \n");
/*if(PHsensorstring.length() == 9) //length of PH response.
{
	PHLevelString = PHsensorstring.substring(0,3);
	Serial.print("PH STRING IS \n");
	Serial.print(PHLevelString);
	Serial.print("\n END PH STRING \n");
}*/

Serial.print(PHsensorstring);
//check which data we are waiting for.
if(waiting_for_PH_info && PHsensor_stringcomplete)
	{
	//this is needed if the info and a reading come at the same time.
	//we get rid of the reading here.
	if(PHsensorstring.length() > 12)
	{
		Serial.print("OFFSET IS \n");
		int offset = PHsensorstring.length() - 12;
		Serial.print(offset);
		PHsensorstring = PHsensorstring.substring(offset,PHsensorstring.length());
		Serial.print("PH PHsensorstring IS \n");
		Serial.print(PHsensorstring);
		Serial.print("\n END PHsensorstring STRING \n");
	}
	drawPHInfoScreen();
	waiting_for_PH_info = false;
	PHsensor_stringcomplete = false;
	}
if(waiting_for_PH_single_reading && PHsensor_stringcomplete)
	{
	Serial.print("\n Drawing Screen \n");
	drawPHReadingScreen();
	waiting_for_PH_single_reading = false;
	PHsensor_stringcomplete = false;
	}

}

void PhCalibrateScreen::getSingleReading()
{
	waiting_for_PH_single_reading = true;
	PHinputstring = "R\r";  //Command to get reading.
	Serial2.print(PHinputstring); //send command to sensor.
	delay(378); //This takes 378ms to complete.
	//Draw part of the screen here and the info when we have it.
	//info comes after the loop has iterated.
	myUTFT.clrScr();
	resetButtons();
	//Draw back button
	subBackButton = myButtons.addButton( 10, 168, 145, 53, mainButton);
	myButtons.drawButton(subBackButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Back", 45, 185);
}

void PhCalibrateScreen::getTempSingleReading()
{
	waiting_for_PH_single_reading = true;
	PHinputstring = ( waterTempString + "\r");  //"17.8\r"Command to get reading.
	Serial2.print(PHinputstring); //send temp to sensor
	PHinputstring = "R\r";  //Command to get reading.
	Serial2.print(PHinputstring); //send command to sensor
	delay(378); //This takes 378ms to complete.
	//Draw part of the screen here and the info when we have it.
	//info comes after the loop has iterated.
	myUTFT.clrScr();
	resetButtons();
	//Draw back button
	subBackButton = myButtons.addButton( 10, 168, 145, 53, mainButton);
	myButtons.drawButton(subBackButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Back", 45, 185);
}

void PhCalibrateScreen::setStopMode()
{
	PHinputstring = "E\r";  //Command to get info
	Serial2.print(PHinputstring); //send command to sensor.

	inStopMode = true;
	drawStoppingScreen();

}

void PhCalibrateScreen::setStartMode()
{
	PHinputstring = ( waterTempString + ",C\r");  //Start based on temp.
	Serial2.print(PHinputstring); //send command to sensor.

	delay(1000);
	inStopMode = false;
	drawStartingScreen();
}

void PhCalibrateScreen::factoryReset()
{
	PHinputstring = "X\r";  //Command to get info
	Serial2.print(PHinputstring); //send command to sensor.

	factReset = true;  //Hack as they share the same button ID.
	delay(1000);
	drawResetScreen();
}

void PhCalibrateScreen::drawResetScreen()
{
	myUTFT.clrScr();
	resetButtons();

	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLACK);

	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.print("PH RESET", CENTER, 1);

	myUTFT.print("All PH Readings", CENTER, 30);
	myUTFT.print("Now  Reset", CENTER, 54);
	myUTFT.print("Sensor UnCalibrated", CENTER, 78);

	//Draw back button
	subBackButton = myButtons.addButton( 10, 168, 145, 53, mainButton);
	myButtons.drawButton(subBackButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Back", 45, 185);
}

void PhCalibrateScreen::drawStartingScreen()
{
	myUTFT.clrScr();
	resetButtons();

	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLACK);

	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.print("PH STARTING", CENTER, 1);

	myUTFT.print("All PH Readings", CENTER, 30);
	myUTFT.print("Now  Started", CENTER, 54);
	myUTFT.print("Sensor Ready", CENTER, 78);

	//Draw back button
	subBackButton = myButtons.addButton( 10, 168, 145, 53, mainButton);
	myButtons.drawButton(subBackButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Back", 45, 185);
}

void PhCalibrateScreen::drawStoppingScreen()
{
	myUTFT.clrScr();
	resetButtons();

	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLACK);

	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.print("PH STOPPED", CENTER, 1);

	myUTFT.print("All PH Readings", CENTER, 30);
	myUTFT.print("Now Stopped", CENTER, 54);
	myUTFT.print("Sensor Standby", CENTER, 78);

	//Draw back button
	subBackButton = myButtons.addButton( 10, 168, 145, 53, mainButton);
	myButtons.drawButton(subBackButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Back", 45, 185);
}






//
// END OF FILE
//
