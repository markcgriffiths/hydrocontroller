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


#include "PumpScreen.h"

//Global definition for whole of the UI
extern UTFT myUTFT;
extern UTouch myTouch;
extern UTFT_Buttons myButtons;

PumpScreen::PumpScreen(){
	Serial.begin(115200);
}

void PumpScreen::drawScreen()
{
	Serial.println("PumpTimerScreen::drawScreen start  \n");
	max = EEPROM.read(EEPROM_PUMP_TIMER_HOUR);
	min = EEPROM.read(EEPROM_PUMP_TIMER_MIN);
	drawMinMaxScreen( max, min, units );
	//Setup header last as drawMinMaxScreen will clear screen.
	drawHeader( "Pump Countdown",  "Set Hours & Mins"  );
	Serial.println("PumpScreen::drawScreen end  \n");

	drawDurationButton();
	iEngine->begin();
}

void PumpScreen::refreshScreen()
{
	if( max > 24 )
		max = 0;

	if( max < 0 )
		max = 24;

	if ( min > 60 )
		min = 0;

	if( min < 0 )
		min = 60;

	refreshMinMaxScreen( max, min, units );
}

void PumpScreen::handleExitScreen()
{
	Serial.println("PumpScreen::Exit start");
	Serial.println(max);
	Serial.println(min);
	Serial.println("PumpScreen::Exit start 11");
	EEPROM.write(EEPROM_PUMP_TIMER_HOUR, max);
	Serial.println("PumpScreen::Exit start 22");
	EEPROM.write(EEPROM_PUMP_TIMER_MIN, min);
	Serial.println("PumpScreen::Exit start 33");
	iEngine->SetPumpTimer(max, min);
	Serial.println("PumpScreen::Exit end");
}

int PumpScreen::handleScreen()
{
	Serial.println("PumpScreen::handleScreen start  \n");

	int pressed_button = myButtons.checkButtons();
	if(durationButton == pressed_button)
	{
		return PUMP_DURATION_BUTTON;
	}
	if (pressed_button==back2MainButton)
					{
					handleExitScreen();
					return BACK2SETUP_BUTTON;
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
						units = 0;
					else
						units++;
					}

				refreshScreen();
				return NONE;

	//return handleMinMaxScreen( max, min, units );
}

void PumpScreen::drawDurationButton()
{
	durationButton = myButtons.addButton( 160, 170, MAIN_BUTTON_X, MAIN_BUTTON_Y, mainButton);
	myButtons.drawButton(durationButton);

	//Draw Button Text
	myUTFT.setBackColor (161,190,237);
	myUTFT.print("Duration", 160, 185 );

}



//
// END OF FILE
//
