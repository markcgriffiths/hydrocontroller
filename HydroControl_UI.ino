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
 
// Hydroponics UI code.
//

/*Digital Pin
INT5 :        18   FREE
INT4 :        19   FREE
INT3 :        20   I2C
INT2 :        21   I2C --RECEIVER
INT1 :        3    // UI
INT0 :        2*/  // UI

//Looks like a hack to include the files here but it's not.
//it goes in line with the whole processing paradigm. 
//Basically a sketch is something that you use to fuse all the working 
//pieces together so providing all the glue logic in the sketch is very appropriate.
//#include <PCM.h> //To play a tone
#include <NewRemoteTransmitter.h>  //Plugs
#include <NewRemoteReceiver.h>
#include <dht11.h> // Air Temp
#include <PhidgetsORPpH.h>  //Ph ORP
#include <Wire.h>  //I2C
#include <EEPROMex.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS3234.h> //RTC
#include "Timer.h"
#include <SimpleTimer.h>
//#include <PlugScreen.h>



#include <UTFT.h> // Graphics Library
#include <UTouch.h> //Touch Screen Library
#include <UTFT_Buttons.h>


#include <HydroponicsEngine.h> //The Engine of the system.
#include <HydroponicsUI.h> //The UI of the system.

HydroponicsEngine Hydro_Eng;
HydroponicsUI Hydro_UI;

SimpleTimer simpleTimer;

//49 so it updates OK on first loop.
int loopCounter = 49;
boolean isDataReceived = false;

/*************************
**  Required functions  **
*************************/

void setup()
{
  Serial.begin(115200);
  Hydro_Eng.begin();
  Hydro_UI.begin(&Hydro_Eng);
  
  //Update the UI of the latest Sensor Info
  Hydro_UI.updateInfoFromSensors( Hydro_Eng.getSensorInformation() );

  Hydro_UI.drawMainScreen();
}

void loop()
{
//while(true)
 //{
 //Serial.println("Looping");
 simpleTimer.run();
 loopCounter++;
 //Check sensor information every 5s ( 0.1s*50 )
 if( (loopCounter%50) == 0  )
   {
   loopCounter = 0;
   //Get/update the sensors information
   Hydro_Eng.pHLevel();
   Hydro_Eng.airTemperatureHumidity();
   Hydro_Eng.dateAndTime();
   Hydro_Eng.waterTemp();
   Hydro_Eng.lightValue();
   Hydro_Eng.ECLevel();
   }
 //Check the time every second.
 //if( (loopCounter%10) == 0 && (loopCounter != 0) )
 //  {
 //   Hydro_Eng.dateAndTime();
 //  }
   
 //If the sensors or time have updated some values we may need to show
 //those on the UI
 if( Hydro_Eng.doesScreenNeedUpdating() )
   {
   //Update the UI of the latest Sensor Info
   Hydro_UI.updateInfoFromSensors( Hydro_Eng.getSensorInformation() );
   //Update the screen with the new info.
   Hydro_UI.refreshScreen();
   //Inform the engine that the UI has been updated.
   Hydro_Eng.screenUpdated();
   }  
 

 //Any bigger delay and touch screen does not respond.
 //Quick enough.
 delay(100); //loop every 100ms/0.1s
 //If the screen is touched.
 Hydro_UI.CheckForScreenTouch();
     
 // }//End while true.
 }



  

