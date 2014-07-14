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

#ifndef TOUCHCALIBRATION_H
#define TOUCHCALIBRATION_H

#include <UI_Shared.h>


// Declare which fonts we will be using
extern uint8_t SmallFont[];
#define TOUCH_ORIENTATION  LANDSCAPE

class TouchCalibration
{
public:
	//Functions
	TouchCalibration();
	void begin();

private:
	void drawCrossHair(int x, int y);
	void readCoordinates();
	void calibrate(int x, int y, int i);
	void waitForTouch();
	void startup();
	void done();
	void toHex(uint32_t num);

private:
	uint32_t cx, cy;
	uint32_t rx[10], ry[10];
	uint32_t clx, crx, cty, cby;
	float px, py;
	int dispx, dispy, text_y_center;
	uint32_t calx, caly, cals;
	char buf[13];
	
    
};

#endif
//
// END OF FILE
//
