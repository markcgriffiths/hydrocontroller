// 
// FILE: HydroponicsUI.h
// VERSION: 0.0.1
// PURPOSE: Handle the UI of the Hydroponics System.

//
//
// HISTORY:
// Mark Griffiths - Original version
// 

#ifndef ABOUTSCREEN_H
#define ABOUTSCREEN_H

#include <string.h>
#include <UI_Shared.h>
#include "Screen.h"

class AboutScreen : public Screen
{
public:
	//Functions
	AboutScreen();
	void begin();
	int handleScreen();
	void drawScreen();
	void handleExitScreen();
	void resetButtons();

};

#endif
//
// END OF FILE
//
