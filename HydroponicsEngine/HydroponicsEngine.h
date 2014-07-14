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


#ifndef HydroponicsEngine_h
#define HydroponicsEngine_h

//Used for testing the engine code.
#define TEST_STUBBS_ON false

#include <Arduino.h>
#include <EEPROM.h>

//The DHT11 sensor is on pin 8.
#define DHT11_PIN 8
#define ONE_WIRE_BUS 12//11  Water Temp
#define LIGHT_PIN 2
#define RTC_PIN 9
const byte TRANSMITTER_PIN = 11;


#if TEST_STUBBS_ON
//Make test stub of these
#include "../PCM_Tones/PCM.h" //To play a tone
#include "../NewRemoteSwitch/NewRemoteTransmitter.h"  //Plugs
#include "../DHT11/dht11.h" // Air Temp
#include "../PhidgetsORPpH/PhidgetsORPpH.h"  //Ph ORP
#include <Wire.h>  //I2C
#include <OneWire.h>

#include <DallasTemperature_Test_Stub.h>


#else
#include "../PCM_Tones/PCM.h" //To play a tone
#include "../NewRemoteSwitch/NewRemoteTransmitter.h"  //Plugs
#include "../NewRemoteSwitch/NewRemoteReceiver.h"  //Plugs
#include "../DHT11/dht11.h" // Air Temp
#include "../PhidgetsORPpH/PhidgetsORPpH.h"  //Ph ORP
#include <Wire.h>  //I2C
#include <OneWire.h>
#include <DallasTemperature.h>

#endif




//All the data from the HW sensors.
    struct SENSOR{
    float phLevel;
    int eCLevel;
    int airTemp;
    int waterTemp;
    int hour;
    int minute;
    int lightLevel;
    };
    
//CALIBRATE THE SENSORS HERE
const int waterTempCal = -1;

#define HYDROPONICSENGINELIB_VERSION "0.0.1"



class HydroponicsEngine
{
public:
	//Functions
	HydroponicsEngine();
	void begin();
	void airTemperatureHumidity();
	void pHLevel();
	void orpLevel();
	void dateAndTime();
	boolean doesScreenNeedUpdating();
	void screenUpdated();
	void getAirTemp();
	void waterTemp();
	void lightValue();
	SENSOR& getSensorInformation();
	void setLightsID();
	static void showLightsCode(NewRemoteCode receivedCode);
	void setPumpID();
	void ECLevel();
	static void showPumpCode(NewRemoteCode receivedCode);
	static void SwitchLightsValue(bool aValue);
	static void SwitchPumpValue(bool aValue);
    
	
	private:
	//Member Variables
    boolean updateScreen;
    int minuteKeep;
    struct SENSOR sensorInfo;
    bool lightPlugOn; //Keeps track if the light is on.
    NewRemoteReceiver* iReciever;
    
  
};
#endif
//
// END OF FILE
//
