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


#include "ECCalibrateScreen.h"

extern UTFT myUTFT;
extern UTouch myTouch;
extern UTFT_Buttons myButtons;


String sensorstring;
String inputstring;
boolean waiting_for_info;
boolean waiting_for_single_reading;
boolean sensor_stringcomplete;
boolean waiting_for_dry;
String ECLevelString;

extern SimpleTimer simpleTimer;
static int iCounter = 0;
static int max_seconds_for_calibration = 12;
static int iSeconds = max_seconds_for_calibration;
static int iTimesToUpdateScreen = (60);

extern String waterTempString;

ECCalibrateScreen::ECCalibrateScreen(){
}

void ECCalibrateScreen::waitForTouch()
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

void ECCalibrateScreen::SwaitForTouch()
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

void ECCalibrateScreen::begin()
{

	// Initial setup
	myUTFT.InitLCD();
	myUTFT.clrScr();

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	myUTFT.setFont(BigFont);
	myUTFT.setBackColor(0, 0, 255);
	myButtons.setTextFont(BigFont);

	resetButtons();

	sensorstring = "";
	sensorstring.reserve(30); //set aside some bytes for receiving data
	inputstring = "";

	waiting_for_info = false;
	waiting_for_single_reading = false;
	sensor_stringcomplete = false;
	factReset = false;
	inStopMode = false;
	waiting_for_dry = false;

	Serial3.begin(38400); //To communicate with the sensor.

	Serial.begin(115200);

	//Start Read straight away.
	inputstring = ( waterTempString + ",C\r");  //Start based on temp.
	Serial3.print(inputstring); //send command to sensor.

}

void ECCalibrateScreen::drawScreen()
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

void ECCalibrateScreen::SdrawScreen()
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


void ECCalibrateScreen::handleExitScreen()
{

}

void ECCalibrateScreen::startCalibration()
{

	myButtons.disableButton( resetButton );
	myButtons.disableButton( infoButton );
	myButtons.disableButton( readButton );
	myButtons.disableButton( tempReadButton );
	myButtons.disableButton( stopButton );
	myButtons.disableButton( startButton );
	myButtons.disableButton( backButton);
	myButtons.disableButton( calibrateButton);

	inputstring = "P,1\r";  //In Hydroponics we use always E.C Sensor 0.1 -- Fresh Water
	Serial3.print(inputstring); //send command to sensor.

	myUTFT.setFont(SmallFont);
	dryCalibration();
	highCalibration();

}
void ECCalibrateScreen::updateWaitScreen()
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
	   iSeconds = max_seconds_for_calibration;
	   //Move to next stage.
	   lowCalibration();
   }



}

void ECCalibrateScreen::endCalibration()
{
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

void ECCalibrateScreen::updateLowWaitScreen()
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
	   iSeconds = max_seconds_for_calibration;
	   //Move to next stage.
	   endCalibration();
   }



}

void ECCalibrateScreen::highCalibration()
{
	myUTFT.clrScr();
	myUTFT.setColor(255, 0, 0);
	myUTFT.fillRect(0, 0, 220, 13);
	myUTFT.setColor(255, 255, 255);
	myUTFT.setBackColor(255, 0, 0);
	myUTFT.drawLine(0, 14, 320, 14);
	myUTFT.print("EC Calibration", CENTER, 1);
	myUTFT.setBackColor(0, 0, 0);

	myUTFT.print("Calibrate now with the high side liquid", LEFT, 30);
	myUTFT.print("Place the sensor in the 3,000Us ", LEFT, 42);
	myUTFT.print("calibration liquid. This process", LEFT, 54);
	myUTFT.print("will take 5 minutes to complete", LEFT, 66);
	myUTFT.print("Wait for process to finish before ", LEFT, 78);
	myUTFT.print("removing the sensor from the liquid", LEFT, 90);
	myUTFT.print("", LEFT, 102);
	myUTFT.print("************************************", LEFT, 114);
	myUTFT.print("Touch screen to continue", CENTER, 162);

	waitForTouch();
	myUTFT.clrScr();

	inputstring = "C\r";  //set in continious mode

	myUTFT.clrScr();
	String thisSt = String(iSeconds);
	myUTFT.print("Calibration in Progress", CENTER, 30);
	myUTFT.print("Seconds left until ready " + thisSt, CENTER, 42);
	simpleTimer.setTimer(3000, updateWaitScreen, /*100*/ 4); //Update screen every 3 seconds for 5 minues.

}


void ECCalibrateScreen::lowCalibration()
{
	myUTFT.clrScr();
	myUTFT.setColor(255, 0, 0);
	myUTFT.fillRect(0, 0, 220, 13);
	myUTFT.setColor(255, 255, 255);
	myUTFT.setBackColor(255, 0, 0);
	myUTFT.drawLine(0, 14, 320, 14);
	myUTFT.print("EC Calibration", CENTER, 1);
	myUTFT.setBackColor(0, 0, 0);

	myUTFT.print("Calibrate now with the low side liquid", LEFT, 30);
	myUTFT.print("Place the sensor in the 220Us", LEFT, 42);
	myUTFT.print("calibration liquid.  This process", LEFT, 54);
	myUTFT.print("will take 5 mins to complete", LEFT, 66);
	myUTFT.print("Wait for process to finish before ", LEFT, 78);
	myUTFT.print("removing the sensor from the liquid", LEFT, 90);
	myUTFT.print("", LEFT, 102);
	myUTFT.print("************************************", LEFT, 114);
	myUTFT.print("Touch screen to continue", CENTER, 162);

	SwaitForTouch();
	myUTFT.clrScr();

	inputstring = "C\r";  //set in continious mode

	String thisSt = String(12);
	myUTFT.print("Calibration in Progress", CENTER, 30);
	myUTFT.print("Seconds left until ready " + thisSt, CENTER, 42);
	simpleTimer.setTimer(3000, updateLowWaitScreen, /*100*/ 4); //Update screen every 3 seconds for 5 minues.


	//delay(2000);  //Wait 2 seconds -- make 5mins
	//Serial3.print(inputstring); //send command to sensor.
	//inputstring = "Z2\r";  //set the sensor at this value.
	//Serial3.print(inputstring); //need to send twice??


}

void ECCalibrateScreen::dryCalibration()
{

	myUTFT.clrScr();
	myUTFT.setColor(255, 0, 0);
	myUTFT.fillRect(0, 0, 220, 13);
	myUTFT.setColor(255, 255, 255);
	myUTFT.setBackColor(255, 0, 0);
	myUTFT.drawLine(0, 14, 320, 14);
	myUTFT.print("EC Calibration", CENTER, 1);
	myUTFT.setBackColor(0, 0, 0);

	myUTFT.print("Do NOT put sensor in any Liquid you are", LEFT, 30);
	myUTFT.print("going to calibrate for a dry condition", LEFT, 42);
	myUTFT.print("This is much like setting a TARE on a", LEFT, 54);
	myUTFT.print("scale", LEFT, 66);
	myUTFT.print("", LEFT, 78);
	myUTFT.print("Please make sure that the EC sensor is", LEFT, 90);
	myUTFT.print("connected to the BNC connector before", LEFT, 102);
	myUTFT.print("you continue", LEFT, 114);
	//myUTFT.print("crosshairs in sequence.", LEFT, 126);
	myUTFT.print("Touch screen to continue", CENTER, 162);

	waitForTouch();
	myUTFT.clrScr();

	inputstring = "Z0\r";  //Command to get info
	Serial3.print(inputstring); //send command to sensor.

}

int ECCalibrateScreen::handleScreen()
{

	int pressed_button = myButtons.checkButtons();

	 if (pressed_button==backButton)
		{
		handleExitScreen();
		return EC_BUTTON;
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

//This stops two myButtons having the same number.
void ECCalibrateScreen::resetButtons()
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
 * Delivering this instruction will instruct the E.C. Circuit to transmit it device info.
	Full proper syntax: i<cr> or I<CR>
	The E.C. Circuit will respond: E,V3.0,4/11<CR>
	Where:
	E =E.C. Circuit
	V3.1= Firmware version
	5/13= Date firmware was written
 */
void ECCalibrateScreen::getInfo()
{
	inputstring = "I\r";  //Command to get info
	Serial3.print(inputstring); //send command to sensor.

	//Draw part of the screen here and the info when we have it.
	//info comes after the loop has iterated.
	myUTFT.clrScr();
	resetButtons();
	//Draw back button
	subBackButton = myButtons.addButton( 10, 168, 145, 53, mainButton);
	myButtons.drawButton(subBackButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Back", 45, 185);

	waiting_for_info = true;

}

void ECCalibrateScreen::getSingleReading()
{
	inputstring = "R\r";  //Command to get reading.
	Serial3.print(inputstring); //send command to sensor.
	delay(1000); //This takes 1000ms to complete.


	//Draw part of the screen here and the info when we have it.
	//info comes after the loop has iterated.
	myUTFT.clrScr();
	resetButtons();
	//Draw back button
	subBackButton = myButtons.addButton( 10, 168, 145, 53, mainButton);
	myButtons.drawButton(subBackButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Back", 45, 185);

	waiting_for_single_reading = true;

}

void ECCalibrateScreen::getTempSingleReading()
{
	inputstring = ( waterTempString + "\r");  //"17.8\r"Command to get reading.
	Serial3.print(inputstring); //send command to sensor.
	delay(1000); //This takes 1000ms to complete.

	//Draw part of the screen here and the info when we have it.
	//info comes after the loop has iterated.
	myUTFT.clrScr();
	resetButtons();
	//Draw back button
	subBackButton = myButtons.addButton( 10, 168, 145, 53, mainButton);
	myButtons.drawButton(subBackButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Back", 45, 185);

	waiting_for_single_reading = true;

}

void ECCalibrateScreen::setStopMode()
{
	inputstring = "E\r";  //Command to get info
	Serial3.print(inputstring); //send command to sensor.

	inStopMode = true;
	drawStoppingScreen();

}

void ECCalibrateScreen::setStartMode()
{
	inputstring = ( waterTempString + ",C\r");  //Start based on temp.
	Serial3.print(inputstring); //send command to sensor.

	//delay(1000);
	inStopMode = false;
	drawStartingScreen();

}

void ECCalibrateScreen::factoryReset()
{
	inputstring = "X\r";  //Command to get info
	Serial3.print(inputstring); //send command to sensor.

	factReset = true;  //Hack as they share the same button ID.
	delay(1000);
	drawResetScreen();

}

void ECCalibrateScreen::drawResetScreen()
{
	myUTFT.clrScr();
	resetButtons();

	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLACK);

	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.print("EC RESET", CENTER, 1);

	myUTFT.print("All EC Readings", CENTER, 30);
	myUTFT.print("Now  Reset", CENTER, 54);
	myUTFT.print("Sensor UnCalibrated", CENTER, 78);

	//Draw back button
	subBackButton = myButtons.addButton( 10, 168, 145, 53, mainButton);
	myButtons.drawButton(subBackButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Back", 45, 185);


}

void ECCalibrateScreen::drawStartingScreen()
{
	myUTFT.clrScr();
	resetButtons();

	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLACK);

	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.print("EC STARTING", CENTER, 1);

	myUTFT.print("All EC Readings", CENTER, 30);
	myUTFT.print("Now  Started", CENTER, 54);
	myUTFT.print("Sensor Ready", CENTER, 78);

	//Draw back button
	subBackButton = myButtons.addButton( 10, 168, 145, 53, mainButton);
	myButtons.drawButton(subBackButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Back", 45, 185);


}

void ECCalibrateScreen::drawStoppingScreen()
{
	myUTFT.clrScr();
	resetButtons();

	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLACK);

	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.print("EC STOPPED", CENTER, 1);

	myUTFT.print("All EC Readings", CENTER, 30);
	myUTFT.print("Now Stopped", CENTER, 54);
	myUTFT.print("Sensor Standby", CENTER, 78);

	//Draw back button
	subBackButton = myButtons.addButton( 10, 168, 145, 53, mainButton);
	myButtons.drawButton(subBackButton);
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Back", 45, 185);
}


//Static so can be used with the callback.
void drawReadingScreen()
{
	String EC = sensorstring.substring(0,3);
	String TDS = sensorstring.substring(4,6);
	String SAL = sensorstring.substring(7,8);
	sensorstring="";

	//resetButtons();
	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLACK);
	//Indicate that we have changed screens.


	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.print("EC Get Reading", CENTER, 1);

	myUTFT.print("EC Level", CENTER, 30);
	myUTFT.print(EC, CENTER, 54);
	myUTFT.print("TDS Level", CENTER, 78);
	myUTFT.print(TDS, CENTER, 102);
	myUTFT.print(SAL, CENTER, 126);

}

//Static so can be used with the callback.
void drawInfoScreen()
{
	String version = sensorstring.substring(3,6);
	String date = sensorstring.substring(7,11);
	sensorstring="";

	//resetButtons();
	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLACK);
	//Indicate that we have changed screens.


	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.print("EC HW Info", CENTER, 1);

	myUTFT.print("Sensor Version", CENTER, 30);
	myUTFT.print(version, CENTER, 54);
	myUTFT.print("Firmware Date", CENTER, 78);
	myUTFT.print(date, CENTER, 102);

}

void drawDryScreen()
{
	String version = sensorstring.substring(3,6);
	String date = sensorstring.substring(7,11);
	sensorstring="";

	//resetButtons();
	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLACK);
	//Indicate that we have changed screens.


	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.print("EC HW Info", CENTER, 1);

	myUTFT.print("Sensor Version", CENTER, 30);
	myUTFT.print(version, CENTER, 54);
	myUTFT.print("Firmware Date", CENTER, 78);
	myUTFT.print(date, CENTER, 102);

}

void serialEvent3(){
Serial.print("IN SERIAL EVENT 3\n");
sensorstring="";
while(Serial3.available())
	   {
	   char inchar = (char)Serial3.read();                              //get the char we just received
	   sensorstring += inchar;                                          //add it to the inputString
	   if(inchar == '\r') {sensor_stringcomplete = true;}
	   }

Serial.print("LENGTH OF STRING \n");
Serial.print(sensorstring.length());
Serial.print("\n END LENGTH OF STRING \n");
if(sensorstring.length() == 9) //length of EC response.
{
	ECLevelString = sensorstring.substring(0,3);
	Serial.print("EC STRING IS \n");
	Serial.print(ECLevelString);
	Serial.print("\n END EC STRING \n");
}

Serial.print(sensorstring);
//check which data we are waiting for.
if(waiting_for_info && sensor_stringcomplete)
	{
	drawInfoScreen();
	waiting_for_info = false;
	sensor_stringcomplete = false;
	}
if(waiting_for_single_reading && sensor_stringcomplete)
	{
	drawReadingScreen();
	waiting_for_single_reading = false;
	sensor_stringcomplete = false;
	}

}




//
// END OF FILE
//
