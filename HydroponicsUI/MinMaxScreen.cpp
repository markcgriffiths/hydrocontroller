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


#include "MinMaxScreen.h"
#include <string.h>
#include <EEPROMex.h>

//Global definition for whole of the UI
extern UTFT myUTFT;
extern UTouch myTouch;
extern UTFT_Buttons myButtons;


MinMaxScreen::MinMaxScreen()
{
}

void MinMaxScreen::begin(int AddMax, int AddMin, int AddUnit, boolean useFloats)
{

	// Initial setup
	myUTFT.InitLCD();
	myUTFT.clrScr();

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	myUTFT.setFont(BigFont);
	myUTFT.setBackColor(0, 0, 255);
	myButtons.setTextFont(BigFont);

	Serial.begin(115200);

	max = 0;
	min = 0;
	units = 0;
	floatMax = 0.0;
	floatMin = 0.0;
    drawUnits = false;
    isExitScreen = false;
    unitsButton =0;

    iUpperText = "Max";
    iLowerText = "Min";


 	//Read values stored on EEPROM.
    getSavedValues(AddMax, AddMin, AddUnit, useFloats);


}

void MinMaxScreen::getSavedValues(int AddMax, int AddMin, int AddUnit, boolean useFloats)
{
	//Get saved values:
	if(useFloats)
	{
		Serial.println("USING FLOATS  \n");
		floatMax = EEPROMEx.readFloat(AddMax);
		floatMin = EEPROMEx.readFloat(AddMin);
		units = EEPROM.read(AddUnit);
	}
	else
	{
		max = EEPROM.read(AddMax);
		min = EEPROM.read(AddMin);
		units = EEPROM.read(AddUnit);
	}

}

void MinMaxScreen::setDrawUnits(bool aDrawUnits )
{
	drawUnits = aDrawUnits;
}

void MinMaxScreen::refreshMinMaxScreen(int aMax, int aMin, int aUnits)
{
	myUTFT.setColor(VGA_WHITE);
	myUTFT.setBackColor (VGA_BLACK);

	//Refresh max Value
	myUTFT.printNumI(aMax, 100, 81, 3);

	//Refresh min Value
	myUTFT.printNumI(aMin, 100, 133, 3);

	myUTFT.setBackColor (161,190,237);
	//Delete button and then below recreate it
	myButtons.deleteButton(unitsButton);
	//Draw  Units Button
	drawUnitsButton( aUnits, 160, 170 );

}

void MinMaxScreen::refreshMinMaxScreen(float aMax, float aMin, int aUnits)
{
	Serial.println(aMax);
	myUTFT.setColor(VGA_WHITE);
	myUTFT.setBackColor (VGA_BLACK);

	//Refresh max Value
	myUTFT.printNumF(aMax, 1, 100, 81);

	//Refresh min Value
	myUTFT.printNumF(aMin, 1, 100, 133);

	//Draw  Units Button
	myUTFT.setBackColor (161,190,237);
	//Delete button and then below recreate it
	myButtons.deleteButton(unitsButton);
	drawUnitsButton( aUnits, 160, 170 );
}

void MinMaxScreen::drawValueBox( char* aText, int aValue, int x, int y )
{
	//Draw Max Value inputs
	myUTFT.setColor(VGA_WHITE);
	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.drawRoundRect (10, 64, 150, 114);
	myUTFT.drawRoundRect ( x, y, (x+140), (y+50) );
	myUTFT.print( aText, (x+25), (y+17) );
	myUTFT.printNumI(aValue, (x+90), (y+17), 3);

}

void MinMaxScreen::drawValueBox( char* aText, float aValue, int x, int y )
{
	//Draw Max Value inputs
	myUTFT.setColor(VGA_WHITE);
	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.drawRoundRect (10, 64, 150, 114);
	myUTFT.drawRoundRect ( x, y, (x+140), (y+50) );
	myUTFT.print( aText, (x+25), (y+17) );
	myUTFT.printNumF(aValue, 1, (x+90), (y+17), '.', 3);

}

void MinMaxScreen::drawHeader( char* aHeaderText,  char* aSubText  )
{
	myUTFT.setBackColor (VGA_BLACK);
	myUTFT.print(aHeaderText, CENTER, 1);
	myUTFT.print(aSubText, CENTER, 30);
}

void MinMaxScreen::SetValueText( char* aUpperText,  char* aLowerText  )
{
	iUpperText = aUpperText;
	iLowerText = aLowerText;
}

void MinMaxScreen::drawUnitsButton( int aUnits, int x, int y )
{
	if( drawUnits)
	{
	//Draw  Units Button
	unitsButton = myButtons.addButton( x, y, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);

	myButtons.drawButton(unitsButton);

	switch( aUnits)
		{
			case 0:
			{
			myUTFT.print("Unit C", (x+30), (y+17) );
			break;
			}
			case 1:
			{
			myUTFT.print("Unit F", (x+30), (y+17) );
			break;
			}
			case 2:
			{
			myUTFT.print("Unit K", (x+30), (y+17) );
			break;
			default:
			{
			Serial.println("MinMaxScreen::drawUnitsButton ERROR WITH UNITS  \n");
			myUTFT.print("Unit C", (x+30), (y+17) );
			}
			}
	   }
	}

}


void MinMaxScreen::drawMinMaxScreen( int aMax, int aMin, int aUnits )
{
	//Clear What was there.
	myUTFT.clrScr();

	resetButtons();

	//Draw Max Value inputs
	drawValueBox( iUpperText, aMax, 10, 64 );

	//Draw Min Value input
	drawValueBox( iLowerText, aMin, 10, 116 );


	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLUE);
	//Draw  increase Max Button
	plusMaxButton = myButtons.addButton( 160, 64, PLUS_BUTTON_X, PLUS_BUTTON_X, plusButton);
	//Draw  decrease Max Button
	minusMaxButton = myButtons.addButton( 230, 64, PLUS_BUTTON_X, PLUS_BUTTON_Y, minusButton);
	//Draw increase MIN Button
	plusMinButton = myButtons.addButton( 160, 116, PLUS_BUTTON_X, PLUS_BUTTON_Y, plusButton);
	//Draw  decrease Min Button
	minusMinButton = myButtons.addButton( 230, 116, PLUS_BUTTON_X, PLUS_BUTTON_Y, minusButton);

	//Draw OK button
	back2MainButton = myButtons.addButton( 10, 170, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);

	myButtons.drawButtons();

	//Add Text for buttons.
	//Draw Button Text
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Save", 40, 185);
	//Draw  Units Button
	drawUnitsButton( aUnits, 160, 170 );

}

void MinMaxScreen::drawMinMaxScreen( float aMax, float aMin, int aUnits )
{
	//Clear What was there.
	myUTFT.clrScr();

	resetButtons();

	//Draw Max Value inputs
	drawValueBox( iUpperText, aMax, 10, 64 );

	//Draw Min Value input
	drawValueBox( iLowerText, aMin, 10, 116 );


	myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLUE);
	//Draw  increase Max Button
	plusMaxButton = myButtons.addButton( 160, 64, PLUS_BUTTON_X, PLUS_BUTTON_X, plusButton);
	//Draw  decrease Max Button
	minusMaxButton = myButtons.addButton( 230, 64, PLUS_BUTTON_X, PLUS_BUTTON_Y, minusButton);
	//Draw increase MIN Button
	plusMinButton = myButtons.addButton( 160, 116, PLUS_BUTTON_X, PLUS_BUTTON_Y, plusButton);
	//Draw  decrease Min Button
	minusMinButton = myButtons.addButton( 230, 116, PLUS_BUTTON_X, PLUS_BUTTON_Y, minusButton);

	//Draw OK button
	back2MainButton = myButtons.addButton( 10, 170, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	myButtons.drawButtons();


	//Add Text for buttons.
	//Draw Button Text
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Save", 40, 185);
	//Draw  Units Button
	drawUnitsButton( aUnits, 160, 170 );

}

int MinMaxScreen::handleMinMaxScreen(int& max, int& min, int& units){


	int pressed_button = myButtons.checkButtons();

	 if (pressed_button==back2MainButton)
		{
		handleExitScreen();
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
		{
			Serial.println("MinMaxScreen::IS ZERO \n");
			units = 0;
		}

		else
		{
			Serial.println("MinMaxScreen::PLUS PLUS \n");
			units++;

		}

		}

	refreshScreen();
	return NONE;
}

int MinMaxScreen::handleMinMaxScreen(float& max, float& min, int& units){

	int pressed_button = myButtons.checkButtons();


	 if (pressed_button==back2MainButton)
		{
		handleExitScreen();
		return BACK2MAIN_BUTTON;
		}
	if (pressed_button==plusMaxButton)
		{
		max+=0.1;
		}
	if (pressed_button==minusMaxButton)
		{
		max-=0.1;
		}
	if (pressed_button==plusMinButton)
		{
		min+=0.1;
		}
	if (pressed_button==minusMinButton)
		{
		min-=0.1;
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

void  MinMaxScreen::resetButtons() {

	myButtons.deleteAllButtons();

	back2MainButton = -10;
	plusMaxButton = -10;
	minusMaxButton = -10;
	plusMinButton= -10;
	minusMinButton = -10;
	unitsButton = -10;


}

//
// END OF FILE
//
