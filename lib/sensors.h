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
    sensorReading[3].value = bme.gas_resistance / 1000.0;
    sensorReading[4].value = tsl.getLuminosity(TSL2591_VISIBLE);
    uva_radiation = veml6075.getUVA();
    uvb_radiation = veml6075.getUVB();
    sensorReading[5].value = veml6075.getUVIndex();
  }
}

#endif