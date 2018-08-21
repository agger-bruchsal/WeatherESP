#ifndef sensors_h
#define sensors_h

#include <Arduino.h>
#include <Adafruit_TSL2591.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <VEML6075.h>

Adafruit_BME680 bme;
Adafruit_TSL2591 tsl;
VEML6075 veml6075;

void GetGasReference(){
  // Now run the sensor for a burn-in period, then use combination of relative humidity and gas resistance to estimate indoor air quality as a percentage.
  Serial.println("Getting a new gas reference value");
  int readings = 10;
  for (int i = 0; i <= readings; i++){ // read gas for 10 x 0.150mS = 1.5secs
    gas_reference += bme.readGas();
  }
  gas_reference = gas_reference / readings;
}

String CalculateIAQ(float score){
  String IAQ_text;
  score = (100-score)*5;
  if      (score >= 301)                  IAQ_text += "Hazardous";
  else if (score >= 201 && score <= 300 ) IAQ_text += "Very Unhealthy";
  else if (score >= 176 && score <= 200 ) IAQ_text += "Unhealthy";
  else if (score >= 151 && score <= 175 ) IAQ_text += "Unhealthy for Sensitive Groups";
  else if (score >=  51 && score <= 150 ) IAQ_text += "Moderate";
  else if (score >=  00 && score <=  50 ) IAQ_text += "Good";
  return IAQ_text;
}

void calculateHumScore(){
  if (sensorReading[2].value >= 38 && sensorReading[2].value <= 42)
    hum_score = 0.25*100; // Humidity +/-5% around optimum 
  else
  { //sub-optimal
    if (sensorReading[2].value < 38) 
      hum_score = 0.25/hum_reference*sensorReading[2].value*100;
    else
    {
      hum_score = ((-0.25/(100-hum_reference)*sensorReading[2].value)+0.416666)*100;
    }
  }
}

float getAirQualityIndex(){
  calculateHumScore();
  int gas_lower_limit = 5000;   // Bad air quality limit
  int gas_upper_limit = 50000;  // Good air quality limit 
  if (gas_reference > gas_upper_limit) gas_reference = gas_upper_limit; 
  if (gas_reference < gas_lower_limit) gas_reference = gas_lower_limit;
  gas_score = (0.75/(gas_upper_limit-gas_lower_limit)*gas_reference -(gas_lower_limit*(0.75/(gas_upper_limit-gas_lower_limit))))*100;
  
  //Combine results for the final IAQ index value (0-100% where 100% is good quality air)
  float air_quality_score = hum_score + gas_score;
  if ((getgasreference_count++)%10==0) GetGasReference(); 
  return(air_quality_score);
}

void sensorSetup(){
  if (tsl.begin()) 
  {
    Serial.println(F("Found a TSL2591 sensor"));
  } 
  else 
  {
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }
  if (bme.begin(0x76))
  {
    Serial.println(F("Found a BME680 sensor"));
  } 
  else 
  {
    Serial.println(F("No BME680 sensor found ... check your wiring?"));
    while (1);
  }
  if (veml6075.begin())
  {
    Serial.println(F("Found a veml6075 sensor"));
  } 
  else 
  {
    Serial.println(F("No veml6075 sensor found ... check your wiring?"));
    while (1);
  }
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
  GetGasReference();
}

void readSensors(){
  if (millis() - lastReadSensors >= INTERVAL){  // if INTERVAL has passed
    lastReadSensors = millis(); 
    veml6075.poll();
    if (! bme.performReading()) {
      Serial.println("Failed to perform reading :(");
      return;
    }
    sensorReading[0].value = bme.temperature;
    sensorReading[1].value = bme.pressure / 100.0;
    sensorReading[2].value = bme.humidity;
    sensorReading[3].value = tsl.getLuminosity(TSL2591_VISIBLE);
    sensorReading[4].value = veml6075.getUVIndex();
    sensorReading[5].value = getAirQualityIndex();
    sensorState[0].value = CalculateIAQ(sensorReading[5].value);
  }
}

#endif