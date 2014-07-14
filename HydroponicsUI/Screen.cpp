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


#include "Screen.h"

Screen::Screen(){
}

void Screen::begin(){
}

int Screen::handleScreenButtons(int pressed_button){


	 //Put these first as not all buttons are deleted.
	if (pressed_button==back2SetupButton)
		{
		return BACK2SETUP_BUTTON;
		}
			 if (pressed_button==back2MainButton)
			 	{

			 	return BACK2MAIN_BUTTON;
			 	}
			  if (pressed_button==pHButton)
			  {

				  return PH_BUTTON;
			  }
			  if (pressed_button==ECButton)
			  {

				  return EC_BUTTON;
			  }
			  if (pressed_button==airButton)
			  {

				  return AIR_BUTTON;
			  }
			  if (pressed_button==waterButton)
			  {

				  return WATER_BUTTON;
			  }
			  if (pressed_button==plugsButton)
			  {

				  return PLUGS_BUTTON;
			  }
			  if (pressed_button==aboutButton)
			  {

				  return ABOUT_BUTTON;
			  }
			  if (pressed_button==clockButton)
			  {

				  return CLOCK_BUTTON;
			  }
			  if (pressed_button==pumpButton)
			 	{

			 	return PUMP_BUTTON;
			 	}
			 if (pressed_button==lightsButton)
			 	 {

			 	 return LAMPS_BUTTON;
			 	 }
			 if (pressed_button==lightSwitchButton)
			 	{

			 	return LIGHTSWITCH_BUTTON;
			 	 }



}

void Screen::resetButtonIDs(){

	airButton = -10;
	pHButton = -10;
	ECButton = -10;
	waterButton = -10;
	plugsButton = -10;
	aboutButton = -10;
	back2SetupButton = -10;
	clockButton = -10;
	back2MainButton = -10;
	plusMaxButton = -10;
	minusMaxButton = -10;
	plusMinButton= -10;
	minusMinButton = -10;
	unitsButton = -10;
	lightsButton = -10;
	pumpButton = -10;
}









//
// END OF FILE
//
