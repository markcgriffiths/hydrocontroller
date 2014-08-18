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


#include "HydroponicsEngine.h"
#include <DS3234.h> //Real Time Clock
#include "UI_Shared.h"

// Create a transmitter on address 9246490, using digital pin 10 to transmit, 
// with a period duration of 261us (default=260), repeating the transmitted
// code 2^3=8 times.
NewRemoteTransmitter transmitter(9246490, TRANSMITTER_PIN, 261, 3);
//Create a new DHT11 temp/humidity sensor object.
dht11 DHT11;
//Create a new object for the ORP/pH Sensor
//PhidgetsORPpH phidgets_PH(PHIDGETS_PH_PIN);

// Init the DS3234
DS3234 rtc(RTC_PIN);
// Init a Time-data structure
Time clock;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature waterTempSensor(&oneWire);

unsigned short lightUnitID;
unsigned short pumpUnitID;
unsigned short heaterUnitID;
static NewRemoteTransmitter iTransmitter;

extern String ECLevelString;

String waterTempString;

HydroponicsEngine::HydroponicsEngine()
{

}

void HydroponicsEngine::begin() {

   Serial.begin(115200);
   pinMode(RTC_PIN, OUTPUT);
   Wire.begin();
// Start up the library
  waterTempSensor.begin();
  setTransmitterInformation();
  
  waterMaxTemp = EEPROM.read(EEPROM_WATERMAXTEMP_ADDRESS);
  waterMinTemp = EEPROM.read(EEPROM_WATERMINTEMP_ADDRESS);

  int hour = EEPROM.read(EEPROM_PUMP_DURATION_TIMER_HOUR);
  int min = EEPROM.read(EEPROM_PUMP_DURATION_TIMER_MIN);
  SetPumpDurationTimer(hour, min );


  hour = EEPROM.read(EEPROM_PUMP_TIMER_HOUR);
  min = EEPROM.read(EEPROM_PUMP_TIMER_MIN);
  SetPumpTimer(hour, min );

  // Initialize the rtc object
  rtc.begin();

  //To make sure it updates at boot.
  sensorInfo.minute = -1;

  updateScreen = true;
    
  //Get the sensor info
  pHLevel();
  airTemperatureHumidity();
  dateAndTime();
  waterTemp();
  ECLevel();
   
}

//Changes the Time of the clock.
void HydroponicsEngine::SetTime(int aHour, int aMin )
{
	// The following lines can be uncommented and edited to set the time and date in the DS3234
	//rtc.setDOW(WEDNESDAY);        // Set Day-of-Week to SATURDAY
	rtc.setTime(aHour, aMin, 0);       // Set the time to 12:00:00 (24hr format)
	//rtc.setDate(9, 07, 2014);    // Set the date to January 25th, 2014
  }


void HydroponicsEngine::SetWaterThresholds(int aMax, int aMin )
{
	waterMaxTemp = aMax;
	waterMinTemp = aMin;
}

//Changes the Time of the clock.
void HydroponicsEngine::GetTime(int& aHour, int& aMin )
{
	clock = rtc.getTime();
	aHour = clock.hour;
	aMin = clock.min;
  }

//Gets the air temperature and humidity from the DHT11 Sensor.
void HydroponicsEngine::airTemperatureHumidity()
{
  
  //Serial.println("\n");
  //Read the PIN to get the values.
  int chk = DHT11.read(DHT11_PIN);
  Serial.println(sensorInfo.airTemp);
  //If there is a temp change.
  //SENSOR* sensorInfo = (SENSOR*)sensorInfo;
  if(  sensorInfo.airTemp != DHT11.temperature() )
  {
  sensorInfo.airTemp = DHT11.temperature();
  updateScreen = true;
  updateAir = true;
  }
  
 
  
  /*Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
		Serial.println("OK"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.println("Checksum error"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.println("Time out error"); 
		break;
    default: 
		Serial.println("Unknown error"); 
		break;
  }

  Serial.print("Humidity (%): ");
  Serial.println(DHT11.humidity());*/

  //Serial.print("Temperature (oC): ");
  //Serial.println(DHT11.temperature());
  //airTemp = DHT11.temperature();

  /*Serial.print("Temperature (oF): ");
  Serial.println(DHT11.fahrenheit());

  Serial.print("Temperature (K): ");
  Serial.println(DHT11.kelvin());

  Serial.print("Dew Point (oC): ");
  Serial.println(DHT11.dewPoint());

  Serial.print("Dew PointFast (oC): ");
  Serial.println(DHT11.dewPointFast());*/
  
  }
  
  //Gets the pH level from the Phidgets sensor.;
void HydroponicsEngine::pHLevel(){
  

  //Read the pH level, needs the water temp for accurate reading.
  //phidgets_PH.readpHLevel(sensorInfo.waterTemp);

  //Serial.print("pH Level ");
  //Serial.println(phidgets_PH.pHLevel());

  //If there is a ph change.
  if(  sensorInfo.phLevel != 0 /*phidgets_PH.pHLevel()*/ )
  	{
  	Serial.println("PH TRUE");
  	sensorInfo.phLevel = 0;//phidgets_PH.pHLevel();
  	updateScreen = true;
  	updatepH = true;
  	}
  
  }

void HydroponicsEngine::ECLevel()
{

  float ecLevel = ECLevelString.toInt();

  //If there is a ph change.
  if(  sensorInfo.eCLevel != ecLevel )
  	{
  	Serial.println("EC TRUE");
  	sensorInfo.eCLevel = ecLevel;
  	updateScreen = true;
  	updateEC = true;
  	}

  }
  
  //Gets how much light there is.
void HydroponicsEngine::lightValue(){
  
  Serial.println(analogRead(LIGHT_PIN) );
  sensorInfo.lightLevel = analogRead(LIGHT_PIN);
  Serial.println("LIGHT VALUE");
  Serial.println(sensorInfo.lightLevel);
  if(sensorInfo.lightLevel > 290 && !lightPlugOn )
  {
  SwitchLightsValue(true);
  Serial.println("LIGHT ON");
  lightPlugOn = true;
  }
  else if(sensorInfo.lightLevel < 290 && lightPlugOn )
  {
  SwitchLightsValue(false);
  Serial.println("LIGHT OFF");
  }
  }
  
//Gets the temp of the water.
void HydroponicsEngine::waterTemp(){
  
  
  //Serial.print("Requesting temperatures...");
  waterTempSensor.requestTemperatures(); // Send the command to get temperatures
  

  int temp = waterTempSensor.getTempCByIndex(0)+ waterTempCal;
  waterTempString = String(temp);
  //Serial.print("Temp is: ");
  //Serial.println(temp);   //Return Float
  
  //If there is a water temp change.
  if(  sensorInfo.waterTemp != temp )
  {
  sensorInfo.waterTemp = ( waterTempSensor.getTempCByIndex(0) + waterTempCal );
  updateScreen = true;
  updateWater = true;
  }
  

  //Check if we need to turn heater plug on or off
  if( temp > waterMaxTemp )
	  SwitchHeaterValue(false);

  if( temp  < waterMinTemp )
 	  SwitchHeaterValue(true);

  }
  
 //Gets the ORP level from the Phidgets sensor.
void HydroponicsEngine::orpLevel(){
  
  //Serial.println("\n");
  //Read the PIN to get the values.
  //phidgets.readORPLevel();
  
  //Serial.print("ORP Level ");
  //Serial.println(phidgets.orpLevel());
  
  }

//Gets the current date and time.  
void HydroponicsEngine::dateAndTime()
{
  
  clock = rtc.getTime();

  if( sensorInfo.minute != clock.min )
    {
      sensorInfo.hour = clock.hour;
      sensorInfo.minute = clock.min;
     updateScreen = true;
    }
    
  }
  
boolean HydroponicsEngine::doesScreenNeedUpdating()
{
  return updateScreen;
}

boolean HydroponicsEngine::getUpdateAir()
{
  return updateAir;
}

boolean HydroponicsEngine::getUpdateWater()
{
  return updateWater;
}

boolean HydroponicsEngine::getUpdatepH()
{
  return updatepH;
}

boolean HydroponicsEngine::getUpdateEC()
{
  return updateEC;
}

void HydroponicsEngine::screenUpdated()
{
  Serial.println("UPDATE_SCREEN");
  updateScreen = false;
  updateWater = false;
  updateAir = false;
  updateEC = false;
  updatepH = false;
}

//Returns the current state of the sensors.
SENSOR& HydroponicsEngine::getSensorInformation()
{
  return sensorInfo;
}


void HydroponicsEngine::SwitchLightsValue(bool aValue)
{
	lightUnitID = EEPROM.read(EEPROM_LEARNED_LIGHTS_UNIT);
	iTransmitter.sendUnit(lightUnitID, aValue);
	EEPROM.write(EEPROM_LIGHTS_UNIT_STATE, aValue);
}

void HydroponicsEngine::SetPumpTimer(int aHour, int aMin)
{
	Serial.println("SetPumpTimer hour ");
	Serial.println(aHour);
	Serial.println("SetPumpTimer min ");
	Serial.println(aMin);
	pumpTimerInSeconds = (aHour *60 + aMin) *60; //convert to mins
	pumpTimerInSecondsSaved = pumpTimerInSeconds;
	Serial.println("SetPumpTimer seconds ");
	Serial.println(pumpTimerInSeconds);
	//pumpTimerInSeconds = pumpTimerInSeconds * 60; //convert to seconds
	Serial.println("SetPumpTimer seconds ");
	Serial.println(pumpTimerInSeconds);
	Serial.println(" EXIT SetPumpTimer ");
}

void HydroponicsEngine::SetPumpDurationTimer(int aHour, int aMin)
{
	pumpDurationTimerInSeconds = (aHour *60 + aMin) *60; //convert to mins
	pumpDurationTimerInSecondsSaved = pumpDurationTimerInSeconds;
}

void HydroponicsEngine::ResetPumpDurationTimer()
{
	pumpDurationTimerInSeconds = pumpDurationTimerInSecondsSaved;
}

void HydroponicsEngine::ResetPumpTimer()
{
	pumpTimerInSeconds = pumpTimerInSecondsSaved;
}

long HydroponicsEngine::GetPumpTimer()
{
	Serial.println("GetPumpTimer seconds -5 ");
	pumpTimerInSeconds = pumpTimerInSeconds -5; // -5 as main loop checks every 5s.

	if(pumpTimerInSeconds < 0 )
			pumpTimerInSeconds = 0;

	Serial.println("GetPumpTimer seconds return ");
	return pumpTimerInSeconds;
}

long HydroponicsEngine::GetPumpDurationTimer()
{
	pumpDurationTimerInSeconds = pumpDurationTimerInSeconds -5; // -5 as main loop checks every 5s.

	if(pumpDurationTimerInSeconds < 0 )
		pumpDurationTimerInSeconds = 0;

	return pumpDurationTimerInSeconds;
}



void HydroponicsEngine::SwitchPumpValue(bool aValue)
{
	pumpUnitID = EEPROM.read(EEPROM_LEARNED_PUMP_UNIT);
	iTransmitter.sendUnit(pumpUnitID, aValue);
	EEPROM.write(EEPROM_PUMP_UNIT_STATE,aValue);
}

void HydroponicsEngine::SwitchHeaterValue(bool aValue)
{
	heaterUnitID = EEPROM.read(EEPROM_LEARNED_HEATER_UNIT);
	iTransmitter.sendUnit(heaterUnitID, aValue);
	EEPROM.write(EEPROM_HEATER_UNIT_STATE,aValue);
}

void HydroponicsEngine::setTransmitterInformation()
{
	iTransmitter.SetAddress(9246490);
	iTransmitter.SetPin(TRANSMITTER_PIN);
	iTransmitter.SetPeriod(261);
	iTransmitter.SetRepeats(3);
}


//
// END OF FILE
//
