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


#include "HydroponicsUI.h"


//Global definition for whole of the UI
UTFT myUTFT(ITDB32S, 38,39,40,41);
UTouch myTouch(6,5,4,3,2);
UTFT_Buttons myButtons(&myUTFT, &myTouch);

//extern SimpleTimer simpleTimer;


HydroponicsUI::HydroponicsUI() {
}

void HydroponicsUI::begin(HydroponicsEngine* aHydroEng) {

	Serial.begin(115200);
	
	iPhCalibrateScreen = new PhCalibrateScreen;
	iPhCalibrateScreen->begin();

 	ipHScreen = new PhScreen;
 	ipHScreen->begin(EEPROM_PHMAXTEMP_ADDRESS, EEPROM_PHMINTEMP_ADDRESS, 0, true);

 	iECCalibrateScreen = new ECCalibrateScreen;
 	iECCalibrateScreen->begin();

 	iECScreen = new ECScreen;
 	iECScreen->begin(EEPROM_ECMAXTEMP_ADDRESS,EEPROM_ECMINTEMP_ADDRESS);

 	iAirScreen = new AirScreen;
 	iAirScreen->begin(EEPROM_AIRMAXTEMP_ADDRESS, EEPROM_AIRMINTEMP_ADDRESS, EEPROM_AIRTEMPUNITS_ADDRESS);
 	iAirScreen->setDrawUnits(true);

 	iWaterScreen = new WaterScreen;
 	iWaterScreen->begin(EEPROM_WATERMAXTEMP_ADDRESS,EEPROM_WATERMINTEMP_ADDRESS,EEPROM_WATERTEMPUNITS_ADDRESS );
 	iWaterScreen->setDrawUnits(true);

 	iCalibrateTouch = new TouchCalibration;

 	iTimeScreen = new TimeScreen;
 	iTimeScreen->begin();
 	iTimeScreen->SetValueText( "Hour", "Min" );

 	iPlugScreen = new PlugScreen;
 	iPlugScreen->begin();

 	iSetupScreen = new SetupScreen;
 	iSetupScreen->begin();

 	iAboutScreen = new AboutScreen;
 	iAboutScreen->begin();

 	iHydroEng = aHydroEng;

 	iMainScreen = new MainScreen;
 	iMainScreen->begin();
 	whichScreen = MAIN_SCREEN;
 	iMainScreen->drawScreen();


}

void HydroponicsUI::CheckForScreenTouch()
{
  if (myTouch.dataAvailable())
    {
      switch(whichScreen)
      {
        case MAIN_SCREEN:
        {
          handleButtons( iMainScreen->handleScreen() );
          break;
        }
         case ABOUT_SCREEN:
        {
          handleButtons( iAboutScreen->handleScreen() );
          break;
        }
          case SETUP_SCREEN:
        {
          handleButtons( iSetupScreen->handleScreen() );
          break;
        }
         case PH_SCREEN:
        {

          handleButtons( ipHScreen->handleScreen() );
          break;
        }
          case EC_SCREEN:
        {

          handleButtons( iECScreen->handleScreen() );
          limitInfo.maxECLevel = iWaterScreen->getMax();
          limitInfo.minECLevel = iWaterScreen->getMin();
          break;
        }
          case AIR_SCREEN:
        {
        	handleButtons( iAirScreen->handleScreen() );
        	limitInfo.maxAirTemp = iAirScreen->getMax();
        	limitInfo.minAirTemp = iAirScreen->getMin();
          break;
        }
          case WATER_SCREEN:
        {
        	handleButtons( iWaterScreen->handleScreen() );
        	limitInfo.maxWaterTemp = iWaterScreen->getMax();
        	limitInfo.minWaterTemp = iWaterScreen->getMin();
          break;
        }
          case PLUGS_SCREEN:
        {
        	handleButtons( iPlugScreen->handleScreen() );
          break;
        }
          case CLOCK_SCREEN:
        {
        	handleButtons( iTimeScreen->handleScreen() );
          break;
        }
         case CALLIBRATE_EC_SCREEN:
        {
           Serial.println("handle EC Cal Screen();  \n");
           handleButtons( iECCalibrateScreen->handleScreen() );
           break;
        }
         case CALLIBRATE_PH_SCREEN:
        {
           Serial.println("handle PH Cal Screen();  \n");
           handleButtons( iPhCalibrateScreen->handleScreen() );
           break;
        }
        default: //If Error, Just go back to main.
        {
        	Serial.println("ERROR :::: handleMainScreen();  \n");
        	handleButtons( iMainScreen->handleScreen() );
          break;
        }
        
      }
    }  
}

//Updates the screen, if needed, with data from HW.
void HydroponicsUI::refreshScreen(){
  
  //See which screen we are on.
      switch(whichScreen)
      {
        case MAIN_SCREEN: //Main Screen
        {
          //refreshMainScreen();
          iMainScreen->refreshScreen();
          break;
        }

        
      }
  
}

void HydroponicsUI::handleButtons( int aHandleWhichButton ){

	Serial.println("HydroponicsUI::handleButtons \n");
	Serial.println(aHandleWhichButton);
	switch(aHandleWhichButton)
	{
	case NONE:
		{
			return;
		}
	case AIR_BUTTON:
		{
		whichScreen = AIR_SCREEN;
		iAirScreen->drawScreen();
		break;
		}
	case PH_BUTTON:
		{
		whichScreen = PH_SCREEN;
		ipHScreen->drawScreen();
		break;
		}
	case EC_BUTTON:
		{
		whichScreen = EC_SCREEN;
		iECScreen->drawScreen();
		break;
		}
	case WATER_BUTTON:
		{
		whichScreen = WATER_SCREEN;
		iWaterScreen->drawScreen();
		break;
		}
	case ABOUT_BUTTON:
		{
		whichScreen = ABOUT_SCREEN;
		iAboutScreen->drawScreen();
		break;
		}
	case CLOCK_BUTTON:
		{
		whichScreen = CLOCK_SCREEN;
		iTimeScreen->drawScreen();
		break;
		}
	case BACK2SETUP_BUTTON:
		{
		whichScreen = SETUP_SCREEN;
		iSetupScreen->drawScreen();
		break;
		}
	case BACK2MAIN_BUTTON:
		{
		whichScreen = MAIN_SCREEN;
		iMainScreen->setLimitValues( limitInfo );
		iMainScreen->drawScreen();
		break;
		}
	case PLUGS_BUTTON:
		{
		whichScreen = PLUGS_SCREEN;
		iPlugScreen->drawScreen();
		break;
		}
	case LAMPS_BUTTON:
		{
		iHydroEng->setLightsID();
		break;
		}
	case PUMP_BUTTON:
		{

		break;
		}
	case LIGHTSWITCH_BUTTON:
		{
		iHydroEng->SwitchLightsValue(true);
		break;
		}
	case EC_CALIBRATION_BUTTON:
		{
		whichScreen = CALLIBRATE_EC_SCREEN;
	    iECCalibrateScreen->drawScreen();
		break;
		}
	case PH_CALIBRATION_BUTTON:
		{
		whichScreen = CALLIBRATE_PH_SCREEN;	
	    iPhCalibrateScreen->drawScreen();
		break;
		}
	}


}

void HydroponicsUI::updateInfoFromSensors( SENSOR& sensorInformation )
{
 iMainScreen->updateInfoFromSensors( sensorInformation );

}

//
// END OF FILE
//
