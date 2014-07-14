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

#ifndef MinMaxScreen_h
#define MinMaxScreen_h

#include "UI_Shared.h"


class MinMaxScreen
{
public:
	//Functions
	MinMaxScreen();
	void begin(int AddMax=0, int AddMin=0, int AddUnit=0, boolean useFloats = false);
	void drawMinMaxScreen(int max, int min, int units);
	void refreshMinMaxScreen(int max, int min, int units);
	void drawMinMaxScreen(float max, float min, int units);
	void refreshMinMaxScreen(float max, float min, int units);
	int handleMinMaxScreen(float& max, float& min, int& units);
	int handleMinMaxScreen(int& max, int& min, int& units);
	void setDrawUnits(bool aDrawUnits );
	void drawHeader( char* aHeaderText,  char* aSubText  );
	void SetValueText( char* aUpperText,  char* aLowerText  );

private:
	void resetButtons();
	void drawValueBox( char* aText, int aMax, int x, int y );
	void drawValueBox( char* aText, float aValue, int x, int y );
	void drawUnitsButton( int aUnits, int x, int y );
	void getSavedValues(int AddMax, int AddMin, int AddUnit, boolean useFloats);



	virtual void refreshScreen() = 0;
	virtual int handleScreen() = 0;
	virtual void handleExitScreen() = 0;
	

	protected:
	int isExitScreen;
	int  max, min, units;
	float floatMax, floatMin;
    char* iUpperText;
    char* iLowerText;


	protected:
    bool drawUnits;
    int plusMaxButton, minusMaxButton, plusMinButton, minusMinButton, unitsButton, back2MainButton;
    
};

#endif
//
// END OF FILE
//
