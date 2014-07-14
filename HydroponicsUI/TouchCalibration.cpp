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


#include "TouchCalibration.h"
#include <string.h>


//Global definition for whole of the UI
extern UTFT myUTFT;
extern UTouch myTouch;
extern UTFT_Buttons myButtons;

TouchCalibration::TouchCalibration(){
}

void TouchCalibration::begin() {

	myUTFT.InitLCD();
	myUTFT.clrScr();
	myUTFT.setFont(SmallFont);

	myTouch.InitTouch(TOUCH_ORIENTATION);
	myTouch.setPrecision(PREC_LOW);
	  dispx=myUTFT.getDisplayXSize();
	  dispy=myUTFT.getDisplayYSize();
	  text_y_center=(dispy/2)-6;

	startup();

	myUTFT.setColor(80, 80, 80);
	  drawCrossHair(dispx-11, 10);
	  drawCrossHair(dispx/2, 10);
	  drawCrossHair(10, 10);
	  drawCrossHair(dispx-11, dispy/2);
	  drawCrossHair(10, dispy/2);
	  drawCrossHair(dispx-11, dispy-11);
	  drawCrossHair(dispx/2, dispy-11);
	  drawCrossHair(10, dispy-11);
	  myUTFT.setColor(255, 255, 255);
	  myUTFT.setBackColor(255, 0, 0);
	  myUTFT.print("***********", CENTER, text_y_center-12);
	  myUTFT.print("***********", CENTER, text_y_center+12);

	  calibrate(10, 10, 0);
	  calibrate(10, dispy/2, 1);
	  calibrate(10, dispy-11, 2);
	  calibrate(dispx/2, 10, 3);
	  calibrate(dispx/2, dispy-11, 4);
	  calibrate(dispx-11, 10, 5);
	  calibrate(dispx-11, dispy/2, 6);
	  calibrate(dispx-11, dispy-11, 7);

	  if (TOUCH_ORIENTATION == LANDSCAPE)
	    cals=(long(dispx-1)<<12)+(dispy-1);
	  else
	    cals=(long(dispy-1)<<12)+(dispx-1);

	  if (TOUCH_ORIENTATION == PORTRAIT)
	    px = abs(((float(rx[2]+rx[4]+rx[7])/3)-(float(rx[0]+rx[3]+rx[5])/3))/(dispy-20));  // PORTRAIT
	  else
	    px = abs(((float(rx[5]+rx[6]+rx[7])/3)-(float(rx[0]+rx[1]+rx[2])/3))/(dispy-20));  // LANDSCAPE

	  if (TOUCH_ORIENTATION == PORTRAIT)
	  {
	    clx = (((rx[0]+rx[3]+rx[5])/3));  // PORTRAIT
	    crx = (((rx[2]+rx[4]+rx[7])/3));  // PORTRAIT
	  }
	  else
	  {
	    clx = (((rx[0]+rx[1]+rx[2])/3));  // LANDSCAPE
	    crx = (((rx[5]+rx[6]+rx[7])/3));  // LANDSCAPE
	  }
	  if (clx<crx)
	  {
	    clx = clx - (px*10);
	    crx = crx + (px*10);
	  }
	  else
	  {
	    clx = clx + (px*10);
	    crx = crx - (px*10);
	  }

	  if (TOUCH_ORIENTATION == PORTRAIT)
	    py = abs(((float(ry[5]+ry[6]+ry[7])/3)-(float(ry[0]+ry[1]+ry[2])/3))/(dispx-20));  // PORTRAIT
	  else
	    py = abs(((float(ry[0]+ry[3]+ry[5])/3)-(float(ry[2]+ry[4]+ry[7])/3))/(dispx-20));  // LANDSCAPE

	  if (TOUCH_ORIENTATION == PORTRAIT)
	  {
	    cty = (((ry[5]+ry[6]+ry[7])/3));  // PORTRAIT
	    cby = (((ry[0]+ry[1]+ry[2])/3));  // PORTRAIT
	  }
	  else
	  {
	    cty = (((ry[0]+ry[3]+ry[5])/3));  // LANDSCAPE
	    cby = (((ry[2]+ry[4]+ry[7])/3));  // LANDSCAPE
	  }
	  if (cty<cby)
	  {
	    cty = cty - (py*10);
	    cby = cby + (py*10);
	  }
	  else
	  {
	    cty = cty + (py*10);
	    cby = cby - (py*10);
	  }

	  calx = (long(clx)<<14) + long(crx);
	  caly = (long(cty)<<14) + long(cby);
	  if (TOUCH_ORIENTATION == LANDSCAPE)
	    cals = cals + (1L<<31);

	  done();

}

void TouchCalibration::drawCrossHair(int x, int y)
{
  myUTFT.drawRect(x-10, y-10, x+10, y+10);
  myUTFT.drawLine(x-5, y, x+5, y);
  myUTFT.drawLine(x, y-5, x, y+5);
}

void TouchCalibration::readCoordinates()
{
	int iter = 2000;
	  int cnt = 0;
	  uint32_t tx=0;
	  uint32_t ty=0;
	  boolean OK = false;

	  while (OK == false)
	  {
	    myUTFT.setColor(255, 255, 255);
	    myUTFT.print("*  PRESS  *", CENTER, text_y_center);
	    while (myTouch.dataAvailable() == false) {}
	    myUTFT.print("*  HOLD!  *", CENTER, text_y_center);
	    while ((myTouch.dataAvailable() == true) && (cnt<iter))
	    {
	      myTouch.read();
	      if (!((myTouch.TP_X==65535) || (myTouch.TP_Y==65535)))
	      {
	        tx += myTouch.TP_X;
	        ty += myTouch.TP_Y;
	        cnt++;
	      }
	    }
	    if (cnt>=iter)
	    {
	      OK = true;
	    }
	    else
	    {
	      tx = 0;
	      ty = 0;
	      cnt = 0;
	    }
	  }

	  cx = tx / iter;
	  cy = ty / iter;


}

void TouchCalibration::calibrate(int x, int y, int i)
{
	 myUTFT.setColor(255, 255, 255);
	  drawCrossHair(x,y);
	  myUTFT.setBackColor(255, 0, 0);
	  readCoordinates();
	  myUTFT.setColor(255, 255, 255);
	  myUTFT.print("* RELEASE *", CENTER, text_y_center);
	  myUTFT.setColor(80, 80, 80);
	  drawCrossHair(x,y);
	  rx[i]=cx;
	  ry[i]=cy;
	  while (myTouch.dataAvailable() == true)
	  {
	    myTouch.read();
	  }
}

void TouchCalibration::waitForTouch()
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

void TouchCalibration::toHex(uint32_t num)
{
  buf[0] = '0';
  buf[1] = 'x';
  buf[10] = 'U';
  buf[11] = 'L';
  buf[12] = 0;
  for (int zz=9; zz>1; zz--)
  {
    if ((num & 0xF) > 9)
      buf[zz] = (num & 0xF) + 55;
    else
      buf[zz] = (num & 0xF) + 48;
    num=num>>4;
  }
}

void TouchCalibration::startup()
{
	 myUTFT.setColor(255, 0, 0);
	  myUTFT.fillRect(0, 0, dispx-1, 13);
	  myUTFT.setColor(255, 255, 255);
	  myUTFT.setBackColor(255, 0, 0);
	  myUTFT.drawLine(0, 14, dispx-1, 14);
	  myUTFT.print("UTouch Calibration", CENTER, 1);
	  myUTFT.setBackColor(0, 0, 0);

	  if (dispx==220)
	  {
	    myUTFT.print("Use a stylus or something", LEFT, 30);
	    myUTFT.print("similar to touch as close", LEFT, 42);
	    myUTFT.print("to the center of the", LEFT, 54);
	    myUTFT.print("highlighted crosshair as", LEFT, 66);
	    myUTFT.print("possible. Keep as still as", LEFT, 78);
	    myUTFT.print("possible and keep holding", LEFT, 90);
	    myUTFT.print("until the highlight is", LEFT, 102);
	    myUTFT.print("removed. Repeat for all", LEFT, 114);
	    myUTFT.print("crosshairs in sequence.", LEFT, 126);
	    myUTFT.print("Touch screen to continue", CENTER, 162);
	  }
	  else
	  {
	    myUTFT.print("INSTRUCTIONS", CENTER, 30);
	    myUTFT.print("Use a stylus or something similar to", LEFT, 50);
	    myUTFT.print("touch as close to the center of the", LEFT, 62);
	    myUTFT.print("highlighted crosshair as possible. Keep", LEFT, 74);
	    myUTFT.print("as still as possible and keep holding", LEFT, 86);
	    myUTFT.print("until the highlight is removed. Repeat", LEFT, 98);
	    myUTFT.print("for all crosshairs in sequence.", LEFT, 110);

	    myUTFT.print("Further instructions will be displayed", LEFT, 134);
	    myUTFT.print("when the calibration is complete.", LEFT, 146);

	    myUTFT.print("Do NOT use your finger as a calibration", LEFT, 170);
	    myUTFT.print("stylus or the result WILL BE imprecise.", LEFT, 182);

	    myUTFT.print("Touch screen to continue", CENTER, 226);
	  }

	  waitForTouch();
	  myUTFT.clrScr();
}

void TouchCalibration::done()
{
	myUTFT.clrScr();
	  myUTFT.setColor(255, 0, 0);
	  myUTFT.fillRect(0, 0, dispx-1, 13);
	  myUTFT.setColor(255, 255, 255);
	  myUTFT.setBackColor(255, 0, 0);
	  myUTFT.drawLine(0, 14, dispx-1, 14);
	  myUTFT.print("UTouch Calibration", CENTER, 1);
	  myUTFT.setBackColor(0, 0, 0);

	  if (dispx==220)
	  {
	    myUTFT.print("To use the new calibration", LEFT, 30);
	    myUTFT.print("settings you must edit the", LEFT, 42);
	    myUTFT.setColor(160, 160, 255);
	    myUTFT.print("UTouchCD.h", LEFT, 54);
	    myUTFT.setColor(255, 255, 255);
	    myUTFT.print("file and change", 88, 54);
	    myUTFT.print("the following values. The", LEFT, 66);
	    myUTFT.print("values are located right", LEFT, 78);
	    myUTFT.print("below the opening comment.", LEFT, 90);
	    myUTFT.print("CAL_X", LEFT, 110);
	    myUTFT.print("CAL_Y", LEFT, 122);
	    myUTFT.print("CAL_S", LEFT, 134);
	    toHex(calx);
	    myUTFT.print(buf, 75, 110);
	    toHex(caly);
	    myUTFT.print(buf, 75, 122);
	    toHex(cals);
	    myUTFT.print(buf, 75, 134);
	  }
	  else
	  {
	    myUTFT.print("CALIBRATION COMPLETE", CENTER, 30);
	    myUTFT.print("To use the new calibration", LEFT, 50);
	    myUTFT.print("settings you must edit the", LEFT, 62);
	    myUTFT.setColor(160, 160, 255);
	    myUTFT.print("UTouchCD.h", LEFT, 74); //USE EEPROM
	    myUTFT.setColor(255, 255, 255);
	    myUTFT.print("file and change", 88, 74);
	    myUTFT.print("the following values.", LEFT, 86);
	    myUTFT.print("The values are located right", LEFT, 98);
	    myUTFT.print("below the opening comment in", LEFT, 110);
	    myUTFT.print("the file.", LEFT, 122);
	    myUTFT.print("CAL_X", LEFT, 150);
	    myUTFT.print("CAL_Y", LEFT, 162);
	    myUTFT.print("CAL_S", LEFT, 174);

	    toHex(calx);
	    myUTFT.print(buf, 75, 150);
	    toHex(caly);
	    myUTFT.print(buf, 75, 162);
	    toHex(cals);
	    myUTFT.print(buf, 75, 174);
	  }
}


//
// END OF FILE
//
