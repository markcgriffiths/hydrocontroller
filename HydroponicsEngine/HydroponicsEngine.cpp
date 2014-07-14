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

static byte lightUnitID;
static byte pumpUnitID;
static NewRemoteTransmitter iTransmitter;

extern String ECLevelString;

String waterTempString;

HydroponicsEngine::HydroponicsEngine() {
 
	updateScreen = true;
   
}

void HydroponicsEngine::begin() {

   Serial.begin(115200);
   pinMode(RTC_PIN, OUTPUT);
   Wire.begin();
// Start up the library
  waterTempSensor.begin();
  
  // Initialize the rtc object
    rtc.begin();

  // The following lines can be uncommented and edited to set the time and date in the DS3234
  //rtc.setDOW(WEDNESDAY);        // Set Day-of-Week to SATURDAY
  //rtc.setTime(22, 39, 0);       // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(9, 07, 2014);    // Set the date to January 25th, 2014

  //To make sure it updates at boot.
  sensorInfo.minute = -1;
    
  //Get the sensor info
  pHLevel();
  airTemperatureHumidity();
  dateAndTime();
  waterTemp();
  ECLevel();
   
}

//Gets the air temperature and humidity from the DHT11 Sensor.
void HydroponicsEngine::airTemperatureHumidity(){
  
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
  	}

  }
  
  //Gets how much light there is.
void HydroponicsEngine::lightValue(){
  
  Serial.println(analogRead(LIGHT_PIN) );
  sensorInfo.lightLevel = analogRead(LIGHT_PIN);
  if(sensorInfo.lightLevel > 290 && !lightPlugOn )
  {
   // Switch unit 0 on
  transmitter.sendUnit(0, true);
  lightPlugOn = true;
  }
  else if(sensorInfo.lightLevel < 290 && lightPlugOn )
  {
  // Switch unit 0 on
  transmitter.sendUnit(0, false);
  lightPlugOn = false;
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
  }
  
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

void HydroponicsEngine::screenUpdated()
{
  Serial.println("UPDATE_SCREEN");
  updateScreen = false;
}

//Returns the current state of the sensors.
SENSOR& HydroponicsEngine::getSensorInformation()
{
  return sensorInfo;
}

void HydroponicsEngine::setLightsID()
{
  Serial.println("SET_LIGHTS_ID");
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  int interrupt = 4;
  int repeats =1 ;
  iTransmitter.SetAddress(9246490);
  iTransmitter.SetPin(TRANSMITTER_PIN);
  iTransmitter.SetPeriod(261);
  iTransmitter.SetRepeats(3);
  iTransmitter.sendUnit(0, false);
  iTransmitter.sendUnit(1, false);
  iTransmitter.sendUnit(2, false);
  NewRemoteReceiver::init(interrupt, repeats, showLightsCode);
}


void HydroponicsEngine::showLightsCode(NewRemoteCode receivedCode)
{
  Serial.println("SHOW_CODE_ID");
  // Note: interrupts are disabled. You can re-enable them if needed.

   // Print the received code.
   Serial.print("Addr ");
   Serial.print(receivedCode.address);

   if (receivedCode.groupBit) {
     Serial.print(" group");
   } else {
     Serial.print(" unit ");
     Serial.print(receivedCode.unit);
     lightUnitID = receivedCode.unit;
   }

   switch (receivedCode.switchType) {
     case 0:
       Serial.print(" off");
       break;
     case 1:
       Serial.print(" on");
       break;
     case 2:
       Serial.print(" dim level ");
       Serial.print(receivedCode.dimLevel);
       break;
     case 3:
       Serial.print(" on with dim level ");
       Serial.print(receivedCode.dimLevel);
       break;
   }

   Serial.print(", period: ");
   Serial.print(receivedCode.period);
   Serial.println("us.");

   iTransmitter.SetAddress(receivedCode.address);
   iTransmitter.SetPin(TRANSMITTER_PIN);
   iTransmitter.SetPeriod(receivedCode.period);
   iTransmitter.SetRepeats(4);

}

void HydroponicsEngine::setPumpID()
{
  Serial.println("SET_PUMP_ID");
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  NewRemoteReceiver::init(4, 2, showPumpCode);

}
void HydroponicsEngine::showPumpCode(NewRemoteCode receivedCode)
{
  Serial.println("SHOW_PUMP_CODE_ID");
  pumpUnitID = receivedCode.unit;
  iTransmitter.SetAddress(receivedCode.address);
  iTransmitter.SetPin(TRANSMITTER_PIN);
  iTransmitter.SetPeriod(receivedCode.period);
  iTransmitter.SetRepeats(4);
}

void HydroponicsEngine::SwitchLightsValue(bool aValue)
{
	Serial.println("SWITCH LIGHTS");
	iTransmitter.SetAddress(9246490);
	iTransmitter.SetPin(TRANSMITTER_PIN);
	iTransmitter.SetPeriod(261);
	iTransmitter.SetRepeats(3);
	iTransmitter.sendUnit(/*lightUnitID*/0, aValue);
	iTransmitter.sendUnit(/*lightUnitID*/1, aValue);
	iTransmitter.sendUnit(/*lightUnitID*/2, aValue);
}

void HydroponicsEngine::SwitchPumpValue(bool aValue)
{
	iTransmitter.sendUnit(pumpUnitID, aValue);
}


//
// END OF FILE
//
